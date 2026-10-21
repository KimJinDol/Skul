#pragma once
#include "AISelect.h"

class CAISelectWarrior :
    public CAISelect
{
public:
	CAISelectWarrior();
	virtual ~CAISelectWarrior();

private:
	bool	m_AttackDistanceRand;
	int		m_AttackDistanceMax;
	int		m_AttackDistanceMin;

public:
	virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

