
#include "SceneMode.h"
#include "../Engine.h"
#include "../Resource/ResourceManager.h"
#include "Scene.h"
#include "SceneResource.h"

CSceneMode::CSceneMode()  :
    m_Portal(nullptr),
    m_Skul(nullptr),
    m_bFadeOutScene(false),
    m_bFadeInScene(true),
    m_CurFadeOutTime(0.f),
    m_CurFadeInTime(0.f),
    m_FadeOutTime(2.f),
    m_FadeInTime(2.f),
    m_bCanChangeNextScene(false),
    m_bFadeOutBGM(false)
{
}

CSceneMode::~CSceneMode()
{
}

bool CSceneMode::Init()
{
    CResourceManager::GetInst()->SetVolume("BGM", 100.f);
    return true;
}

void CSceneMode::Update(float DeltaTime)
{
    if (!m_Skul) return;

    if (m_Portal)
        CheckPortalMission();

    if (m_bFadeOutScene)
    {
        m_CurFadeOutTime += DeltaTime;
        CEngine::GetInst()->SetContrast(1.f - (m_CurFadeOutTime / m_FadeOutTime));

        if (m_bFadeOutBGM)
            CResourceManager::GetInst()->SetVolume("BGM", (1.f - (m_CurFadeOutTime / m_FadeOutTime)) * 100.f);

        if (m_CurFadeOutTime >= m_FadeOutTime)
        {
            m_bFadeOutScene = false;
            m_bCanChangeNextScene = true;

            if (m_bFadeOutBGM)
                m_pScene->GetResource()->SoundStop(m_SoundName);
        }
    }

    if (m_bFadeInScene)
    {
        m_CurFadeInTime += DeltaTime;
        CEngine::GetInst()->SetContrast(m_CurFadeInTime / m_FadeInTime);

        if (m_CurFadeInTime >= m_FadeInTime)
            m_bFadeInScene = false;
    }
}

void CSceneMode::PostUpdate(float DeltaTime)
{
}

void CSceneMode::CheckPortalMission()
{
}
