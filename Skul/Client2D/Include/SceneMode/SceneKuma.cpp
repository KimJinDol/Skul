#include "SceneKuma.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "BackgroundImage.h"
#include "Object/Skul.h"
#include "Component/SpriteComponent.h"
#include "Resource/ResourceManager.h"
#include "../Object/MainMap.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"
#include "UI/SkulWidget.h"
#include "../Object/Portal.h"
#include "Object/OgreAI.h"
#include "Scene/Viewport.h"
#include "Object/WarriorAI.h"
#include "Object\KumaAI.h"
#include "Dialogue/DialogueManager.h"
#include "UI/DialogueWindow.h"
#include "Engine.h"
#include "BasicMath.h"

CSceneKuma::CSceneKuma()	:
	m_BossFightPos(2200.f, 0.f, 0.f),
	m_bCheckBossFightStart(false),
	m_bCameraFocus(false),
	m_bBossIntroCheck(false),
	m_bDialogueCheck(false),
	m_CameraAreaRatio(0.7f),
	m_CurRatioTime(0.f),
	m_RatioTime(1.f),
	m_bFadeIn(false),
	m_bFadeOut(false)
{
}

CSceneKuma::~CSceneKuma()
{
}

void CSceneKuma::FocusCamera()
{
	m_bCheckBossFightStart = true;
	m_CameraFocusPos = Vector2(1900.f, 300.f);
	m_bCameraFocus = true;
	m_pScene->GetCameraManager()->GetCurrentCamera()->StartCameraFocus(m_CameraFocusPos, 2.f);
	m_Skul->GetWidget()->Enable(false);
	m_bFadeIn = true;
}

void CSceneKuma::StartDialogue()
{
	m_bDialogueCheck = true;
	CDialogueManager::GetInst()->StartDialogue();
	CDialogueManager::GetInst()->SetEndDialogueFunc<CSceneKuma>(this, &CSceneKuma::KumaDrinkAnimation);
	m_pDialogueWindow->SetNextDialogue(0.f);
}

void CSceneKuma::KumaDrinkAnimation()
{
	m_Kuma->Intro();
}

void CSceneKuma::StartFight()
{
	m_Kuma->IntroEnd();
	m_Skul->SetEnable();
	m_Skul->GetWidget()->Enable(true);

	m_bFadeOut = true;
	m_bFadeIn = false;
}

