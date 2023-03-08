#include "Thread.h"

CThread::CThread()	:
	m_hThread(0),
	m_hStart(0),
	m_Loop(true)
{
}

CThread::~CThread()
{
	if (m_hStart)
		CloseHandle(m_hStart);
}

bool CThread::Create()
{
	// CreateEvent()
	// 1. LPSECURITY_ATTRIBUTES lpEventAttributes : 보안 속성을 지정할 때 사용. 핸들을 자식 프로세스에게 상속하고자 할 경우 NULL이 아닌 다른 값을 전달한다.
	// 2. BOOL bManualReset : 이 함수의 가장 중요한 전달인자로서 수동리셋 모드로 이벤트 오브젝트를 생성하느냐, 자동 리셋 모드로 이벤트 오브젝트를 생성하느냐 결정짓는다. 
	//						  (TRUE = 수동 모드 리셋 / FALSE = 자동 모드 리셋)
	// 3. BOOL bInitialState : 이벤트 오브젝트의 초기 상태를 결정짓는다. 
	//						  (TRUE = Signaled 상태의 이벤트 오브젝트 생성 / FALSE = Non-Signaled 상태의 이벤트가 생성)
	// 4. LPCTSTR lpName : 이벤트 오브젝트에 이름을 중 경우에 사용하는 전달인자. (NULL = 이름 없는 이벤트 오브젝트가 생성)
	m_hStart = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, CThread::ThreadFunc,
		this, 0, nullptr);

	return true;
}

void CThread::Wait()
{
	WaitForSingleObject(m_hStart, INFINITE);
}

void CThread::Start()
{
	SetEvent(m_hStart);
}

unsigned int __stdcall CThread::ThreadFunc(void* Arg)
{
	CThread* Thread = (CThread*)Arg;

	Thread->Wait();

	do
	{
		Thread->Run();
	} while (Thread->m_Loop);

	return 0;
}
