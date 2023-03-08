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

	// ClearState : 모든 기본 설정을 복원해주는 함수
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

    // Create할 스왑 체인의 정보를 설정할 구조체
    SwapDesc.BufferDesc.Width = Width;
    SwapDesc.BufferDesc.Height = Height;
    SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // 백버퍼 픽셀 포맷을 설정.
    SwapDesc.BufferDesc.RefreshRate.Numerator = 1;              // 화면을 몇초에 한 번 그릴건지 (Numerator : 분자, Denominator : 분모)(1/60초에 한 번 그린다~)
    SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
    SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;    // 창을 확대/축소 시 어떻게 할건지. (DXGI_MODE_SCALING_UNSPECIFIED : 배율을 지정하지 않을 것이다~)
    SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;    // 이건 뭐누 (DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED : 스캔 라인 순서를 지정하지 않겠다~)
    SwapDesc.BufferCount = 1;                                   // 백버퍼 몇 개 만들건지.
    SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // 버퍼의 용도를 지정. (DXGI_USAGE_RENDER_TARGET_OUTPUT : 렌더 타겟 출력용으로 사용을 하겠다~)
    SwapDesc.OutputWindow = hWnd;                               // 출력할 윈도우가 무엇인지 설정.
    SwapDesc.SampleDesc.Count = 2;                              // 멀티 샘플링(엘리어싱같은 계단 현상, 픽셀 깨짐을 완화시켜주는 기법) 관련. (1 : 멀티 샘플링을 안한다는 뜻. 보통 부드럽게 해주려면 4나 8의 값을 넣는다고 함)
    SwapDesc.SampleDesc.Quality = 0;
    SwapDesc.Windowed = ScreenMode;                             // 창모드로 할지, Full Screen으로 할지 설정. (스크린 모드 설정)
    SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;             // IDXGISwapChain1::Present1를 호출한 이후에 디스플레이 서페이스 내부의 픽셀 관리에 대한 옵션.
                                                                // DXGI_SWAP_EFFECT_DISCARD : 이 플래그는 IDXGISwapChain1::Present1을 호출한 이후에 bit-block transfer(bitblt) 모델을 지정하고 DXGI가 
                                                                // back buffer의 내용을 지우도록 지정한다. 이 플래그는 하나 이상의 back buffer를 가진 swap chain에 대해 유효하지만 응용 프로그램은 0번 buffer(첫번째 버퍼)
                                                                // 에 대한 읽기/쓰기 접근이 가능하다.

    D3D_FEATURE_LEVEL   Flevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL   Flevel1 = D3D_FEATURE_LEVEL_11_0;

    // Device, Context, SwapChain을 생성한다.
    // 첫번째 인자 : 어댑터 (이 디바이스에서 연결하여 사용할 그래픽 카드 : nullptr은 현재 PC의 기본 그래픽 카드를 말함)
    // 두번째 인자 : 그래픽을 출력하게 할 드라이버 타입. HARDWARE는 그래픽 카드를 의미하고, 만약 그래픽 카드가 없는 PC일 경우, CPU에서 자체적으로 그래픽 출력에 사용할 에뮬레이터를 통해 다렉을 돌려줌. 근데 준나 느림)
    // 세번째 인자 : 두번째 인자에서 설명했듯, 그래픽 카드 외의 모듈을 사용할 때에 설정하는 값이다. 우리는 그래픽 카드로 출력할 것이기 때문에 설정할 필요가 없음.
    // 네번째 인자 : 디바이스 생성 시 필요한 플래그
    // 다섯째 인자 : 사용할 특정 다렉버전(버전이 여러개라면 배열로 만들어 주소 넘기면 됨)
    // 여섯째 인자 : 다섯째 인자에서 특정된 버전 개수 (여기선 1개쥬?)
    // 일곱째 인자 : SDK 버전
    // 여덟째 인자 : 스왑체인 Desc 구조체
    // 아홉째 인자 : 여덟째 인자를 통해 생성한 스왑체인을 저장할 이중 포인터
    // 열번쨰 인자 : 생성된 Device를 저장할 이중 포인터
    // 열한쨰 인자 : 지원하는 다렉 레벨 수준
    // 열두쨰 인자 : 생성된 Context를 저장할 이중 포인터
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag, &Flevel, 1,
        D3D11_SDK_VERSION, &SwapDesc, &m_pSwapChain, &m_pDevice, &Flevel1, &m_pContext)))
        return false;

    // SwapChain이 가지고 있는 백버퍼를 얻어온다.
    // ID3D11Texture2D : DirectX11에서 지원되는 Texture 객체이다.
    // SwapChain이 Texture로 BackBuffer 정보를 가지고 있기 때문에 이 타입의 변수를 선언하여
    // 버퍼를 가지고온다.
    ID3D11Texture2D* pBackBuffer = nullptr;

    // 2번 인자에 들어가는 타입의 ID를 이용하여 SwapChain이 가지고 있는 해당 타입의 버퍼를 얻어온다.
    // 이렇게 얻어온 버퍼는 RefCount가 증가하게 된다.
    // 그러므로 모두 사용시에는 반드시 Release를 해주어야 한다.
    // IDXGISwapChain::GetBuffer(접근할 버퍼의 인덱스, 버퍼를 조작하는데에 사용할 인터페이스, 가져온 백버퍼를 저장할 이중 포인터)
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    // 위에서 얻어온 BackBuffer를 이용해서 RenderTargetView를 생성하고 BackBuffer를 이 RenderTargetView에
    // 묶어준다.
    m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

    SAFE_RELEASE(pBackBuffer);

    // 깊이버퍼를 만들기위해 텍스처 Desc구조체를 설정해준다.
    // 2D에서는 필요하지 않지만 나중을 위해 빌드업
    D3D11_TEXTURE2D_DESC    DepthDesc = {};

    DepthDesc.Width = Width;        // 기본적으로 깊이 버퍼는 백버퍼와 픽셀이 1:1 대응이 되어야 함.
    DepthDesc.Height = Height;
    DepthDesc.ArraySize = 1;        // 하나의 텍스쳐 객체에 같은 크기의 그래픽 이미지를 저장하기 위해 사용될 것.

    DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;   // 텍스처의 픽셀 포맷
                                                        // (깊이 정보 : Unsinged Normal이므로 0~1사이의 값을 24비트로 저장하고, Stencil 정보 : Unsingned Int 이므로 0~255사이의 값을 8비트에 저장하겠다) 
    DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     // BindFlags : 해당 텍스처를 사용할 기능의 용도를 지정한다.
    DepthDesc.Usage = D3D11_USAGE_DEFAULT;              // 해당 텍스처의 용도. 위와 다른점은 출력용인지, 접근용인지, 동적으로 처리할 수 있는 버퍼용인지 등등의 용도를 구분한다..
    DepthDesc.SampleDesc.Count = 2;                     // 멀티 샘플링(엘리어싱같은 계단 현상, 픽셀 깨짐을 완화시켜주는 기법) 관련. (1 : 멀티 샘플링을 안한다는 뜻. 보통 부드럽게 해주려면 4나 8의 값을 넣는다고 함)
    DepthDesc.SampleDesc.Quality = 0;
    DepthDesc.MipLevels = 1;                            // 밉맵의 최대 단계를 설정. 현재는 2D이므로 LOD가 상관없다. 그러므로 Level을 1로 설정.
                                                        // 밉맵 : 텍스처를 씌우는 과정에서 렌더링 속도를 향상시키기 위해 축소된 패턴을 늘어놓은 방식. 하나의 이미지를 크기별로 (128*128,64*64 등등)늘어놓고 LOD에 따라 
                                                        //        가까울수록 비율이 큰 텍스처를 입히는 방식이다.

    ID3D11Texture2D* pDepthStencilTex = nullptr;

    m_pDevice->CreateTexture2D(&DepthDesc, nullptr, &pDepthStencilTex);

    m_pDevice->CreateDepthStencilView(pDepthStencilTex, nullptr, &m_pDepthView);

    SAFE_RELEASE(pDepthStencilTex);

    // Device에 생성한 RenderTargetView와 DepthStencilView를 묶어준다. 최종 출력 때 이 렌더 타겟 뷰와 뎁스 스텐실 뷰를 이용해라! 라는 뜻이 됨.
    // 접두사 OM : Output Merger (출력 병합기)의 약자.
    // m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

    // 실제 출력되는 2D 뷰포트 영역을 만들어준다.
    D3D11_VIEWPORT  VP = {};

    VP.Width = (float)Width;
    VP.Height = (float)Height;
    VP.MaxDepth = 1.f;      // 해당 뷰포트에서 취급할 수 있는 최대 깊이값

    // 접두사 RS : Rasterize State (레스터라이즈 상태)의 약자.
    m_pContext->RSSetViewports(1, &VP);

    // 2D Factory를 만들어준다.
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
        return false;

    // 3D BackBuffer의 Surface을 얻어온다.
    IDXGISurface* BackSurface = nullptr;

    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&BackSurface));

    // 2D용 렌더타겟을 만들어준다.
    // 단, 출력할 Surface는 위에서 얻어온 BackBuffer의 Surface로 지정하여 2D도
    // 출력이 가능하도록 만들어 주어야 한다.
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
    // 접두사 OM : Output Merger (출력 병합기)의 약자.
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

	m_pContext->ClearRenderTargetView(m_pTargetView, m_ClearColor);
	m_pContext->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f, 0);
}

void CDevice::Render()
{
    // IDXGISwapChain::Present() : 페이지 플립핑을 실행하고 렌더해주는 함수
    m_pSwapChain->Present(0, 0);
}
