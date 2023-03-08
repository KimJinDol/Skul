#pragma once
#include "Texture.h"

// ���� Ÿ�� : ���� Ÿ���� ǥ�� �� ���� �������� �׷����� ��ǻ�� �޸� ������ ���Ѵ�.
// ����ۿ� �ٷ� �׸��� �ʰ� ���� Ÿ���� �̿��ϴ� ������ ����Ʈ ���μ����� �����ε�, ���� �ΰ��� ȭ�鿡 ���� ��ó��(�� ��) �۾��� ���� UI�� ������� �ʰ� �� ���� �ִ�.
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

