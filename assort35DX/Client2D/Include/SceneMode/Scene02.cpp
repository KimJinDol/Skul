#include "Scene02.h"
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
#include "Scene/SceneManager.h"
#include "SceneOgre.h"
#include "Dialogue/DialogueManager.h"
#include "UI/DialogueWindow.h"
#include "Scene/Viewport.h"
#include "Animation2D\SkulAnimation.h"

CScene02::CScene02()	:
	m_bCheckWeaponDialogue(false)
{
}

CScene02::~CScene02()
{
}

bool CScene02::Init()
{
	CreateAnimation();
	CreateParticle();

	m_SoundName = "Tutorial";

	CBackgroundImage* Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundColor");
	CSpriteComponent* BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	CMaterial* BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundColorTex", TEXT("Map/Background/Background11.png"));
	BackgroundMtrl->SetTransparency(false);

	Resolution RS = m_pScene->GetCameraManager()->GetCurrentCamera()->GetResolution();
	Vector3 Scale(RS.Width * 1.3f, RS.Height * 2.f, 1.f);

	Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundTree01");
	Background->SetCameraInfluend(0.00005f);
	BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	BackgroundSprite->SetWorldScale(Scale);
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundTree01Tex", TEXT("Map/Background/Background9.png"));
	BackgroundMtrl->SetTintColor(0.2f, 0.2f, 0.2f, 0.f);

	Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundTree02");
	Background->SetCameraInfluend(0.0001f);
	BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundSprite->SetWorldScale(Scale);
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundTree02Tex", TEXT("Map/Background/Background8.png"));
	BackgroundMtrl->SetTintColor(0.2f, 0.2f, 0.2f, 0.f);

	Background = m_pScene->SpawnObject<CBackgroundImage>("BackgroundTree03");
	Background->SetCameraInfluend(0.0002f);
	Background->SetInitUV(0.f, -0.05f);
	BackgroundSprite = (CSpriteComponent*)Background->FindSceneComponent("Sprite");
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundSprite->SetWorldScale(Scale);
	BackgroundMtrl = BackgroundSprite->GetMaterial(0);
	BackgroundMtrl->AddTexture("BackgroundTree03Tex", TEXT("Map/Background/Background6.png"));
	BackgroundMtrl->SetTintColor(0.2f, 0.2f, 0.2f, 0.f);

	m_Skul = m_pScene->SpawnObject<CSkul>("Skul");
	m_Skul->SetWorldPos2D(300.f, 320.f);
	m_Skul->SetSubState(ESkulState::None);

	m_Portal = m_pScene->SpawnObject<CPortal>("Portal");
	m_Portal->SetWorldPos2D(2350.f, 300.f);

	CMainMap* MainMap = m_pScene->SpawnObject<CMainMap>("TileMap");
	MainMap->Load(TEXT("Scene02.map"));

	CTileMapComponent* TileMap = (CTileMapComponent*)MainMap->FindSceneComponent("TileMap");

	CGameObject* Bone = m_pScene->SpawnObject<CGameObject>("BoneProb");
	CSpriteComponent* Sprite = Bone->CreateSceneComponent<CSpriteComponent>("Sprite");
	Bone->SetRootComponent(Sprite);
	Sprite->SetWorldPos(790.f, 303.f, 0.f);
	Sprite->SetWorldScale(102.f, 48.f, 1.f);
	Sprite->SetRender2DType(Render_Type_2D::RT2D_Default);
	Sprite->GetMaterial(0)->AddTexture("BoneProb", TEXT("Object/Dead.png"));

	m_ChiefHead = m_pScene->SpawnObject<CGameObject>("ChiefHead");
	Sprite = m_ChiefHead->CreateSceneComponent<CSpriteComponent>("Sprite");
	m_ChiefHead->SetRootComponent(Sprite);
	Sprite->SetWorldPos(800.f, 280.f, 0.f);
	Sprite->SetWorldScale(60.f, 104.f, 1.f);
	Sprite->SetRender2DType(Render_Type_2D::RT2D_Default);
	Sprite->GetMaterial(0)->AddTexture("ChiefGuardHead", TEXT("Player/ChiefGuardMode/ChiefGuard.png"));


	m_pScene->GetCameraManager()->GetCurrentCamera()->Set2DWorldSize(TileMap->GetTileMapSize());

	m_pDialogueWindow = m_pScene->GetViewport()->AddWindow<CDialogueWindow>("DialogueWindow");
	CDialogueManager::GetInst()->SetDialogueWindow(m_pDialogueWindow);
	CDialogueManager::GetInst()->SetDialCode("# Weapon,,,,,,,,,,,,");


	return CSceneMode::Init();
}

