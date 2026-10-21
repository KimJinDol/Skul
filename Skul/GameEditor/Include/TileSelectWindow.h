#pragma once
#include "IMGUIWindow.h"
#include "Resource/Texture.h"

class CTileSelectWindow :
    public CIMGUIWindow
{
	friend class CTileMapWindow;
	friend class CIMGUIManager;

private:
	CTileSelectWindow();
	virtual ~CTileSelectWindow();

private:
	class CIMGUIImage* m_TileImage;

public:
	Vector2 GetCursorPos()	const;
	Vector2 GetImageWindowPos() const;
	Vector2 GetTextureSize()	const;

public:
	void SetTileTexture(CTexture* Texture);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

