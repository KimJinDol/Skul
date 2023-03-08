#include "Scene03.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "BackgroundImage.h"
#include "Object/Skul.h"
#include "Component/SpriteComponent.h"
#include "Resource/ResourceManager.h"
#include "../Object/MainMap.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"
#include "Scene/SceneManager.h"
#include "../Object/Portal.h"
#include "AIManager.h"
#include "SceneKuma.h"
#include "Object/ArcherAI.h"
#include "Object/WarriorAI.h"

CScene03::CScene03()
{
}

CScene03::~CScene03()
{
}

bool CScene03::Init()
{
	CreateAnimation();

	m_pScene->GetResource()->LoadSound("BGM", false, "Chapter1",
		"AudioClip/BGM/Chapter1.wav");
	m_SoundName = "Chapter1";
	m_pScene->GetResource()->SoundPlay(m_SoundName);


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

	m_Portal = m_pScene->SpawnObject<CPortal>("Portal");
	m_Portal->SetWorldPos2D(3550.f, 560.f);

	m_Skul = m_pScene->SpawnObject<CSkul>("Skul");
	m_Skul->SetWorldPos2D(287.f, 704.f);
	m_Skul->SetSubState(ESkulState::ChiefGuard);

	CArcherAI* Archer = CAIManager::GetInst()->SpawnObject<CArcherAI>("Archer");
	Archer->SetWorldPos2D(2082.f, 320.f);
	Archer->SetView(Object_View::Right);

	CWarriorAI* Warrior = CAIManager::GetInst()->SpawnObject<CWarriorAI>("Warrior");
	Warrior->SetWorldPos2D(1920.f, 320.f);
	Warrior->SetView(Object_View::Left);

	Warrior = CAIManager::GetInst()->SpawnObject<CWarriorAI>("Warrior");
	Warrior->SetWorldPos2D(2320.f, 320.f);
	Warrior->SetView(Object_View::Left);

	Warrior = CAIManager::GetInst()->SpawnObject<CWarriorAI>("Warrior");
	Warrior->SetWorldPos2D(2160.f, 320.f);
	Warrior->SetView(Object_View::Right);


	CMainMap* MainMap = m_pScene->SpawnObject<CMainMap>("TileMap");
	MainMap->Load(TEXT("Scene03.map"));

	CTileMapComponent* TileMap = (CTileMapComponent*)MainMap->FindSceneComponent("TileMap");


	m_pScene->GetCameraManager()->GetCurrentCamera()->Set2DWorldSize(TileMap->GetTileMapSize());

    return CSceneMode::Init();
}

void CScene03::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);


	if (m_Portal->GetEnableInteraction() && m_Skul->GetInteraction())
	{
		m_bFadeOutScene = true;
		m_bFadeOutBGM = true;
	}

	if (m_bCanChangeNextScene)
	{
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->SetSceneModeNoInit<CSceneKuma>(false);
	}
}

void CScene03::CreateAnimation()
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
}


void CScene03::CheckPortalMission()
{
	float Distance = m_Skul->GetWorldPos().Distance(m_Portal->GetWorldPos());
	if (Distance <= 600.f && CAIManager::GetInst()->GetEnemyNum() == 0)
		m_Portal->SetActivate();
}
