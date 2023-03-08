#pragma once
#include "Scene\SceneMode.h"
class CSceneKuma :
    public CSceneMode
{
    friend class CScene;

protected:
    CSceneKuma();
    virtual ~CSceneKuma();

private:
    class CKumaAI* m_Kuma;

    Vector3 m_BossFightPos;
    Vector2 m_CameraFocusPos;
    bool    m_bCameraFocus;
    bool    m_bCheckBossFightStart;
    bool    m_bBossIntroCheck;
    bool    m_bDialogueCheck;
    bool    m_bFadeIn;
    bool    m_bFadeOut;

    float   m_CameraAreaRatio;
    float   m_RatioTime;
    float   m_CurRatioTime;

    class CDialogueWindow* m_pDialogueWindow;

public:
    void FocusCamera();
    void StartDialogue();
    void KumaDrinkAnimation();
    void StartFight();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    void CreateMaterial();
    void CreateAnimation();
    void CreateParticle();

public:
    virtual void CheckPortalMission();
};

