#pragma once

#include "GameEngine.h"

class CDevice
{
private:
    // ����̽��� 3D �׷��� ī��� ����Ǵ� �⺻ ����̽� ��ü�̴�. (Hal ����̽��� ��ĥ ���� �ְ� �Ȱ�ĥ ���� �ְ�~)
    // Dx11���ʹ� ��Ƽ�����带 ����ȭ�ϱ� ���� ���� ����̽��� ������� �ʰ� Context�� �̿��Ͽ� ����̽��� ����� ������ ����̴�.���� ����� ���� ���� �����Ŷ� ��~
    ID3D11Device* m_pDevice;

    // ����̽��� ����� ���� �� �ֵ��� �����ִ� ��ü.
    // ��Ƽ �ھ� / ��Ƽ �����带 ����ȭ �ϱ� ���� Device�� ������ Context�� �и��ؼ� ����ϴ� ���̴�.
    ID3D11DeviceContext* m_pContext;

    // ���� ���ۿ� �ĸ� ���۴� �ϳ��� ��ȯ �罽�� �����Ѵ�. Direct3D���� �ϳ��� ��ȯ �罽�� IDXGISwapChain�̶�� �������̽��� ��ǥ�ȴ�.
    // �� �������̽��� ���� ���� �ؽ�ó, �ĸ� ���� �ؽ�ó�� ���, ���� ũ�� ������ ���� �޼��� (ResizeBuffers)�� ������ *���ø� ���� �޼���(Present)�� �����Ѵ�.
    // *���� : �ĸ� ���۸� ���� ���ۿ� ��ȯ�ؼ� ȭ�鿡 ǥ�õǰ� �ϴ� ��
    IDXGISwapChain* m_pSwapChain;
    // SwapChain�� ������ �ִ� BackBuffer�� RenderTargetView�� �����ְ� ����� �� RenderTargetView�� ���ְԵǸ�
    // �����ִ� BackBuffer�� �ȼ������� ����ǰ� ����� �ǰ� �ȴ�.
    ID3D11RenderTargetView* m_pTargetView;

    // Depth�� ���� ���� �ǹ��ϰ� Stencil�� �ȼ��� �ռ�, ������, �������� �� Ư�� ȿ���� ���� �� ���ȴ�.
    ID3D11DepthStencilView* m_pDepthView;

    Resolution              m_RS;
    HWND                    m_hWnd;

    // API�� 0~255������ ������ ���� ǥ�������� ���̷�Ʈ11������ �Ҽ����� �̿��� �Ǽ� ������ ���� ǥ���Ѵ�. (r, g, b, a)�� ũ�� 4 �迭.
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