void CScene02::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	if (GetPlayer()->GetWorldPos().x >= 776.f && !m_bCheckWeaponDialogue)
	{
		StartWeaponDialogue();
	}

	if (m_Portal->GetEnableInteraction() && m_Skul->GetInteraction() && m_bCheckWeaponDialogue)
	{
		m_bFadeOutScene = true;
	}

	if (m_bCanChangeNextScene)
	{
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->SetSceneModeNoInit<CSceneOgre>(false);
	}
}

void CScene02::CreateAnimation()
{
	CSceneResource* SceneResource = m_pScene->GetResource();
	SceneResource->LoadTexture("Effect_Altas", TEXT("Effect/Effect_Altas.png"));
	SceneResource->LoadTexture("Portal_Atlas", TEXT("Object/Portal.png"));
	SceneResource->LoadTexture("SkulAtlas", TEXT("Player/Default/DefaultMode.png"));

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
	SceneResource->CreateAnimationSequence2D("PortalDeactivate", 1.f, "Portal_Atlas", 2, 2, 1, Vector2(352.f, 256.f));
	SceneResource->CreateAnimationSequence2D("PortalActivate", 0.5f, "Portal_Atlas", 0, 0, 8, Vector2(352.f, 256.f));
	SceneResource->CreateAnimationSequence2D("SkulEquipChiefGuard", 4.f, "SkulAtlas", 7, 4, 45);

}

