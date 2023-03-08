#pragma once

#include "GameEngine.h"

class CNavigationManager
{
private:
	class CNavigation* m_Navigation;

public:
	class CNavigation* GetNavigation()	const
	{
		return m_Navigation;
	}

public:
	bool Init();
	void CreateNavigation();

	DECLARE_SINGLE(CNavigationManager)
};