bool CSceneKuma::Init()
{
	CreateAnimation();
	CreateParticle();

	m_pScene->GetResource()->LoadSound("BGM", false, "Adventurer",
		"AudioClip/BGM/Adventurer.wav");
	m_pScene->GetResource()->SoundPlay("Adventurer");
	m_pScene->GetResource()->SetVolume(100.f);

	CBackgroundImage* Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundColor");
	CSpriteComponent* BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	CMaterial* BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundColorTex2", TEXT("Map/Background/Background12.png"));
	BackgroundMtrl->SetTransparency(false);

	Resolution RS = m_pScene->GetCameraManager()->GetCurrentCamera()->GetResolution();
	Vector3 Scale(RS.Width * 1.3f, RS.Height * 2.f, 1.f);

	Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundTree01");
	Background->SetCameraInfluend(0.00005f);
	BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	BackgroundSprite->SetWorldScale(Scale);
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundTree04Tex", TEXT("Map/Background/Background10.png"));
	BackgroundMtrl->SetTintColor(0.2f, 0.2f, 0.2f, 0.f);

	Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundTree02");
	Background->SetCameraInfluend(0.0001f);
	BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundSprite->SetWorldScale(Scale);
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundTree05Tex", TEXT("Map/Background/Background2.png"));
	BackgroundMtrl->SetTintColor(0.2f, 0.2f, 0.2f, 0.f);

	Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundTree03");
	Background->SetCameraInfluend(0.0002f);
	Background->SetInitUV(0.f, -0.05f);
	BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundSprite->SetWorldScale(Scale);
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundTree06Tex", TEXT("Map/Background/Background0.png"));
	BackgroundMtrl->SetTintColor(0.2f, 0.2f, 0.2f, 0.f);

	//CGameObject* Gate = m_pScene->SpawnObject<CGameObject>("CastleGate");
	//CSpriteComponent* Sprite = Gate->CreateSceneComponent<CSpriteComponent>("Sprite");
	//Gate->SetRootComponent(Sprite);
	//Sprite->SetRender2DType(Render_Type_2D::RT2D_Background);
	//Sprite->GetMaterial(0)->AddTexture("CastleGate", TEXT("Map/Prop/CastleGate.png"));
	//Sprite->SetWorldScale(332.f, 628.f, 1.f);
	//Sprite->SetWorldPos2D(2390.f, 285.f);

	m_Skul = m_pScene->SpawnObject<CSkul>("Skul");
	m_Skul->SetWorldPos2D(300.f, 256.f);
	m_Skul->SetSubState(ESkulState::ChiefGuard);

	/*m_Portal = m_pScene->SpawnObject<CPortal>("Portal");
	m_Portal->SetWorldPos2D(2720.f, 500.f);*/

	CMainMap* MainMap = m_pScene->SpawnObject<CMainMap>("TileMap");
	MainMap->Load(TEXT("SceneKuma.map"));

	CTileMapComponent* TileMap = (CTileMapComponent*)MainMap->FindSceneComponent("TileMap");

	m_pScene->GetCameraManager()->GetCurrentCamera()->Set2DWorldSize(TileMap->GetTileMapSize());


	m_Kuma = m_pScene->SpawnObject<CKumaAI>("Kuma");
	m_Kuma->SetWorldPos(3500.f, 448.f, 0.f);
	m_Kuma->SetEnableController(false);

	m_Kuma->GetAnim()->SetFrameEndFunction<CSceneKuma>("KumaIntro", this, &CSceneKuma::StartFight);

	m_pDialogueWindow = m_pScene->GetViewport()->AddWindow<CDialogueWindow>("DialogueWindow");
	CDialogueManager::GetInst()->SetDialogueWindow(m_pDialogueWindow);
	CDialogueManager::GetInst()->SetDialCode("# Final Fight of Adventurers,,,,,,,,,,,,");


    return CSceneMode::Init();
}  

void CSceneKuma::Update(float DeltaTime)
{
    CSceneMode::Update(DeltaTime);

	if (!m_bCheckBossFightStart && m_Skul->GetWorldPos().x >= m_BossFightPos.x)
	{
		FocusCamera();
		m_Skul->SetEnable(false, false, false);
	}

	if (!m_bBossIntroCheck && m_bCameraFocus && m_pScene->GetCameraManager()->GetCurrentCamera()->GetWorldPos().Distance(Vector3(m_CameraFocusPos.x, m_CameraFocusPos.y, 0.f)) < 10.f)
	{
		m_bBossIntroCheck = true;
		m_Kuma->SetEnableController(true);
	}

	if (!m_bDialogueCheck && m_Kuma->IsIntroJumpStampingEnd())
	{
		StartDialogue();
	}

	if (m_bFadeIn)
	{
		m_CurRatioTime += DeltaTime;
		float Ratio = BasicMath::FLerp(1.f, m_CameraAreaRatio, m_CurRatioTime / m_RatioTime);
		CEngine::GetInst()->SetCameraAreaRatio(Ratio);
	}
	if (m_bFadeOut)
	{
		m_CurRatioTime += DeltaTime;
		float Ratio = BasicMath::FLerp(m_CameraAreaRatio, 1.f, m_CurRatioTime / m_RatioTime);
		CEngine::GetInst()->SetCameraAreaRatio(Ratio);
	}
}

void CSceneKuma::CreateMaterial()
{
}

