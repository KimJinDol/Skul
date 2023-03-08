#pragma once
#include "GameObject.h"

class CBackgroundImage :
    public CGameObject
{
    friend CScene;

protected:
    CBackgroundImage();
    CBackgroundImage(const CBackgroundImage& obj);
    virtual ~CBackgroundImage();

private:
    Vector2 m_RenderPos;
    Vector2 m_Pos;
    class CSpriteComponent* m_SpriteComponent;
    float m_CameraInfluend;
    Vector3 m_PrevCameraPos;
    Vector2 m_InitUV;

public:
    void SetCameraInfluend(float Influend);
    void SetInitUV(float x, float y);

public:
    virtual bool Init();
    virtual void Start();
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender(float DeltaTime);
};

