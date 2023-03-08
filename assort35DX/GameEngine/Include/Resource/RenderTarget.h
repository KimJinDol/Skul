#pragma once
#include "Texture.h"

// 렌더 타겟 : 렌더 타겟은 표시 될 다음 프레임이 그려지는 컴퓨터 메모리 영역을 말한다.
// 백버퍼에 바로 그리지 않고 렌더 타겟을 이용하는 이유는 포스트 프로세싱을 위함인데, 만약 인게임 화면에 여러 후처리(블러 등) 작업이 들어가고 UI에 적용되지 않게 할 수도 있다.
class CRenderTarget :
	public CTexture
{
	friend class CResourceManager;

protected:
	CRenderTarget();
	~CRenderTarget();

protected:
	std::string m_Name;
	ID3D11RenderTargetView* m_TargetView;
	ID3D11Texture2D* m_TargetTex;
	ID3D11RenderTargetView* m_PrevTargetView;
	ID3D11DepthStencilView* m_PrevDepthView;
	IDXGISurface* m_Surface;
	float   m_ClearColor[4];
	bool	m_DebugRender;
	Matrix	m_matDebugWVP;

public:
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	IDXGISurface* GetSurface()	const
	{
		return m_Surface;
	}

	ID3D11RenderTargetView* GetTargetView()	const
	{
		return m_TargetView;
	}

	void SetDebugRender(bool Render)
	{
		m_DebugRender = Render;
	}

	void SetClearColor(float r, float g, float b, float a)
	{
		m_ClearColor[0] = r;
		m_ClearColor[1] = g;
		m_ClearColor[2] = b;
		m_ClearColor[3] = a;
	}

public:
	bool CreateTarget(const std::string& Name, unsigned int Width,
		unsigned int Height, DXGI_FORMAT PixelFormat);
	void ClearTarget();
	void SetTarget(ID3D11DepthStencilView* DepthView);
	void ResetTarget();
};

