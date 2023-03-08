
#include "AIController.h"
#include "GameObject.h"
#include "AITask.h"
#include "AISelect.h"

CAIController::CAIController() :
    m_pScene(nullptr),
    m_pCurrentAI(nullptr),
    m_pOwner(nullptr),
    m_pSelectAI(nullptr),
    m_Enable(true)
{
}

CAIController::CAIController(const CAIController& controller)
{
    *this = controller;

    m_pScene = nullptr;
    m_pOwner = nullptr;
}

CAIController::~CAIController()
{
}

void CAIController::SetEnable(bool Enable)
{
    m_Enable = Enable;
}

void CAIController::SetScene(CScene* pScene)
{
    m_pScene = pScene;
}

void CAIController::SetOwner(CAICharacter* pOwner)
{
    m_pOwner = pOwner;
}

void CAIController::ChangeAI(const std::string& Name)
{
    m_pCurrentAI = FindAI(Name);
    // m_pCurrentAI->SetComplete(false);
}

CAITask* CAIController::FindAI(const std::string& Name)
{
    auto    iter = m_mapAI.find(Name);

    if (iter == m_mapAI.end())
        return nullptr;

    return iter->second;
}

void CAIController::SetIntData(const std::string& Name, int Data)
{
    auto    iter = m_mapIntData.find(Name);

    if (iter == m_mapIntData.end())
    {
        m_mapIntData.insert(std::make_pair(Name, Data));
    }

    else
    {
        iter->second = Data;
    }
}

void CAIController::SetFloatData(const std::string& Name, float Data)
{
    auto    iter = m_mapFloatData.find(Name);

    if (iter == m_mapFloatData.end())
    {
        m_mapFloatData.insert(std::make_pair(Name, Data));
    }

    else
    {
        iter->second = Data;
    }
}

void CAIController::SetBoolData(const std::string& Name, bool Data)
{
    auto    iter = m_mapBoolData.find(Name);

    if (iter == m_mapBoolData.end())
    {
        m_mapBoolData.insert(std::make_pair(Name, Data));
    }

    else
    {
        iter->second = Data;
    }
}

void CAIController::SetStringData(const std::string& Name, std::string Data)
{
    auto    iter = m_mapStringData.find(Name);

    if (iter == m_mapStringData.end())
    {
        m_mapStringData.insert(std::make_pair(Name, Data));
    }

    else
    {
        iter->second = Data;
    }
}

void CAIController::SetGameObjectData(const std::string& Name, CGameObject* Data)
{
    auto    iter = m_mapObjectData.find(Name);

    if (iter == m_mapObjectData.end())
    {
        m_mapObjectData.insert(std::make_pair(Name, Data));
    }

    else
    {
        iter->second = Data;
    }
}

void CAIController::Run(float DeltaTime)
{
    if (!m_Enable) return;

    assert(m_pSelectAI && m_pCurrentAI);

    m_pSelectAI->Run(this, m_pOwner, m_pScene, DeltaTime);

    m_pCurrentAI->Run(this, m_pOwner, m_pScene, DeltaTime);
}

CAIController* CAIController::Clone()
{
    return new CAIController(*this);
}

int CAIController::GetIntData(const std::string& Name)
{
    auto    iter = m_mapIntData.find(Name);

    if (iter == m_mapIntData.end())
    {
        return INT_MIN;
    }

    return iter->second;
}

float CAIController::GetFloatData(const std::string& Name)
{
    auto    iter = m_mapFloatData.find(Name);

    if (iter == m_mapFloatData.end())
    {
        return FLT_MIN;
    }

    return iter->second;
}

bool CAIController::GetBoolData(const std::string& Name)
{
    auto    iter = m_mapBoolData.find(Name);

    if (iter == m_mapBoolData.end())
    {
        return false;
    }

    return iter->second;
}

std::string CAIController::GetStringData(const std::string& Name)
{
    auto    iter = m_mapStringData.find(Name);

    if (iter == m_mapStringData.end())
    {
        return std::string();
    }

    return iter->second;
}

CGameObject* CAIController::GetGameObjectData(const std::string& Name)
{
    auto    iter = m_mapObjectData.find(Name);

    if (iter == m_mapObjectData.end())
    {
        return nullptr;
    }

    return iter->second;
}
