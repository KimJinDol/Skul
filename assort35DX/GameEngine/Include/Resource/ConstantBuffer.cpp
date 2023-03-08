
#include "ConstantBuffer.h"
#include "../Device.h"

CConstantBuffer::CConstantBuffer() :
    m_pBuffer(nullptr)
{
}

CConstantBuffer::~CConstantBuffer()
{
    SAFE_RELEASE(m_pBuffer);
}

bool CConstantBuffer::Init(const std::string& Name, int Size, int Register, int ConstantBufferType)
{
    m_Name = Name;
    m_Size = Size;
    m_Register = Register;
    m_ConstantBufferType = ConstantBufferType;

    D3D11_BUFFER_DESC   Desc = {};

    Desc.Usage = D3D11_USAGE_DYNAMIC;   // Dynamic�� ������ ���� ��� �ٲ� �� �ֱ� ����. (��ġ�� ���� ��� ���� ���࿡ ���� ��� �ٲ�)
    Desc.ByteWidth = Size;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(DEVICE->CreateBuffer(&Desc, nullptr, &m_pBuffer)))
        return false;

    return true;
}

void CConstantBuffer::UpdateBuffer(void* pData)
{
    D3D11_MAPPED_SUBRESOURCE    Map = {};


    /* <DirectX11���� ���ҽ��� ���� �� �����͸� �����ϴ� ���>
    1. D3D11 _ MAPPED _ SUBRESOURCE ���� ������ �����ϰ� 0���� �ʱ�ȭ�մϴ�.�� ������ ����Ͽ� ���ҽ��� �����մϴ�.
    2. �����Ϸ��� �����Ϳ� ���� GPU(�׷��� ó�� ��ġ) �׼����� ������� �ʵ��� �����ϰ� �����Ͱ� ���Ե� �޸𸮿� ���� �����͸� ����ϴ�.
       �� �����͸� �������� ID3D11DeviceContext::Map�� ȣ���� �� MapType �Ű� ������ D3D11_MAP_WRITE_DISCARD�� �����մϴ�.
       �� �����͸� ���� �ܰ��� D3D11_MAPPED_SUBRESOURCE ���� �ּҷ� �����մϴ�.
    3. �� �����͸� �޸𸮿� ���ϴ�.
    4. �� ������ ���Ⱑ �Ϸ�Ǹ� ID3D11DeviceContext::Unmap�� ȣ���Ͽ� �����Ϳ� ���� GPU �׼����� �ٽ� Ȱ��ȭ�մϴ�.*/

    CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map); // �ش� ���۸� GPU���� ������� ���ϵ��� ��Ȱ��ȭ ��Ų ��,
                                                                  // ������ ������ ���� �����ϱ� ���� D3D11_MAP_WRITE_DISCARD �÷��׸� ����Ͽ� Map ������ �����͸� ��´�.

    memcpy(Map.pData, pData, m_Size);   // Map�����ͷ� �ش� ������ �ּ����� �� �����͸� �������ش�.

    CONTEXT->Unmap(m_pBuffer, 0);   // ��Ȱ��ȭ ��Ų ���۸� Unmap�Ͽ� �ٽ� Ȱ��ȭ ���� GPU���� ��� �����ϵ��� �������ش�.

    if (m_ConstantBufferType & CBT_VERTEX)
        CONTEXT->VSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_PIXEL)
        CONTEXT->PSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_DOMAIN)
        CONTEXT->DSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_HULL)
        CONTEXT->HSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_GEOMETRY)
        CONTEXT->GSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_COMPUTE)
        CONTEXT->CSSetConstantBuffers(m_Register, 1, &m_pBuffer);
}
