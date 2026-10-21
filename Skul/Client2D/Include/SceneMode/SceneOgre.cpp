#include "SceneOgre.h"
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
#include "UI\BossWidget.h"
#include "Scene/SceneManager.h"
#include "Component/HPComponent.h"
#include "Scene03.h"


CSceneOgre::CSceneOgre()
{
	m_bFocusOgre = false;
	m_bDeadOgre = false;
}

CSceneOgre::~CSceneOgre()
{
}

void CSceneOgre::ActiveSkulWidget()
{
	m_Skul->GetWidget()->Enable(true);
}

bool CSceneOgre::Init()
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

	CGameObject* Gate = m_pScene->SpawnObject<CGameObject>("CastleGate");
	CSpriteComponent* Sprite = Gate->CreateSceneComponent<CSpriteComponent>("Sprite");
	Gate->SetRootComponent(Sprite);
	Sprite->SetRender2DType(Render_Type_2D::RT2D_Background);
	Sprite->GetMaterial(0)->AddTexture("CastleGate", TEXT("Map/Prop/CastleGate.png"));
	Sprite->SetWorldScale(332.f, 628.f, 1.f);
	Sprite->SetWorldPos2D(2390.f, 285.f);

	m_Skul = m_pScene->SpawnObject<CSkul>("Skul");
	m_Skul->SetWorldPos2D(300.f, 320.f);
	m_Skul->SetSubState(ESkulState::ChiefGuard);

	/*m_Portal = m_pScene->SpawnObject<CPortal>("Portal");
	m_Portal->SetWorldPos2D(2720.f, 500.f);*/

	CMainMap* MainMap = m_pScene->SpawnObject<CMainMap>("TileMap");
	MainMap->Load(TEXT("Scene02.map"));

	CTileMapComponent* TileMap = (CTileMapComponent*)MainMap->FindSceneComponent("TileMap");

	m_pScene->GetCameraManager()->GetCurrentCamera()->Set2DWorldSize(TileMap->GetTileMapSize());

	m_pOgre = m_pScene->SpawnObject<COgreAI>("Ogre");
	m_pOgre->SetWorldPos(2500.f, 320.f, 0.f);
	m_pOgre->SetView(Object_View::Left);
	m_pOgre->SetEnableController(false);	


	return CSceneMode::Init();
}

void CSceneOgre::Update(float DeltaTime)
{
    CSceneMode::Update(DeltaTime);

	if (GetPlayer()->GetWorldPos().x >= 700.f && !m_bFocusOgre)
	{
		FocusOgre();
	}

	if (m_pOgre->GetHPComponent()->IsDie())
	{
		m_bFadeOutScene = true;
		m_bFadeOutBGM = true;
	}

	if (m_bCanChangeNextScene)
	{
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->SetSceneModeNoInit<CScene03>(false);
	}

}

void CSceneOgre::CreateMaterial()
{
}

void CSceneOgre::CreateAnimation()
{
	CSceneResource* SceneResource = m_pScene->GetResource();
	SceneResource->LoadTexture("Effect_Altas", TEXT("Effect/Effect_Altas.png"));
	SceneResource->LoadTexture("Portal_Atlas", TEXT("Object/Portal.png"));
	SceneResource->LoadTexture("Ogre_Atlas", TEXT("Enemy/Ogre/Ogre_Atlas.png"));

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

	SceneResource->CreateAnimationSequence2D("OgreIdle", 0.9f);
	SceneResource->SetAnimationSequence2DTexture("OgreIdle", "Ogre_Atlas");
	SceneResource->AddAnimationSequence2DFrame("OgreIdle", 0, 0, 9, Vector2(400.f, 400.f));

	SceneResource->CreateAnimationSequence2D("OgreRun", 0.7f);
	SceneResource->SetAnimationSequence2DTexture("OgreRun", "Ogre_Atlas");
	SceneResource->AddAnimationSequence2DFrame("OgreRun", 0, 1, 7, Vector2(400.f, 400.f));

	SceneResource->CreateAnimationSequence2D("OgreAttack", 3.5f);
	SceneResource->SetAnimationSequence2DTexture("OgreAttack", "Ogre_Atlas");
	SceneResource->AddAnimationSequence2DFrame("OgreAttack", 0, 2, 28, Vector2(400.f, 400.f));

	SceneResource->CreateAnimationSequence2D("OgreDie", 2.f);
	SceneResource->SetAnimationSequence2DTexture("OgreDie", "Ogre_Atlas");
	SceneResource->AddAnimationSequence2DFrame("OgreDie", 0, 6, 14, Vector2(400.f, 400.f));

}

void CSceneOgre::CreateParticle()
{
}

void CSceneOgre::FocusOgre()
{
	CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();
	Camera->StartCameraFocus(Vector2(1400.f, 120.f), 2.f, 4.f);
	Camera->SetFocusEndFunction<CSceneOgre>(this, &CSceneOgre::ActiveSkulWidget);
	m_Skul->GetWidget()->Enable(false);
	CBossWidget* Widget = m_pOgre->GetWidget();
	Widget->OnBossInfoWidget(true, 1.f, 3.f);
	
	m_bFocusOgre = true;
}

void CSceneOgre::CheckPortalMission()
{

}
