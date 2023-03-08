#pragma once

#include "GameEngine.h"

class CDevice
{
private:
    // 디바이스는 3D 그래픽 카드와 연결되는 기본 디바이스 객체이다. (Hal 디바이스를 거칠 수도 있고 안거칠 수도 있고~)
    // Dx11부터는 멀티스레드를 최적화하기 위해 직접 디바이스를 사용하지 않고 Context를 이용하여 디바이스에 명령을 내리는 방식이다.별로 사용할 일이 많지 않을거란 뜻~
    ID3D11Device* m_pDevice;

    // 디바이스에 명령을 내릴 수 있도록 도와주는 객체.
    // 멀티 코어 / 멀티 스레드를 최적화 하기 위해 Device의 하위에 Context를 분리해서 사용하는 것이다.
    ID3D11DeviceContext* m_pContext;

    // 전면 버퍼와 후면 버퍼는 하나의 교환 사슬을 형성한다. Direct3D에서 하나의 교환 사슬은 IDXGISwapChain이라는 인터페이스로 대표된다.
    // 이 인터페이스는 전면 버퍼 텍스처, 후면 버퍼 텍스처를 담고, 버퍼 크기 변경을 위한 메서드 (ResizeBuffers)와 버퍼의 *제시를 위한 메서드(Present)도 제공한다.
    // *제시 : 후면 버퍼를 전면 버퍼와 교환해서 화면에 표시되게 하는 것
    IDXGISwapChain* m_pSwapChain;
    // SwapChain이 가지고 있는 BackBuffer를 RenderTargetView에 묶어주고 출력을 이 RenderTargetView에 해주게되면
    // 묶여있는 BackBuffer에 픽셀정보가 저장되고 출력이 되게 된다.
    ID3D11RenderTargetView* m_pTargetView;

    // Depth는 깊이 값을 의미하고 Stencil은 픽셀을 합성, 디졸브, 스와이프 등 특수 효과를 내는 데 사용된다.
    ID3D11DepthStencilView* m_pDepthView;

    Resolution              m_RS;
    HWND                    m_hWnd;

    // API는 0~255까지의 정수로 색을 표현했지만 다이렉트11에서는 소수점을 이용한 실수 값으로 색을 표현한다. (r, g, b, a)라서 크기 4 배열.
    float                   m_ClearColor[4];
    Vector2                 m_ResolutionRatio;

private:
    ID2D1RenderTarget* m_2DTarget;
    ID2D1RenderTarget* m_2DTargetWorld;
    ID2D1Factory* m_2DFactory;

public:
    ID2D1RenderTarget* Get2DTarget()   const
    {
        return m_2DTarget;
    }

    ID2D1RenderTarget* Get2DTargetWorld()   const
    {
        return m_2DTargetWorld;
    }

    ID2D1Factory* Get2DFactory()   const
    {
        return m_2DFactory;
    }

public:
    ID3D11Device* GetDevice()   const
    {
        return m_pDevice;
    }

    ID3D11DeviceContext* GetContext()   const
    {
        return m_pContext;
    }

    IDXGISwapChain* GetSwapChain()   const
    {
        return m_pSwapChain;
    }

    Resolution GetResolution()  const
    {
        return m_RS;
    }

    Vector2 GetResolutionRatio()    const;

public:
    bool CreateWorld2DTarget(IDXGISurface* Surface);

public:
    bool Init(HWND hWnd, int Width, int Height, bool ScreenMode = true);
    void Update();
    void ClearTarget();
    void Render();

    DECLARE_SINGLE(CDevice)
};

