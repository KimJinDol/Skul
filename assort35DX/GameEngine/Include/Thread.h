#pragma once

#include "GameEngine.h"

class CThread
{
	friend class CThreadManager;

protected:
	CThread();
	virtual ~CThread() = 0;

private:
	HANDLE	m_hThread;	// ������ �ڵ�
	HANDLE	m_hStart;	// �̺�Ʈ �ڵ� (���� �� �����带 ���� ��Ű�� ���Ἥ?)
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
	static unsigned int __stdcall ThreadFunc(void* Arg); // ������ �ݹ� �Լ�
};

