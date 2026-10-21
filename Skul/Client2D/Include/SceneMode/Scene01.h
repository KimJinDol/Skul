#pragma once

#include "Scene/SceneMode.h"

class CScene01 :
    public CSceneMode
{
    friend class CScene;

protected:
    CScene01();
    virtual ~CScene01();


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

