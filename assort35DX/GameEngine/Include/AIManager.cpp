
#include "AIManager.h"
#include "AITask.h"
#include "AISelect.h"

DEFINITION_SINGLE(CAIManager)

CAIManager::CAIManager()    :
    m_EnemyNum(0)
{
}

CAIManager::~CAIManager()
{
    auto    iter = m_mapAITask.begin();
    auto    iterEnd = m_mapAITask.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }

    auto    iter1 = m_mapAISelect.begin();
    auto    iter1End = m_mapAISelect.end();

    for (; iter1 != iter1End; ++iter1)
    {
        SAFE_DELETE(iter1->second);
    }
}

CAITask* CAIManager::FindAI(const std::string& Name)
{
    auto    iter = m_mapAITask.find(Name);

    if (iter == m_mapAITask.end())
        return nullptr;

    return iter->second;
}

CAISelect* CAIManager::FindAISelect(const std::string& Name)
{
    auto    iter = m_mapAISelect.find(Name);

    if (iter == m_mapAISelect.end())
        return nullptr;

    return iter->second;
}
