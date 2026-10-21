
#include "Viewport.h"

CViewport::CViewport()	:
	m_Camera(nullptr)
{
}

CViewport::~CViewport()
{

}

void CViewport::AllWindowEnable(bool Enable)
{
	size_t Size = m_vecWindow.size();

	for (int i = 0; i < Size; i++)
	{
		m_vecWindow[i]->Enable(Enable);
	}
}

void CViewport::AllWindowEnableExceptParam(const std::string& Name, bool Enable)
{
	size_t Size = m_vecWindow.size();

	for (int i = 0; i < Size; i++)
	{
		if (m_vecWindow[i]->GetName() != Name)
			m_vecWindow[i]->Enable(Enable);
	}
}

void CViewport::Start()
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

bool CViewport::Init()
{
	m_Camera = new CCamera;

	m_Camera->SetName("UICamera");
	m_Camera->m_pScene = m_Scene;

	m_Camera->Init();
	m_Camera->SetCameraType(Camera_Type::CamUI);

	m_Camera->Release();

	return true;
}

void CViewport::Update(float DeltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->m_Enable) continue;

		(*iter)->Update(DeltaTime);
	}
}

void CViewport::PostUpdate(float DeltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->m_Enable) continue;

		(*iter)->PostUpdate(DeltaTime);
	}
}

void CViewport::Render()
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->m_Enable) continue;

		(*iter)->Render();
	}
}