void CSceneKuma::CreateAnimation()
{
	CSceneResource* SceneResource = m_pScene->GetResource();
	SceneResource->LoadTexture("Effect_Altas", TEXT("Effect/Effect_Altas.png"));
	SceneResource->LoadTexture("Portal_Atlas", TEXT("Object/Portal.png"));
	SceneResource->LoadTexture("Kuma_Atlas00", TEXT("Enemy/Kuma/Kuma_Atlas00.png"));
	SceneResource->LoadTexture("Kuma_Atlas01", TEXT("Enemy/Kuma/Kuma_Atlas01.png"));
	SceneResource->LoadTexture("UltimateSkill_Aura_Atkas", TEXT("Effect/UltimateSkill_Aura_Atkas.png"));
	SceneResource->LoadTexture("UltimateSkill_Complete_Atlas", TEXT("Effect/UltimateSkill_Complete_Atlas.png"));
	SceneResource->LoadTexture("Fighter_Aura_Atlas", TEXT("Effect/Fighter_Aura_Atlas.png"));
	SceneResource->LoadTexture("InSign_Atlas", TEXT("Effect/InSign_Atlas.png"));
	SceneResource->LoadTexture("BossP2_BothFistSlam_Smoke_Atlas", TEXT("Effect/BossP2_BothFistSlam_Smoke_Atlas.png"));
	SceneResource->LoadTexture("StampingEffect", TEXT("Effect/BOSS_STAMPING_HIT_EFFECT_0-Sheet.png"));
	SceneResource->LoadTexture("KumaRock", TEXT("Object/Veteran_Warrior_Rock_Projectile.png"));
	SceneResource->LoadTexture("KumaRockSmoke", TEXT("Effect/Ch5BossP1_Slam_Smoke_0-Sheet.png"));
	SceneResource->LoadTexture("KumaShield", TEXT("Effect/AdventurerWarrior_Guard_Atlas.png"));
	SceneResource->LoadTexture("KumaShieldDashAura", TEXT("Effect/Veteran_Warrior_BodyTank_Trail_0-Sheet.png"));

	SceneResource->CreateAnimationSequence2D("KumaUltimateAura", 2.2f, "UltimateSkill_Aura_Atkas", 0, 0, 22, Vector2(400.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaUltimateComplete", .5f, "UltimateSkill_Complete_Atlas", 0, 0, 14, Vector2(1200.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthHitCharging", 2.f, "Fighter_Aura_Atlas", 0, 0, 57, Vector2(200.f, 200.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthHitInSign", 1.f, "InSign_Atlas", 0, 0, 18, Vector2(1280.f, 200.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthHitSmoke", 1.2f, "BossP2_BothFistSlam_Smoke_Atlas", 0, 0, 23, Vector2(1200.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaStampingSmoke", 2.5f, "StampingEffect", 0, 0, 42, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaRockSmoke", 1.5f, "KumaRockSmoke", 0, 0, 24, Vector2(800.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaShieldOn", 0.4f, "KumaShield", 0, 0, 8, Vector2(200.f, 200.f));
	SceneResource->CreateAnimationSequence2D("KumaShieldOff", 0.4f, "KumaShield", 0, 0, 8, Vector2(200.f, 200.f));
	SceneResource->CreateAnimationSequence2D("KumaShieldLoop", 0.1f, "KumaShield", 1, 2, 1, Vector2(200.f, 200.f));
	SceneResource->CreateAnimationSequence2D("KumaShieldDashAura", 1.f, "KumaShieldDashAura", 0, 0, 24, Vector2(400.f, 400.f));
	

	SceneResource->CreateAnimationSequence2D("FireBall", 0.5f, "Effect_Altas", 0, 9, 12, Vector2(200.f, 200.f));
	SceneResource->CreateAnimationSequence2D("Hit_Fire", 0.5f, "Effect_Altas", 0, 11, 12, Vector2(200.f, 200.f));
	SceneResource->CreateAnimationSequence2D("PortalDeactivate", 1.f, "Portal_Atlas", 2, 2, 1, Vector2(352.f, 256.f));
	SceneResource->CreateAnimationSequence2D("PortalActivate", 0.5f, "Portal_Atlas", 0, 0, 8, Vector2(352.f, 256.f));

	SceneResource->CreateAnimationSequence2D("KumaIntro", 2.3f, "Kuma_Atlas00", 0, 0, 23, Vector2(400.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaIdle", 0.6f, "Kuma_Atlas00", 0, 3, 6, Vector2(400.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaStampingUp", 1.f, "Kuma_Atlas00", 0, 4, 7, Vector2(400.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaStampingHit", 0.8f, "Kuma_Atlas01", 0, 12, 8, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaStampingJumpUp", 0.7f, "Kuma_Atlas00", 0, 6, 7, Vector2(400.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaStampingReady", 0.7f, "Kuma_Atlas00", 0, 7, 1, Vector2(400.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaWhirlWindLoop", 0.7f, "Kuma_Atlas01", 0, 0, 7, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaWhirlWindReady", 1.f, "Kuma_Atlas01", 0, 2, 1, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthQuakeAttack", 0.8f, "Kuma_Atlas01", 0, 3, 8, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthQuakeHit", 0.8f, "Kuma_Atlas01", 0, 5, 8, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthQuakeJump", 0.7f, "Kuma_Atlas01", 0, 7, 7, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthQuakeReady", 1.1f, "Kuma_Atlas01", 0, 9, 11, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaDie", 0.5f, "Kuma_Atlas01", 0, 11, 5, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaEarthQuakeJumpReady", 2.f, "Kuma_Atlas01", 0, 2, 1, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaJumpStampingJumpUp", 0.7f, "Kuma_Atlas00", 0, 6, 7, Vector2(400.f, 400.f));
	SceneResource->CreateAnimationSequence2D("KumaJumpStampingHit", 0.8f, "Kuma_Atlas01", 0, 12, 8, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaShieldReady", 0.8f, "Kuma_Atlas01", 0, 14, 4, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaTackleReady", 0.8f, "Kuma_Atlas01", 0, 15, 7, Vector2(600.f, 600.f));
	SceneResource->CreateAnimationSequence2D("KumaTackle", 0.8f, "Kuma_Atlas01", 0, 17, 4, Vector2(600.f, 600.f));


	if (SceneResource->FindAnimationSequence2D("SwitchEffect_ChiefGuard") == nullptr)
	{
		SceneResource->CreateAnimationSequence2D("SwitchEffect_ChiefGuard", .2f);
		SceneResource->SetAnimationSequence2DTexture("SwitchEffect_ChiefGuard", "Effect_Altas");
		for (int i = 0; i < 8; ++i)
		{
			m_pScene->GetResource()->AddAnimationSequence2DFrame("SwitchEffect_ChiefGuard",
				Vector2(0.f, (i + 1) * 200.f), Vector2(676.f, (i + 2) * 200.f));
		}
	}
	if (SceneResource->FindAnimationSequence2D("Skill1Effect_ChiefGuard") == nullptr)
	{
		SceneResource->CreateAnimationSequence2D("Skill1Effect_ChiefGuard", .2f);
		SceneResource->SetAnimationSequence2DTexture("Skill1Effect_ChiefGuard", "Effect_Altas");
		for (int i = 0; i < 8; ++i)
		{
			m_pScene->GetResource()->AddAnimationSequence2DFrame("Skill1Effect_ChiefGuard",
				Vector2(0.f, (i + 13) * 200.f), Vector2(622.f, (i + 14) * 200.f));
		}
	}

	SceneResource->CreateAnimationSequence2D("FireBall", 0.5f, "Effect_Altas", 0, 9, 12, Vector2(200.f, 200.f));
	SceneResource->CreateAnimationSequence2D("Hit_Fire", 0.5f, "Effect_Altas", 0, 11, 12, Vector2(200.f, 200.f));
}

void CSceneKuma::CreateParticle()
{
}

void CSceneKuma::CheckPortalMission()
{
}
