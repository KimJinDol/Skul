#include "pch.h"
#include "LogWindow.h"
#include "TestWindow.h"
#include "IMGUIButton.h"
#include "IMGUIConsole.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"

CLogWindow::CLogWindow()
{
}

CLogWindow::~CLogWindow()
{
}

bool CLogWindow::Init()
{
	CIMGUIButton* ClearButton = AddWidget<CIMGUIButton>("Clear");
	ClearButton->SetSize(100.f, 30.f);
	ClearButton->SetClickCallback<CLogWindow>(this, &CLogWindow::ClearButton);

	m_Console = AddWidget<CIMGUIConsole>("Console");

	m_Console->SetMaxHistory(300);

	return true;
}

void CLogWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CLogWindow::ClearButton()
{
	m_Console->Clear();
}

void CLogWindow::AddLog(const char* Text)
{
	m_Console->AddText(Text);
}
