#include "Scene01.h"
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
#include "Scene02.h"
#include "Input.h"
#include "Object/ArcherAI.h"

CScene01::CScene01()
{
}

CScene01::~CScene01()
{
}

bool CScene01::Init()
{
	CreateAnimation();
	CreateParticle();

	m_pScene->GetResource()->LoadSound("BGM", false, "Tutorial",
		"AudioClip/BGM/Tutorial.wav");
	m_SoundName = "Tutorial";
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

    m_Skul = m_pScene->SpawnObject<CSkul>("Skul");
	m_Skul->SetWorldPos2D(300.f, 512.f);
	m_Skul->SetSubState(ESkulState::None);

	m_Portal = m_pScene->SpawnObject<CPortal>("Portal");
	m_Portal->SetWorldPos2D(2720.f, 500.f);
	
	CMainMap* MainMap = m_pScene->SpawnObject<CMainMap>("TileMap");
	MainMap->Load(TEXT("Scene01.map"));

	CTileMapComponent* TileMap = (CTileMapComponent*)MainMap->FindSceneComponent("TileMap");

	m_pScene->GetCameraManager()->GetCurrentCamera()->Set2DWorldSize(TileMap->GetTileMapSize());

	return CSceneMode::Init();
}

void CScene01::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	if (m_Portal->GetEnableInteraction() && m_Skul->GetInteraction())
	{
		m_bFadeOutScene = true;
	}

	if(m_bCanChangeNextScene)
	{
		//CSceneManager::GetInst()->CreateChangeNextScene<CScene02>();
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->SetSceneModeNoInit<CScene02>(false);
	}
}

void CScene01::CreateAnimation()
{
	CSceneResource* SceneResource = m_pScene->GetResource();
	SceneResource->LoadTexture("Portal_Atlas", TEXT("Object/Portal.png"));
	

	//SceneResource->CreateAnimationSequence2D("FireBall", 0.5f);
	//SceneResource->SetAnimationSequence2DTexture("FireBall", "Effect_Altas");
	//SceneResource->AddAnimationSequence2DFrame("FireBall", 0, 9, 12, Vector2(200.f, 200.f));

	//SceneResource->CreateAnimationSequence2D("Hit_Fire", 0.5f);
	//SceneResource->SetAnimationSequence2DTexture("Hit_Fire", "Effect_Altas");
	//SceneResource->AddAnimationSequence2DFrame("Hit_Fire", 0, 11, 12, Vector2(200.f, 200.f));

	SceneResource->CreateAnimationSequence2D("PortalDeactivate", 1.f);
	SceneResource->SetAnimationSequence2DTexture("PortalDeactivate", "Portal_Atlas");
	SceneResource->AddAnimationSequence2DFrame("PortalDeactivate", 2, 2, 1, Vector2(352.f, 256.f));

	SceneResource->CreateAnimationSequence2D("PortalActivate", 0.5f);
	SceneResource->SetAnimationSequence2DTexture("PortalActivate", "Portal_Atlas");
	SceneResource->AddAnimationSequence2DFrame("PortalActivate", 0, 0, 8, Vector2(352.f, 256.f));

}

void CScene01::CreateParticle()
{
	CMaterial* SnowMaterial = m_pScene->GetResource()->FindMaterial("Snow");

	m_pScene->GetResource()->CreateParticle("TestParticle");
	m_pScene->GetResource()->SetParticleMaterial("TestParticle", SnowMaterial);
	m_pScene->GetResource()->SetParticleMaxParticleCount("TestParticle", 1000);
	m_pScene->GetResource()->SetParticleStartColor("TestParticle", 0.68627f, 0.8745f, 0.89411f, 0.8f);
	m_pScene->GetResource()->SetParticleEndColor("TestParticle", 0.68627f, 0.8745f, 0.89411f, 0.f);
	m_pScene->GetResource()->SetParticleStartScale("TestParticle", 100.f, 100.f, 1.f);
	m_pScene->GetResource()->SetParticleEndScale("TestParticle", 20.f, 20.f, 1.f);
	m_pScene->GetResource()->SetParticleLifeTimeMin("TestParticle", 2.f);
	m_pScene->GetResource()->SetParticleLifeTimeMax("TestParticle", 4.f);
	m_pScene->GetResource()->SetParticleRange("TestParticle", 100.f, 0.f, 0.f);
	m_pScene->GetResource()->SetParticleMinSpeed("TestParticle", 100.f);
	m_pScene->GetResource()->SetParticleMaxSpeed("TestParticle", 200.f);
	m_pScene->GetResource()->SetParticleMoveEnable("TestParticle", true);
	m_pScene->GetResource()->SetParticle2D("TestParticle", true);
	m_pScene->GetResource()->SetParticleMoveDir("TestParticle", 0.f, 1.f, 0.f);
	m_pScene->GetResource()->SetParticleMoveAngle("TestParticle", 0.f, 0.f, 90.f);

	CMaterial* FlameMaterial = m_pScene->GetResource()->FindMaterial("Flame");

	m_pScene->GetResource()->CreateParticle("FlameParticle");
	m_pScene->GetResource()->SetParticleMaterial("FlameParticle", FlameMaterial);
	m_pScene->GetResource()->SetParticleMaxParticleCount("FlameParticle", 500);
	m_pScene->GetResource()->SetParticleStartColor("FlameParticle", 0.94901f, 0.490019f, 0.04705f, 0.8f);
	m_pScene->GetResource()->SetParticleEndColor("FlameParticle", 0.94901f, 0.490019f, 0.04705f, 0.f);
	m_pScene->GetResource()->SetParticleStartScale("FlameParticle", 20.f, 20.f, 1.f);
	m_pScene->GetResource()->SetParticleEndScale("FlameParticle", 10.f, 10.f, 1.f);
	//m_pScene->GetResource()->SetParticleLifeTimeMin("FlameParticle", 0.5f);
	//m_pScene->GetResource()->SetParticleLifeTimeMax("FlameParticle", 0.8f);
	m_pScene->GetResource()->SetParticleLifeTimeMin("FlameParticle", 10.5f);
	m_pScene->GetResource()->SetParticleLifeTimeMax("FlameParticle", 10.8f);
	m_pScene->GetResource()->SetParticleRange("FlameParticle", 0.f, 0.f, 0.f);
	m_pScene->GetResource()->SetParticleMinSpeed("FlameParticle", 600.f);
	m_pScene->GetResource()->SetParticleMaxSpeed("FlameParticle", 800.f);
	m_pScene->GetResource()->SetParticleMoveEnable("FlameParticle", true);
	m_pScene->GetResource()->SetParticle2D("FlameParticle", true);
	m_pScene->GetResource()->SetParticleMoveDir("FlameParticle", 0.f, 1.f, 0.f);
	m_pScene->GetResource()->SetParticleMoveAngle("FlameParticle", 0.f, 0.f, 90.f);

}

void CScene01::CheckPortalMission()
{
	float Distance = m_Skul->GetWorldPos().Distance(m_Portal->GetWorldPos());
	if (Distance <= 600.f)
	{
		m_Portal->SetActivate();
	}
}
