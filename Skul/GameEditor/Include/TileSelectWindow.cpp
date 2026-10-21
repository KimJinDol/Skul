#include "pch.h"
#include "TileSelectWindow.h"
#include "IMGUIImage.h"
#include "IMGUIManager.h"
#include "LogWindow.h"

CTileSelectWindow::CTileSelectWindow()
{
}

CTileSelectWindow::~CTileSelectWindow()
{
}

Vector2 CTileSelectWindow::GetCursorPos() const
{
	return m_TileImage->GetCursorPos();
}

Vector2 CTileSelectWindow::GetImageWindowPos() const
{
	return m_TileImage->GetImageWindowPos();
}

Vector2 CTileSelectWindow::GetTextureSize() const
{
	return m_TileImage->GetTextureSize();
}

void CTileSelectWindow::SetTileTexture(CTexture* Texture)
{
	m_TileImage->SetTexture(Texture);
	m_TileImage->SetSize(Texture->GetWidth(), Texture->GetHeight());
}

bool CTileSelectWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	m_TileImage = AddWidget<CIMGUIImage>("##TileImage", 500.f, 500.f);
	m_TileImage->SetStartUV(0.f, 0.f);
	m_TileImage->SetEndUV(1.f, 1.f);
	m_TileImage->SetTexture("DefaultImageSlot", TEXT("DefaultSlot.png"));

	return true;
}

void CTileSelectWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);


}
