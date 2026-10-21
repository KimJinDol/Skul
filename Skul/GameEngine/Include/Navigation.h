#pragma once

#include "GameEngine.h"
#include "Component/TileMapComponent.h"

class CNavigation
{
	friend class CNavigationManager;

protected:
	CNavigation();
	virtual ~CNavigation() = 0;

private:
	std::vector<CSharedPtr<CTileMapComponent>>	m_NavData;

public:
	void AddNavData(CTileMapComponent* Data)
	{
		m_NavData.push_back(Data);
	}

	void DeleteTileMapData(CTileMapComponent* Data)
	{
		auto	iter = m_NavData.begin();
		auto	iterEnd = m_NavData.end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == Data)
			{
				m_NavData.erase(iter);
				break;
			}
		}
	}
};

