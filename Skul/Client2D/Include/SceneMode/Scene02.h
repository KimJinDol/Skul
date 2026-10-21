#pragma once
#include "Scene\SceneMode.h"

class CScene02 :
    public CSceneMode
{
    friend class CScene;

protected:
    CScene02();
    virtual ~CScene02();

private:
    class CDialogueWindow* m_pDialogueWindow;

    CGameObject* m_ChiefHead;

    bool m_bCheckWeaponDialogue;

private:
    void StartChiefEquipAnimation();
    void EndChiefEquipAnimation();
    void StartWeaponDialogue();

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

