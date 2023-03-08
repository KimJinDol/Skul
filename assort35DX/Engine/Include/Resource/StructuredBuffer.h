#pragma once

#include "../GameEngine.h"

// ����ȭ ���۴� ��� ����ó�� �ϳ��� ���� �Ѱ��ִ°� �ƴ϶� ���� ������ �����͸� ������ ����� ������ �Ѱ��ִ� ����̴�.

// Structured Buffer�� �Է¿�� ��¿�, �� ������ ������.
// �Է� ���� : ���̴� �ܺο��� ���ϴ� �����͸� �Է� ���� �� �ְ� ���ִ� �����̴�. (������� ó��)
// ��� ���� : ��κ� rw (�б� ���� ����)���� ����µ�, ��ǻƮ ���̴����� ����� ����� C++�� ���Ή� �� ���ۿ� ���� �����ش�.
// 			  �׷��� ��� ���۷� ����Ѵٸ� c++�ڵ� �󿡼� �� ��������� ������ ����� �� �ְ� �Ǵ� �����̴�.
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

	// ���� ���۶�� SRV��, �ƴ϶�� UAV�� �۾��� ����.
	ID3D11ShaderResourceView* m_SRV; // ���̴� �ڿ� ��� ������������ ���̴� ���α׷� �ܰ迡�� �а� ����� �� ������ ��������� ���ϴ� �ڿ��� ���Ѵ�.
	ID3D11UnorderedAccessView* m_UAV; // �������� ���� ��� ���̴� �ڿ� ��ó�� �ڿ� �б⸦ �����ϰ� �ϰ� �� ���ư���, ���� ���̴� ���α׷� �ȿ��� �ڿ��� ����� ���� �ִ�. 
									  // �׸��� ��� ��Ұ� �̸� ������ ���� �ʱ� ������ ���̴� ���α׷� �ȿ��� ������ ��ġ�� Scatter ������ �����ϴ� �͵� �����ϴ�.
	unsigned int    m_Size;	// �ϳ��� ũ��
	unsigned int    m_Count;
	int             m_ConstantBufferType; // ����ȭ ���۸� ���� ������ (��𿡼� ������? ����ȭ ���۰� ������ ��ǻƮ ���̴������� ���Ǵ� �� �ƴϱ� ����)
	bool            m_Dynamic;	// ���� �������� �ƴ���
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
	void CopyData(void* Data);	// ���� ���� ��� �����͸� Ǯ���ڷ� ���� Data �����Ϳ� Ǯ����
	void CopyResource(CStructuredBuffer* Buffer);
};

