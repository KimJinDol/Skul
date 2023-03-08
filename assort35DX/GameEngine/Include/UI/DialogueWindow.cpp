#include "DialogueWindow.h"
#include "../Input.h"
#include "../Dialogue/DialogueManager.h"
#include <atlstr.h>

CDialogueWindow::CDialogueWindow()
{
}

CDialogueWindow::CDialogueWindow(const CDialogueWindow& widget)
{
}

CDialogueWindow::~CDialogueWindow()
{
}

void CDialogueWindow::SetNextDialogue(float DeltaTime)
{
    std::string String = CDialogueManager::GetInst()->GetText();
    if (String == "null")
    {
        Enable(false);
        return;
    }
    int Len = String.length();
    TCHAR Text[256] = {};
    _tcscpy_s(Text, CA2T(String.c_str()));
    m_Text->SetText(Text);
}

void CDialogueWindow::SetNameText(std::string& Name)
{
}

void CDialogueWindow::SetDialogueText(std::string& Text)
{
}

bool CDialogueWindow::Init()
{
    if (!CWidgetWindow::Init())
        return false;

 /*   CDialogueManager::GetInst()->SetDialCode("# Unknown");
    CDialogueManager::GetInst()->StartDialogue();*/

    CInput::GetInst()->AddKeyCallback<CDialogueWindow>("Attack", KT_Down, this, &CDialogueWindow::SetNextDialogue);

    m_BackgroundImage = CreateWidget<CImage>("Background");
    m_BackgroundImage->SetTexture("UI_Atals02", TEXT("UI/UI_Atlas02.png"));
    m_BackgroundImage->SetStartEndUV(0, 1, Vector2(612.f, 200.f));
    m_BackgroundImage->SetPos(350.f, -60.f);
    m_BackgroundImage->SetSize(612.f, 200.f);

    m_Text = CreateWidget<CText>("Text");
    m_Text->SetFont("SkulFont");
    m_Text->SetPos(380.f, -110.f);
    m_Text->SetSize(500.f, 200.f);
    m_Text->SetFontSize(15.f);
    m_Text->SetColor(Vector3(225.f, 208.f, 175.f));

    Enable(false);

    return true;
}

void CDialogueWindow::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CDialogueWindow::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CDialogueWindow::Render()
{
    CWidgetWindow::Render();
}

void CDialogueWindow::ClickCallback()
{
}
