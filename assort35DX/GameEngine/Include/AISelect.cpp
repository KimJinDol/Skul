
#include "AISelect.h"

CAISelect::CAISelect() :
    m_DelayTimeMax(0.5f),
    m_DelayTime(0.f)
{
}

CAISelect::~CAISelect()
{
}

bool CAISelect::Init()
{
    return true;
}

//bool CAISelect::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
//{
//    m_DelayTime += DeltaTime;
//
//    if (m_DelayTime >= m_DelayTimeMax)
//    {
//        m_DelayTime -= m_DelayTimeMax;
//
//        return true;
//    }
//
//    return false;
//}
