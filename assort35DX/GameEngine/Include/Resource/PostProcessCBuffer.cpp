
#include "PostProcessCBuffer.h"
#include "ConstantBuffer.h"

CPostProcessCBuffer::CPostProcessCBuffer() :
    m_BufferData{}
{
}

CPostProcessCBuffer::CPostProcessCBuffer(const CPostProcessCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CPostProcessCBuffer::~CPostProcessCBuffer()
{
}

bool CPostProcessCBuffer::Init()
{
    // Init()은 CRenderManager::SetDefaultTarget()에서 실행된다.
    SetConstantBuffer("PostProcessCBuffer");

    return true;
}

void CPostProcessCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CPostProcessCBuffer* CPostProcessCBuffer::Clone()
{
    return new CPostProcessCBuffer(*this);
}
