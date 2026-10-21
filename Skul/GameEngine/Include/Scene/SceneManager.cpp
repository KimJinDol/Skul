
#include "SceneManager.h"
#include "../AIManager.h"


DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()  :
    m_pNextScene(nullptr),
    m_pScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
    SAFE_DELETE(m_pNextScene);
    SAFE_DELETE(m_pScene);
}

bool CSceneManager::Init()
{
    m_pScene = new CScene;

    m_pScene->Init();

    //m_pScene->Start();

    return true;
}

bool CSceneManager::Update(float DeltaTime)
{
    if (!m_pScene->IsStart())
        m_pScene->Start();

    CInput* Input = CInput::GetInst();
    m_pScene->Update(DeltaTime);

    return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
    m_pScene->PostUpdate(DeltaTime);

    return ChangeScene();
}

bool CSceneManager::Collision(float DeltaTime)
{
    m_pScene->Collision(DeltaTime);

    return ChangeScene();
}

bool CSceneManager::Render(float DeltaTime)
{
    m_pScene->Render(DeltaTime);

    return ChangeScene();
}

void CSceneManager::Clear()
{
    m_pScene->Clear();
}

void CSceneManager::CreateNextScene()
{
    CInput::GetInst()->ClearCallback();

    SAFE_DELETE(m_pNextScene);

    m_pNextScene = new CScene;
    
    m_pNextScene->Init();
}

CScene* CSceneManager::CreateScene()
{
    CScene* Scene = new CScene;

    Scene->Init();

    return Scene;
}

void CSceneManager::SetNextScene(CScene* Scene)
{
    SAFE_DELETE(m_pNextScene);

    m_pNextScene = Scene;
}

bool CSceneManager::ChangeScene()
{
    if (m_pNextScene)
    {
        SAFE_DELETE(m_pScene);

        CAIManager::GetInst()->RemoveAllEnemy();

        m_pScene = m_pNextScene;
        m_pScene->m_pSceneMode->Init();
        
        m_pNextScene = nullptr;

        return true;
    }

    return false;
}