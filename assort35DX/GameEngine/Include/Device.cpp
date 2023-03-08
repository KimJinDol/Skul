#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()  :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pTargetView(nullptr),
	m_pDepthView(nullptr),
	m_RS{},
	m_hWnd(nullptr),
    m_ClearColor{},
	m_2DTarget(nullptr),
	m_2DTargetWorld(nullptr),
	m_2DFactory(nullptr)
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_2DTargetWorld);
	SAFE_RELEASE(m_2DTarget);
	SAFE_RELEASE(m_2DFactory);

	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pSwapChain);

	// ClearState : ��� �⺻ ������ �������ִ� �Լ�
	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

Vector2 CDevice::GetResolutionRatio() const
{
	return m_ResolutionRatio;
}

bool CDevice::CreateWorld2DTarget(IDXGISurface* Surface)
{
	D2D1_RENDER_TARGET_PROPERTIES	props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(Surface,
		props, &m_2DTargetWorld)))
		return false;

	return true;
}

bool CDevice::Init(HWND hWnd, int Width, int Height, bool ScreenMode)
{
    m_hWnd = hWnd;
    m_RS.Width = Width;
    m_RS.Height = Height;

    unsigned int    Flag = 0;

#ifdef _DEBUG

    Flag = D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

    Flag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    DXGI_SWAP_CHAIN_DESC    SwapDesc = {};

    // Create�� ���� ü���� ������ ������ ����ü
    SwapDesc.BufferDesc.Width = Width;
    SwapDesc.BufferDesc.Height = Height;
    SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // ����� �ȼ� ������ ����.
    SwapDesc.BufferDesc.RefreshRate.Numerator = 1;              // ȭ���� ���ʿ� �� �� �׸����� (Numerator : ����, Denominator : �и�)(1/60�ʿ� �� �� �׸���~)
    SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
    SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;    // â�� Ȯ��/��� �� ��� �Ұ���. (DXGI_MODE_SCALING_UNSPECIFIED : ������ �������� ���� ���̴�~)
    SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;    // �̰� ���� (DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED : ��ĵ ���� ������ �������� �ʰڴ�~)
    SwapDesc.BufferCount = 1;                                   // ����� �� �� �������.
    SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // ������ �뵵�� ����. (DXGI_USAGE_RENDER_TARGET_OUTPUT : ���� Ÿ�� ��¿����� ����� �ϰڴ�~)
    SwapDesc.OutputWindow = hWnd;                               // ����� �����찡 �������� ����.
    SwapDesc.SampleDesc.Count = 2;                              // ��Ƽ ���ø�(������̰��� ��� ����, �ȼ� ������ ��ȭ�����ִ� ���) ����. (1 : ��Ƽ ���ø��� ���Ѵٴ� ��. ���� �ε巴�� ���ַ��� 4�� 8�� ���� �ִ´ٰ� ��)
    SwapDesc.SampleDesc.Quality = 0;
    SwapDesc.Windowed = ScreenMode;                             // â���� ����, Full Screen���� ���� ����. (��ũ�� ��� ����)
    SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;             // IDXGISwapChain1::Present1�� ȣ���� ���Ŀ� ���÷��� �����̽� ������ �ȼ� ������ ���� �ɼ�.
                                                                // DXGI_SWAP_EFFECT_DISCARD : �� �÷��״� IDXGISwapChain1::Present1�� ȣ���� ���Ŀ� bit-block transfer(bitblt) ���� �����ϰ� DXGI�� 
                                                                // back buffer�� ������ ���쵵�� �����Ѵ�. �� �÷��״� �ϳ� �̻��� back buffer�� ���� swap chain�� ���� ��ȿ������ ���� ���α׷��� 0�� buffer(ù��° ����)
                                                                // �� ���� �б�/���� ������ �����ϴ�.

    D3D_FEATURE_LEVEL   Flevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL   Flevel1 = D3D_FEATURE_LEVEL_11_0;

    // Device, Context, SwapChain�� �����Ѵ�.
    // ù��° ���� : ����� (�� ����̽����� �����Ͽ� ����� �׷��� ī�� : nullptr�� ���� PC�� �⺻ �׷��� ī�带 ����)
    // �ι�° ���� : �׷����� ����ϰ� �� ����̹� Ÿ��. HARDWARE�� �׷��� ī�带 �ǹ��ϰ�, ���� �׷��� ī�尡 ���� PC�� ���, CPU���� ��ü������ �׷��� ��¿� ����� ���ķ����͸� ���� �ٷ��� ������. �ٵ� �س� ����)
    // ����° ���� : �ι�° ���ڿ��� �����ߵ�, �׷��� ī�� ���� ����� ����� ���� �����ϴ� ���̴�. �츮�� �׷��� ī��� ����� ���̱� ������ ������ �ʿ䰡 ����.
    // �׹�° ���� : ����̽� ���� �� �ʿ��� �÷���
    // �ټ�° ���� : ����� Ư�� �ٷ�����(������ ��������� �迭�� ����� �ּ� �ѱ�� ��)
    // ����° ���� : �ټ�° ���ڿ��� Ư���� ���� ���� (���⼱ 1����?)
    // �ϰ�° ���� : SDK ����
    // ����° ���� : ����ü�� Desc ����ü
    // ��ȩ° ���� : ����° ���ڸ� ���� ������ ����ü���� ������ ���� ������
    // ������ ���� : ������ Device�� ������ ���� ������
    // ���Ѥ� ���� : �����ϴ� �ٷ� ���� ����
    // ���Τ� ���� : ������ Context�� ������ ���� ������
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag, &Flevel, 1,
        D3D11_SDK_VERSION, &SwapDesc, &m_pSwapChain, &m_pDevice, &Flevel1, &m_pContext)))
        return false;

    // SwapChain�� ������ �ִ� ����۸� ���´�.
    // ID3D11Texture2D : DirectX11���� �����Ǵ� Texture ��ü�̴�.
    // SwapChain�� Texture�� BackBuffer ������ ������ �ֱ� ������ �� Ÿ���� ������ �����Ͽ�
    // ���۸� ������´�.
    ID3D11Texture2D* pBackBuffer = nullptr;

    // 2�� ���ڿ� ���� Ÿ���� ID�� �̿��Ͽ� SwapChain�� ������ �ִ� �ش� Ÿ���� ���۸� ���´�.
    // �̷��� ���� ���۴� RefCount�� �����ϰ� �ȴ�.
    // �׷��Ƿ� ��� ���ÿ��� �ݵ�� Release�� ���־�� �Ѵ�.
    // IDXGISwapChain::GetBuffer(������ ������ �ε���, ���۸� �����ϴµ��� ����� �������̽�, ������ ����۸� ������ ���� ������)
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    // ������ ���� BackBuffer�� �̿��ؼ� RenderTargetView�� �����ϰ� BackBuffer�� �� RenderTargetView��
    // �����ش�.
    m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

    SAFE_RELEASE(pBackBuffer);

    // ���̹��۸� ��������� �ؽ�ó Desc����ü�� �������ش�.
    // 2D������ �ʿ����� ������ ������ ���� �����
    D3D11_TEXTURE2D_DESC    DepthDesc = {};

    DepthDesc.Width = Width;        // �⺻������ ���� ���۴� ����ۿ� �ȼ��� 1:1 ������ �Ǿ�� ��.
    DepthDesc.Height = Height;
    DepthDesc.ArraySize = 1;        // �ϳ��� �ؽ��� ��ü�� ���� ũ���� �׷��� �̹����� �����ϱ� ���� ���� ��.

    DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;   // �ؽ�ó�� �ȼ� ����
                                                        // (���� ���� : Unsinged Normal�̹Ƿ� 0~1������ ���� 24��Ʈ�� �����ϰ�, Stencil ���� : Unsingned Int �̹Ƿ� 0~255������ ���� 8��Ʈ�� �����ϰڴ�) 
    DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     // BindFlags : �ش� �ؽ�ó�� ����� ����� �뵵�� �����Ѵ�.
    DepthDesc.Usage = D3D11_USAGE_DEFAULT;              // �ش� �ؽ�ó�� �뵵. ���� �ٸ����� ��¿�����, ���ٿ�����, �������� ó���� �� �ִ� ���ۿ����� ����� �뵵�� �����Ѵ�..
    DepthDesc.SampleDesc.Count = 2;                     // ��Ƽ ���ø�(������̰��� ��� ����, �ȼ� ������ ��ȭ�����ִ� ���) ����. (1 : ��Ƽ ���ø��� ���Ѵٴ� ��. ���� �ε巴�� ���ַ��� 4�� 8�� ���� �ִ´ٰ� ��)
    DepthDesc.SampleDesc.Quality = 0;
    DepthDesc.MipLevels = 1;                            // �Ӹ��� �ִ� �ܰ踦 ����. ����� 2D�̹Ƿ� LOD�� �������. �׷��Ƿ� Level�� 1�� ����.
                                                        // �Ӹ� : �ؽ�ó�� ����� �������� ������ �ӵ��� ����Ű�� ���� ��ҵ� ������ �þ���� ���. �ϳ��� �̹����� ũ�⺰�� (128*128,64*64 ���)�þ���� LOD�� ���� 
                                                        //        �������� ������ ū �ؽ�ó�� ������ ����̴�.

    ID3D11Texture2D* pDepthStencilTex = nullptr;

    m_pDevice->CreateTexture2D(&DepthDesc, nullptr, &pDepthStencilTex);

    m_pDevice->CreateDepthStencilView(pDepthStencilTex, nullptr, &m_pDepthView);

    SAFE_RELEASE(pDepthStencilTex);

    // Device�� ������ RenderTargetView�� DepthStencilView�� �����ش�. ���� ��� �� �� ���� Ÿ�� ��� ���� ���ٽ� �並 �̿��ض�! ��� ���� ��.
    // ���λ� OM : Output Merger (��� ���ձ�)�� ����.
    // m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

    // ���� ��µǴ� 2D ����Ʈ ������ ������ش�.
    D3D11_VIEWPORT  VP = {};

    VP.Width = (float)Width;
    VP.Height = (float)Height;
    VP.MaxDepth = 1.f;      // �ش� ����Ʈ���� ����� �� �ִ� �ִ� ���̰�

    // ���λ� RS : Rasterize State (�����Ͷ����� ����)�� ����.
    m_pContext->RSSetViewports(1, &VP);

    // 2D Factory�� ������ش�.
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
        return false;

    // 3D BackBuffer�� Surface�� ���´�.
    IDXGISurface* BackSurface = nullptr;

    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&BackSurface));

    // 2D�� ����Ÿ���� ������ش�.
    // ��, ����� Surface�� ������ ���� BackBuffer�� Surface�� �����Ͽ� 2D��
    // ����� �����ϵ��� ����� �־�� �Ѵ�.
    D2D1_RENDER_TARGET_PROPERTIES	props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_HARDWARE,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

    if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(BackSurface,
        props, &m_2DTarget)))
        return false;

    SAFE_RELEASE(BackSurface);

    return true;
}

void CDevice::Update()
{
    RECT	rcClient = {};
    GetClientRect(m_hWnd, &rcClient);

    float	Width = (float)(rcClient.right - rcClient.left);
    float	Height = (float)(rcClient.bottom - rcClient.top);

    m_ResolutionRatio.x = m_RS.Width / Width;
    m_ResolutionRatio.y = m_RS.Height / Height;
}

void CDevice::ClearTarget()
{
    // ���λ� OM : Output Merger (��� ���ձ�)�� ����.
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

	m_pContext->ClearRenderTargetView(m_pTargetView, m_ClearColor);
	m_pContext->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f, 0);
}

void CDevice::Render()
{
    // IDXGISwapChain::Present() : ������ �ø����� �����ϰ� �������ִ� �Լ�
    m_pSwapChain->Present(0, 0);
}
