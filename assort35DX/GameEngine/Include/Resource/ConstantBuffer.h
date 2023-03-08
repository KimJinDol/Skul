#pragma once

#include "../GameEngine.h"

class CConstantBuffer
{
    friend class CShaderManager;

private:
    CConstantBuffer();
    ~CConstantBuffer();

private:
    std::string     m_Name;
    ID3D11Buffer* m_pBuffer;
    int             m_Size; // 버퍼의 크기
    int             m_Register; // 버퍼의 레지스터(임시 기억 공간) 번호
    int             m_ConstantBufferType;   // 이 값에 따라 어떤 쉐이더를 쓸지 결정 됨.

public:
    bool Init(const std::string& Name, int Size, int Register, int ConstantBufferType);

    // 원하는 데이터를 Update 시켜줄 함수. 어떤 데이터가 들어올 지 모르기 때문에 void* 타입으로 세팅.
    void UpdateBuffer(void* pData);
};

