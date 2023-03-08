
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

    Desc.Usage = D3D11_USAGE_DYNAMIC;   // Dynamic인 이유는 값이 계속 바뀔 수 있기 때문. (위치값 같은 경우 게임 진행에 따라 계속 바뀜)
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


    /* <DirectX11에서 리소스에 대한 새 데이터를 설정하는 방법>
    1. D3D11 _ MAPPED _ SUBRESOURCE 형식 변수를 선언하고 0으로 초기화합니다.이 변수를 사용하여 리소스를 변경합니다.
    2. 변경하려는 데이터에 대한 GPU(그래픽 처리 장치) 액세스를 사용하지 않도록 설정하고 데이터가 포함된 메모리에 대한 포인터를 얻습니다.
       이 포인터를 얻으려면 ID3D11DeviceContext::Map를 호출할 때 MapType 매개 변수에 D3D11_MAP_WRITE_DISCARD를 전달합니다.
       이 포인터를 이전 단계의 D3D11_MAPPED_SUBRESOURCE 변수 주소로 설정합니다.
    3. 새 데이터를 메모리에 씁니다.
    4. 새 데이터 쓰기가 완료되면 ID3D11DeviceContext::Unmap을 호출하여 데이터에 대한 GPU 액세스를 다시 활성화합니다.*/

    CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map); // 해당 버퍼를 GPU에서 사용하지 못하도록 비활성화 시킨 후,
                                                                  // 버퍼의 데이터 값을 변경하기 위해 D3D11_MAP_WRITE_DISCARD 플래그를 사용하여 Map 변수에 포인터를 담는다.

    memcpy(Map.pData, pData, m_Size);   // Map포인터로 해당 버퍼의 주소지로 들어가 데이터를 변경해준다.

    CONTEXT->Unmap(m_pBuffer, 0);   // 비활성화 시킨 버퍼를 Unmap하여 다시 활성화 시켜 GPU에서 사용 가능하도록 변경해준다.

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
