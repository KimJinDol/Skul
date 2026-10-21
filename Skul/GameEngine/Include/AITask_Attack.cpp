#include "AITask_Attack.h"
#include "AICharacter.h"

CAITask_Attack::CAITask_Attack()
{
}

CAITask_Attack::~CAITask_Attack()
{
}

void CAITask_Attack::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    pOwner->Attack();
}
