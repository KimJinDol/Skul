#include "NavigationManager.h"
#include "Navigation2D.h"
#include "Navigation3D.h"
#include "Render/RenderManager.h"

DEFINITION_SINGLE(CNavigationManager)

CNavigationManager::CNavigationManager()	:
	m_Navigation(nullptr)
{
}

CNavigationManager::~CNavigationManager()
{
	SAFE_DELETE(m_Navigation);
}

bool CNavigationManager::Init()
{
	return true;
}

void CNavigationManager::CreateNavigation()
{
	SAFE_DELETE(m_Navigation);

	Render_Space	Space = CRenderManager::GetInst()->GetRenderSpace();

	switch (Space)
	{
	case Render_Space::Render2D:
		m_Navigation = new CNavigation2D;
		break;
	case Render_Space::Render3D:
		m_Navigation = new CNavigation3D;
		break;
	}
}
