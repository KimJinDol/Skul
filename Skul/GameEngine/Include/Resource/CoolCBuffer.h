#pragma once
#include "ConstantBufferData.h"
class CCoolTimeCBuffer :
    public CConstantBufferData
{
public:
	CCoolTimeCBuffer();
	CCoolTimeCBuffer(const CCoolTimeCBuffer& buffer);
	virtual ~CCoolTimeCBuffer();

protected:
	CoolTimeCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CCoolTimeCBuffer* Clone();

public:
	void SetCircle(int Circle)
	{
		m_BufferData.CoolCircle = Circle;
	}

	void SetDir(int Dir)
	{
		m_BufferData.CoolDir = Dir;
	}

	void SetRatio(float Ratio)
	{
		m_BufferData.CoolRatio = Ratio;
	}
};

