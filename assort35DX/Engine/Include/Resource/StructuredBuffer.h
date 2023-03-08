#pragma once

#include "../GameEngine.h"

// 구조화 버퍼는 상수 버퍼처럼 하나를 만들어서 넘겨주는게 아니라 같은 형식의 데이터를 여러개 만들어 통으로 넘겨주는 방식이다.

// Structured Buffer는 입력용과 출력용, 두 종류로 나뉜다.
// 입력 버퍼 : 쉐이더 외부에서 원하는 데이터를 입력 받을 수 있게 해주는 버퍼이다. (상수버퍼 처럼)
// 출력 버퍼 : 대부분 rw (읽기 쓰기 가능)으로 만드는데, 컴퓨트 쉐이더에서 연산된 결과를 C++에 바인됭 된 버퍼에 값을 보내준다.
// 			  그래서 출력 버퍼로 사용한다면 c++코드 상에서 그 결과값들을 가져다 사용할 수 있게 되는 개념이다.
class CStructuredBuffer
{
public:
	CStructuredBuffer();
	CStructuredBuffer(const CStructuredBuffer& buffer);
	~CStructuredBuffer();

private:
	std::string         m_Name;
	D3D11_BUFFER_DESC   m_Desc;
	ID3D11Buffer* m_Buffer;

	// 동적 버퍼라면 SRV로, 아니라면 UAV로 작업될 것임.
	ID3D11ShaderResourceView* m_SRV; // 쉐이더 자원 뷰는 파이프라인의 쉐이더 프로그램 단계에서 읽고 사용할 수 있지만 기록하지는 못하는 자원을 말한다.
	ID3D11UnorderedAccessView* m_UAV; // 순서없는 접근 뷰는 쉐이더 자원 뷰처럼 자원 읽기를 가능하게 하고 더 나아가서, 같은 쉐이더 프로그램 안에서 자원을 기록할 수도 있다. 
									  // 그리고 출력 장소가 미리 정해져 있지 않기 때문에 쉐이더 프로그램 안에서 임의의 위치에 Scatter 연산을 수행하는 것도 가능하다.
	unsigned int    m_Size;	// 하나의 크기
	unsigned int    m_Count;
	int             m_ConstantBufferType; // 구조화 버퍼를 어디로 보낼지 (어디에서 사용될지? 구조화 버퍼가 무조건 컴퓨트 쉐이더에서만 사용되는 건 아니기 때문)
	bool            m_Dynamic;	// 동적 버퍼인지 아닌지
	int				m_Register;

public:
	const std::string& GetName()	const
	{
		return m_Name;
	}

public:
	bool Init(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int ConstantBufferType = CBT_COMPUTE);
	bool InitRead(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, int ConstantBufferType = CBT_COMPUTE);
	void UpdateBuffer(void* Data, int Size);
	CStructuredBuffer* Clone();
	void SetShader(int Register);
	void ResetShader(int Register);
	void SetShader(int Register, int ConstantBufferType);
	void ResetShader(int Register, int ConstantBufferType);
	void CopyData(void* Data);	// 버퍼 안의 모든 데이터를 풀인자로 들어온 Data 포인터에 풀복사
	void CopyResource(CStructuredBuffer* Buffer);
};

