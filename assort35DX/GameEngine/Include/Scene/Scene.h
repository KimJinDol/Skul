#pragma once

#include "../GameEngine.h"
#include "../GameObject.h"
#include "SceneMode.h"
#include "../SceneObjectWindow.h"

class CScene
{
    friend class CSceneManager;

private:
    CScene();
    ~CScene();

private:
    class CSceneMode* m_pSceneMode;
    class CSceneResource* m_pSceneResource;
    class CCameraManager* m_pCameraManager;
    class CSceneCollision* m_pCollision;
    class CViewport* m_pViewport;
//#if _DEBUG
    class CSceneObjectWindow* m_pObjectWindow;
    class CObjectDetailWindow* m_pDetailWindow;
//#endif
private:
    bool m_StartScene;

public:
    bool IsStart()  const
    {
        return m_StartScene;
    }

public:
    template <typename T>
    T* GetSceneMode()    const
    {
        return (T*)m_pSceneMode;
    }

    CSceneMode* GetSceneMode()  const
    {
        return m_pSceneMode;
    }

    class CSceneResource* GetResource() const
    {
        return m_pSceneResource;
    }

    class CCameraManager* GetCameraManager()    const
    {
        return m_pCameraManager;
    }

    class CSceneCollision* GetCollisionManager()    const
    {
        return m_pCollision;
    }

    class CViewport* GetViewport()  const
    {
        return m_pViewport;
    }

private:
    std::list<CSharedPtr<CGameObject>>  m_ObjList;

public:
    std::list<CSharedPtr<CGameObject>> GetAllObjects() const
    {
        return m_ObjList;
    }
    CGameObject* FindObject(const std::string& Name);

public:
    void Start();
    bool Init();
    void Update(float DeltaTime);
    void PostUpdate(float DeltaTime);
    void Collision(float DeltaTime);
    void Render(float DeltaTime);
    void Clear();

private:
    const std::string CheckObjectNameReduplication(const std::string& Name, int Numbering = 0) const;

public:
    template <typename T>
    T* SpawnObject(const std::string& Name, const Vector3& Pos = Vector3::Zero, 
        const Vector3& Rot = Vector3::Zero, const Vector3& Scale = Vector3::One)
    {
        T* pObj = new T;

        pObj->SetName(CheckObjectNameReduplication(Name));
        pObj->m_pScene = this;
        pObj->SetWorldPos(Pos);
        pObj->SetWorldRotation(Rot);
        pObj->SetWorldScale(Scale);

        if (!pObj->Init())
        {
            SAFE_RELEASE(pObj);
            return nullptr;
        }
 /*       pObj->SetWorldPos(Pos);
        pObj->SetWorldRotation(Rot);
        pObj->SetWorldScale(Scale);*/

        m_ObjList.push_back(pObj);
        pObj->Release();

#if _DEBUG
 /*       if (m_pObjectWindow)
            m_pObjectWindow->AddItem(pObj->GetName().c_str());*/
#endif
        return pObj;
    }

    template <typename T>
    bool SetSceneMode()
    {
        SAFE_DELETE(m_pSceneMode);

        m_pSceneMode = new T;

        m_pSceneMode->m_pScene = this;

        if (!m_pSceneMode->Init())
            return false;

        return true;
    }

    template <typename T>
    bool SetSceneModeNoInit()
    {
        SAFE_DELETE(m_pSceneMode);
        
        m_pSceneMode = new T;

        m_pSceneMode->m_pScene = this;

        return true;
    }

    template <typename T>
    T* CreateWidgetWindow(const std::string& Name)
    {
        T* Window = new T;

        Window->m_Viewport = m_pViewport;

        if (!Window->Init())
        {
            SAFE_DELETE(Window);
            return nullptr;
        }

        return Window;
    }
};

