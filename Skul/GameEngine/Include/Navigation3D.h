#pragma once
#include "Navigation.h"
class CNavigation3D :
    public CNavigation
{
	friend class CNavigationManager;

protected:
	CNavigation3D();
	virtual ~CNavigation3D();
};

