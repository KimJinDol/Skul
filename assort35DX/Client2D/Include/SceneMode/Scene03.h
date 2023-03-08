#pragma once
#include "Scene\SceneMode.h"
class CScene03 :
    public CSceneMode
{
    friend class CScene;

protected:
    CScene03();
    virtual ~CScene03();

private:
    //class CTrashEnemyAI* m

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    void CreateAnimation();

public:
    virtual void CheckPortalMission();
};