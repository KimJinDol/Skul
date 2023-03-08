#pragma once
#include "AISelect.h"
class CAISelectArcher :
    public CAISelect
{
public:
	CAISelectArcher();
	virtual ~CAISelectArcher();

private:
	bool	m_AttackDistanceRand;
	int		m_AttackDistanceMax;
	int		m_AttackDistanceMin;

public:
	virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

