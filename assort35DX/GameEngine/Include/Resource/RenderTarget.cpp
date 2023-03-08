
#include "RenderTarget.h"
#include "../Device.h"

CRenderTarget::CRenderTarget()	:
	m_TargetView(nullptr),
	m_TargetTex(nullptr),
	m_PrevTargetView(nullptr),
	m_PrevDepthView(nullptr),
	m_Surface(nullptr),
	m_ClearColor{},
	m_DebugRender(false),
	m_Name{}
{
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(m_Surface);
	SAFE_RELEASE(m_PrevDepthView);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_TargetTex);
}

bool CRenderTarget::CreateTarget(const std::string& Name, 
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat)
{
	m_Name = Name;
	// Target Texture ����
	D3D11_TEXTURE2D_DESC	Desc = {};

	Desc.Width = Width;
	Desc.Height = Height;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 2;
	Desc.SampleDesc.Quality = 0;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = PixelFormat;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(DEVICE->CreateTexture2D(&Desc, nullptr, &m_TargetTex)))
		return false;

	TextureResourceInfo* Info = new TextureResourceInfo;

	Info->Width = Width;
	Info->Height = Height;	

	m_vecResourceInfo.push_back(Info);

	m_TargetTex->QueryInterface(__uuidof(IDXGISurface), (void**)&m_Surface);

	// TargetTexture�� ����� TargetView�� �����, �� ���� Ÿ�� �ؽ�ó ������ ���̴������� ����� ���̱� ������ �ڿ� ����� ������ ShaderResourceView�� ������ش�.
	if (FAILED(DEVICE->CreateShaderResourceView(m_TargetTex, nullptr, &Info->SRV)))
		return false;

	if (FAILED(DEVICE->CreateRenderTargetView(m_TargetTex, nullptr, &m_TargetView)))
		return false;

	return true;
}

void CRenderTarget::ClearTarget()
{
	CONTEXT->ClearRenderTargetView(m_TargetView, m_ClearColor);
}

void CRenderTarget::SetTarget(ID3D11DepthStencilView* DepthView)
{
	// Get�� �ϸ� ���������� ���۷��� ī��Ʈ�� �����ϱ� ������ �پ��� Release�� ������Ѵ�.
	// ���� �츮�� ���۹��� Ÿ�ٺ並 ����ϰ� �־����Ƿ� m_PrevTargetView���� ������� Ÿ�ٺ䰡 ���� ����.
	CONTEXT->OMGetRenderTargets(1, &m_PrevTargetView, &m_PrevDepthView);

	ID3D11DepthStencilView* Depth = m_PrevDepthView;

	if (DepthView)
		Depth = DepthView;

	CONTEXT->OMSetRenderTargets(1, &m_TargetView, Depth);
}

void CRenderTarget::ResetTarget()
{
	CONTEXT->OMSetRenderTargets(1, &m_PrevTargetView, m_PrevDepthView);
	SAFE_RELEASE(m_PrevDepthView);
	SAFE_RELEASE(m_PrevTargetView);
}
