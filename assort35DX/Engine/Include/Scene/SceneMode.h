#pragma once

#include "../GameEngine.h"

class CSceneMode
{
    friend class CScene;

protected:
    CSceneMode();
    virtual ~CSceneMode();

protected:
    class CScene* m_pScene;
    class CPortal* m_Portal;
    class CSkul* m_Skul;
    SceneType	m_NextSceneType;

    bool    m_bFadeOutBGM;
    bool    m_bCanChangeNextScene;
    bool    m_bFadeOutScene;
    bool    m_bFadeInScene;
    float   m_CurFadeOutTime;
    float   m_FadeOutTime;
    float   m_CurFadeInTime;
    float   m_FadeInTime;

    class CCinemaWidget* m_CinemaWidget;

    std::string m_SoundName;

public:
    class CCinemaWidget* GetCinemaWidget()  const
    {
        return m_CinemaWidget;
    }

    class CCharacter* GetPlayer()    const
    {
        return (CCharacter*)m_Skul;
    }

public:
    void SetNextSceneType(SceneType Type)
    {
        m_NextSceneType = Type;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);

    virtual void CheckPortalMission();
};

