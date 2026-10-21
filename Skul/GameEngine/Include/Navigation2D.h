#pragma once
#include "Navigation.h"
class CNavigation2D :
    public CNavigation
{
	friend class CNavigationManager;

protected:
	CNavigation2D();
	virtual ~CNavigation2D();
};

