
#include "StructuredBuffer.h"
#include "../Device.h"

CStructuredBuffer::CStructuredBuffer() :
	m_Desc{},
	m_Buffer(nullptr),
	m_SRV(nullptr),
	m_UAV(nullptr),
	m_Size(0),
	m_Count(0),
	m_ConstantBufferType(0),
	m_Register(0),
	m_Dynamic(false)
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& buffer)
{
	*this = buffer;

	DEVICE->CreateBuffer(&m_Desc, nullptr, &m_Buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};

	// 구조화 버퍼는 원소의 형식이 정해져 있지 않고 사용자가 커스텀해 사용하는 것이기 때문에
	// 원소 형식(Format)을 '알 수 없음'을 뜻하는 DXGI_FORMAT_UNKNOWN플래그로 지정해야 한다.
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	// 버퍼 자원을 위한 셰이더 자원 뷰 서술 구조체를 준비할 때, 뷰에 포함시킬(즉, 뷰들을 통해 접근할 수 있는) 원소 범위의 첫 원소와 개수를 지정해야 한다. 
	// 응용 프로그램의 요구에 따라서는 버퍼의 모든 원소를 뷰에 포함시킬 수도 있고 일부만 포함시킬 수도 있다. 
	// 일부만 포함시키는 경우라도 HLSL 안에서 그 원소들의 색인 범위는 [0, 개수1]이며, 그 바깥에 있는 색인을 사용하는 것으 범위 위반 오류로 간주된다.
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	// CreateShaderResourceView(자원 뷰를 적용할 버퍼자원을 가리키는 포인터,
	//							자원 뷰를 서술하는 구조체 포인터,
	//							생성 성공한 자원뷰를 담을 포인터)
	// -> <m_Buffer라는 자원을 렌더링 파이프 라인에 연결해줄 어댑터 역할의 SRVDesc의 옵션을 가진 자원 뷰를 m_SRV포인터에 만들어주세요~> 라는 뜻.
	DEVICE->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV);

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};

		// 구조화 버퍼는 원소의 형식이 정해져 있지 않고 사용자가 커스텀해 사용하는 것이기 때문에
		// 원소 형식(Format)을 '알 수 없음'을 뜻하는 DXGI_FORMAT_UNKNOWN플래그로 지정해야 한다.
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;

		// 정점버퍼, 색인버퍼와 다르게 표준 버퍼와 구조화 버퍼는 직접 파이프라인에 연결할 수 없다.
		// 그러므로 이 버퍼들은 파이프라인과 자원을 연결해주는 어댑터 역할인 자원 뷰, 순서없는 접근 뷰를 통해 
		// 파이프라인에 연결해 주어야 한다.
		DEVICE->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV);
	}

}

CStructuredBuffer::~CStructuredBuffer()
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);
}

bool CStructuredBuffer::Init(const std::string& Name, unsigned int Size,
	unsigned int Count, int Register, bool Dynamic, int ConstantBufferType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);

	m_Dynamic = Dynamic;

	m_Name = Name;
	m_Size = Size;
	m_Count = Count;
	m_Register = Register;
	m_ConstantBufferType = ConstantBufferType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;

	if (m_Dynamic)
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	else
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
	}

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	if (FAILED(DEVICE->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV)))
		return false;

	// 동적이 아닐 경우 UAV도 만들어준다.
	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};

		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV)))
			return false;
	}

	return true;
}

bool CStructuredBuffer::InitRead(const std::string& Name, unsigned int Size,
	unsigned int Count, int Register, int ConstantBufferType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);

	m_Name = Name;
	m_Size = Size;
	m_Count = Count;
	m_Register = Register;
	m_ConstantBufferType = ConstantBufferType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = 0;
	m_Desc.StructureByteStride = m_Size;
	m_Desc.Usage = D3D11_USAGE_STAGING;
	m_Desc.BindFlags = 0;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		return false;

	return true;
}

void CStructuredBuffer::UpdateBuffer(void* Data, int Size)
{
	if (!m_Dynamic)
		return;

	// Map : 동적 자원에는 동시에 접근해서 값을 변경할 수 없게 하기 위해서 락(rock)이 되어 있는데,
	//		 이에 엑세스 할 수 있는 포인터를 가져와 접근하는 기능.
	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CONTEXT->Map(m_Buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, Data, Size);

	// Unmap : 엑세스한 자원을 해제해주는 기능.
	CONTEXT->Unmap(m_Buffer, 0);
}

CStructuredBuffer* CStructuredBuffer::Clone()
{
	return new CStructuredBuffer(*this);
}

void CStructuredBuffer::SetShader(int Register)
{
	if (m_Dynamic)
	{
		if (m_ConstantBufferType & CBT_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &m_SRV);
	}

	else
	{
		UINT	Count = -1;
		CONTEXT->CSSetUnorderedAccessViews(Register, 1, &m_UAV, &Count);
	}
}

void CStructuredBuffer::ResetShader(int Register)
{
	if (m_Dynamic)
	{
		ID3D11ShaderResourceView* SRV = nullptr;

		if (m_ConstantBufferType & CBT_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &SRV);
	}

	else
	{
		UINT	Count = -1;
		ID3D11UnorderedAccessView* UAV = nullptr;
		CONTEXT->CSSetUnorderedAccessViews(Register, 1, &UAV, &Count);
	}
}

void CStructuredBuffer::SetShader(int Register, int ConstantBufferType)
{
	if (ConstantBufferType & CBT_VERTEX)
		CONTEXT->VSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_PIXEL)
		CONTEXT->PSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_DOMAIN)
		CONTEXT->DSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_HULL)
		CONTEXT->HSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_GEOMETRY)
		CONTEXT->GSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_COMPUTE)
		CONTEXT->CSSetShaderResources(Register, 1, &m_SRV);
}

void CStructuredBuffer::ResetShader(int Register, int ConstantBufferType)
{
	ID3D11ShaderResourceView* SRV = nullptr;

	if (ConstantBufferType & CBT_VERTEX)
		CONTEXT->VSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_PIXEL)
		CONTEXT->PSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_DOMAIN)
		CONTEXT->DSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_HULL)
		CONTEXT->HSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_GEOMETRY)
		CONTEXT->GSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_COMPUTE)
		CONTEXT->CSSetShaderResources(Register, 1, &SRV);
}

void CStructuredBuffer::CopyData(void* Data)
{
	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CONTEXT->Map(m_Buffer, 0, D3D11_MAP::D3D11_MAP_READ, 0, &Map);

	memcpy(Data, Map.pData, m_Size * m_Count);

	CONTEXT->Unmap(m_Buffer, 0);
}

void CStructuredBuffer::CopyResource(CStructuredBuffer* Buffer)
{
	CONTEXT->CopyResource(Buffer->m_Buffer, m_Buffer);
}