void CScene02::CreateParticle()
{
	//CMaterial* SnowMaterial = m_pScene->GetResource()->FindMaterial("Snow");

	//m_pScene->GetResource()->CreateParticle("TestParticle");
	//m_pScene->GetResource()->SetParticleMaterial("TestParticle", SnowMaterial);
	//m_pScene->GetResource()->SetParticleMaxParticleCount("TestParticle", 1000);
	//m_pScene->GetResource()->SetParticleStartColor("TestParticle", 0.68627f, 0.8745f, 0.89411f, 0.8f);
	//m_pScene->GetResource()->SetParticleEndColor("TestParticle", 0.68627f, 0.8745f, 0.89411f, 0.f);
	//m_pScene->GetResource()->SetParticleStartScale("TestParticle", 100.f, 100.f, 1.f);
	//m_pScene->GetResource()->SetParticleEndScale("TestParticle", 20.f, 20.f, 1.f);
	//m_pScene->GetResource()->SetParticleLifeTimeMin("TestParticle", 2.f);
	//m_pScene->GetResource()->SetParticleLifeTimeMax("TestParticle", 4.f);
	//m_pScene->GetResource()->SetParticleRange("TestParticle", 100.f, 0.f, 0.f);
	//m_pScene->GetResource()->SetParticleMinSpeed("TestParticle", 100.f);
	//m_pScene->GetResource()->SetParticleMaxSpeed("TestParticle", 200.f);
	//m_pScene->GetResource()->SetParticleMoveEnable("TestParticle", true);
	//m_pScene->GetResource()->SetParticle2D("TestParticle", true);
	//m_pScene->GetResource()->SetParticleMoveDir("TestParticle", 0.f, 1.f, 0.f);
	//m_pScene->GetResource()->SetParticleMoveAngle("TestParticle", 0.f, 0.f, 90.f);

	//CMaterial* FlameMaterial = m_pScene->GetResource()->FindMaterial("Flame");

	//m_pScene->GetResource()->CreateParticle("FlameParticle");
	//m_pScene->GetResource()->SetParticleMaterial("FlameParticle", FlameMaterial);
	//m_pScene->GetResource()->SetParticleMaxParticleCount("FlameParticle", 500);
	//m_pScene->GetResource()->SetParticleStartColor("FlameParticle", 0.94901f, 0.490019f, 0.04705f, 0.8f);
	//m_pScene->GetResource()->SetParticleEndColor("FlameParticle", 0.94901f, 0.490019f, 0.04705f, 0.f);
	//m_pScene->GetResource()->SetParticleStartScale("FlameParticle", 20.f, 20.f, 1.f);
	//m_pScene->GetResource()->SetParticleEndScale("FlameParticle", 10.f, 10.f, 1.f);
	////m_pScene->GetResource()->SetParticleLifeTimeMin("FlameParticle", 0.5f);
	////m_pScene->GetResource()->SetParticleLifeTimeMax("FlameParticle", 0.8f);
	//m_pScene->GetResource()->SetParticleLifeTimeMin("FlameParticle", 10.5f);
	//m_pScene->GetResource()->SetParticleLifeTimeMax("FlameParticle", 10.8f);
	//m_pScene->GetResource()->SetParticleRange("FlameParticle", 0.f, 0.f, 0.f);
	//m_pScene->GetResource()->SetParticleMinSpeed("FlameParticle", 600.f);
	//m_pScene->GetResource()->SetParticleMaxSpeed("FlameParticle", 800.f);
	//m_pScene->GetResource()->SetParticleMoveEnable("FlameParticle", true);
	//m_pScene->GetResource()->SetParticle2D("FlameParticle", true);
	//m_pScene->GetResource()->SetParticleMoveDir("FlameParticle", 0.f, 1.f, 0.f);
	//m_pScene->GetResource()->SetParticleMoveAngle("FlameParticle", 0.f, 0.f, 90.f);

}

void CScene02::StartChiefEquipAnimation()
{
	CSpriteComponent* Sprite = (CSpriteComponent*)m_Skul->FindSceneComponent("Sprite");
	CSkulAnimation* Anim = (CSkulAnimation*)Sprite->GetAnimation2D();
	m_Skul->SetState(ESkulState::ChiefGuard);
	m_Skul->SetSubState(ESkulState::Default);
	Anim->AddAnimationSequence2D("SkulEquipChiefGuard", false);
	Anim->SetFrameEndFunction<CScene02>("SkulEquipChiefGuard", this, &CScene02::EndChiefEquipAnimation);
	Anim->ChangeAnimation("SkulEquipChiefGuard");
	Anim->SetEventMode(true);

	m_ChiefHead->Active(false);
}


void CScene02::EndChiefEquipAnimation()
{
	CSpriteComponent* Sprite = (CSpriteComponent*)m_Skul->FindSceneComponent("Sprite");
	CSkulAnimation* Anim = (CSkulAnimation*)Sprite->GetAnimation2D();
	Anim->SetEventMode(false);
	m_Skul->SetEnable();

	CScene* Scene = CSceneManager::GetInst()->GetScene();
	CViewport* Viewport = Scene->GetViewport();
	Viewport->AllWindowEnableExceptParam("DialogueWindow", true);
}

void CScene02::StartWeaponDialogue()
{
	m_bCheckWeaponDialogue = true;
	m_Skul->SetEnable(false, false, false);
	CDialogueManager::GetInst()->StartDialogue();
	CDialogueManager::GetInst()->SetEndDialogueFunc<CScene02>(this, &CScene02::StartChiefEquipAnimation);
	m_pDialogueWindow->SetNextDialogue(0.f);
}

void CScene02::CheckPortalMission()
{
	float Distance = m_Skul->GetWorldPos().Distance(m_Portal->GetWorldPos());
	if (Distance <= 600.f)
		m_Portal->SetActivate();
}
