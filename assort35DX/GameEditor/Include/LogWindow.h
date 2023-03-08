#pragma once
#include "IMGUIWindow.h"
class CLogWindow :
    public CIMGUIWindow
{
public:
	CLogWindow();
	virtual ~CLogWindow();

private:
	class CIMGUIConsole* m_Console;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void ClearButton();
	void AddLog(const char* Text);
};

