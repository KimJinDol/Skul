
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
	// Target Texture 생성
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

	// TargetTexture와 연결된 TargetView를 만들고, 이 렌더 타겟 텍스처 정보는 쉐이더에서도 사용할 것이기 때문에 자원 어댑터 역할인 ShaderResourceView를 만들어준다.
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
	// Get을 하면 내부적으로 래퍼런스 카운트가 증가하기 때문에 다쓰면 Release를 해줘야한다.
	// 원래 우리는 백퍼버의 타겟뷰를 사용하고 있었으므로 m_PrevTargetView에는 백버퍼의 타겟뷰가 들어올 것임.
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
