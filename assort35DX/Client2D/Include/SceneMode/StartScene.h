#pragma once

#include "Scene\SceneMode.h"

class CStartScene :
    public CSceneMode
{
    friend class CScene;

protected:
    CStartScene();
    virtual ~CStartScene();

private:
    class CWidgetWindow* m_Widget;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

