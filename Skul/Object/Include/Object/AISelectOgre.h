#pragma once
#include "AISelect.h"

class CAISelectOgre :
    public CAISelect
{
public:
	CAISelectOgre();
	virtual ~CAISelectOgre();

public:
	virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

