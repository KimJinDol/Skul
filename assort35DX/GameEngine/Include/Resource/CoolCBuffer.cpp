#include "CoolCBuffer.h"
#include "ConstantBuffer.h"

CCoolTimeCBuffer::CCoolTimeCBuffer()    :
    m_BufferData{}
{
    m_BufferData.CoolRatio = 0.f;
    m_BufferData.CoolDir = 1;
    m_BufferData.CoolCircle = 0;
}

CCoolTimeCBuffer::CCoolTimeCBuffer(const CCoolTimeCBuffer& buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CCoolTimeCBuffer::~CCoolTimeCBuffer()
{

}

bool CCoolTimeCBuffer::Init()
{
    SetConstantBuffer("CoolTimeCBuffer");

    return true;
}

void CCoolTimeCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CCoolTimeCBuffer* CCoolTimeCBuffer::Clone()
{
    return new CCoolTimeCBuffer(*this);
}
