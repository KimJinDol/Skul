#pragma once

#include "GameEngine.h"
#include "Scene/SceneManager.h"

class CAIManager
{
private:
    int m_EnemyNum;
    std::unordered_map<std::string, class CAITask*> m_mapAITask;
    std::unordered_map<std::string, class CAISelect*> m_mapAISelect;

public:
    class CAITask* FindAI(const std::string& Name);
    class CAISelect* FindAISelect(const std::string& Name);

    template <typename T>
    T* SpawnObject(const std::string& Name, const Vector3& Pos = Vector3::Zero,
        const Vector3& Rot = Vector3::Zero, const Vector3& Scale = Vector3::One)
    {
        CScene* Scene = CSceneManager::GetInst()->GetScene();
        class CAICharacter* AI = (CAICharacter*)Scene->SpawnObject<T>(Name, Pos, Rot, Scale);

        if (AI)
        {
            m_EnemyNum++;
            return (T*)AI;
        }

        return nullptr;

    }

public:
    void RemoveAllEnemy()
    {
        m_EnemyNum = 0;
    }

    void DecreaseEnemy()
    {
        m_EnemyNum--;
    }

    int GetEnemyNum()   const
    {
        return m_EnemyNum;
    }

public:
    template <typename T>
    T* CreateAI(const std::string& Name)
    {
        T* pAI = (T*)FindAI(Name);

        if (pAI)
            return pAI;

        pAI = new T;

        pAI->SetName(Name);

        m_mapAITask.insert(std::make_pair(Name, (class CAITask*)pAI));

        return pAI;
    }

    template <typename T>
    T* CreateAISelect(const std::string& Name)
    {
        T* pAISelect = (T*)FindAISelect(Name);

        if (pAISelect)
            return pAISelect;

        pAISelect = new T;

        pAISelect->SetName(Name);

        m_mapAISelect.insert(std::make_pair(Name, (class CAISelect*)pAISelect));

        return pAISelect;
    }

    DECLARE_SINGLE(CAIManager)
};

