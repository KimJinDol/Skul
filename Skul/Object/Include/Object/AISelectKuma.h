#pragma once
#include "AISelect.h"

class CAISelectKuma :
    public CAISelect
{
public:
	CAISelectKuma();
	virtual ~CAISelectKuma();

public:
	bool m_bIntroEnableCheck;
	bool m_bIntro;
	bool m_bPhase2;
	bool m_bPhase3;

	float	m_PaternPercent;

public:
	virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

