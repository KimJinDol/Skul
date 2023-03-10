
#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Scene/SceneResource.h"
#include "../UI/StartHUDWidget.h"
#include "Engine.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	m_pScene->GetResource()->LoadSound("UI", false, "ButtonMouseOn",
		"TeemoSmile.mp3");
	m_pScene->GetResource()->LoadSound("UI", false, "ButtonClick",
		"TeemoStartClicck.mp3");

	CStartHUDWidget* Widget = m_pScene->GetViewport()->AddWindow<CStartHUDWidget>("StartHUD");

	//CEngine::GetInst()->OnDebugLog();
	//CEngine::GetInst()->AddDebugLog("Test Debug Log");

	return true;
}

void CStartScene::Update(float DeltaTime)
{
}
