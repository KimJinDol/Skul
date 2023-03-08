#pragma once

#include "GameEngine.h"

class CThread
{
	friend class CThreadManager;

protected:
	CThread();
	virtual ~CThread() = 0;

private:
	HANDLE	m_hThread;	// 쓰레드 핸들
	HANDLE	m_hStart;	// 이벤트 핸들 (원할 때 쓰레드를 동작 시키기 위햐서?)
	bool	m_Loop;

public:
	bool Create();
	virtual void Run() = 0;

public:
	void Wait();
	void Start();
	void SetLoop(bool Loop)
	{
		m_Loop = Loop;
	}

public:
	static unsigned int __stdcall ThreadFunc(void* Arg); // 쓰레드 콜백 함수
};

