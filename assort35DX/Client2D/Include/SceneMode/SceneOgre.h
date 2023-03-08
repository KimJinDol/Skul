#pragma once
#include "Scene\SceneMode.h"

class CSceneOgre :
    public CSceneMode
{
    friend class CScene;

protected:
    CSceneOgre();
    virtual ~CSceneOgre();

private:
    class COgreAI* m_pOgre;

    bool    m_bFocusOgre;
    bool    m_bDeadOgre;


public:
    void ActiveSkulWidget();
    void DeadOgre()
    {
        m_bDeadOgre = true;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    void CreateMaterial();
    void CreateAnimation();
    void CreateParticle();

    void FocusOgre();

public:
    virtual void CheckPortalMission();
};

