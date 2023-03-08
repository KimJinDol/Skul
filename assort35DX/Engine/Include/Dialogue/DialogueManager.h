#pragma once
#include <iostream>
#include <fstream>
#include "../GameEngine.h"

enum DialogueType
{
    EDialogue,
    ELine
};

class CDialogueManager
{
private:
    std::ifstream   m_File;

    bool            m_IsDialogue;
    std::function<void()>	m_pEndDialogueFunc;

    std::string     m_curDialCode;

    bool            m_bEnableAllStop;
    std::string     m_MapName;
    DialogueType    m_Type; // Dialogue인지 Line인지
    std::string     m_DialogueName; // String텍스트 파일에서 세번째 인자에 해당. 다이얼로그 이름
    std::string     m_Speaker;// 말하는 애
    int             m_DialogueNum1; // 4번째 인자
    bool            m_OnTermEvent;
    int             m_DialogueNum2; // 5번째 인자

    class CDialogueWindow* m_DialogueWindow;

private:
    void FindDialogueCursor();
    void EndDialogue();

public:
    void SetDialogueWindow(class CDialogueWindow* Window);
    void StartDialogue();
    void SetEnableAllStop(bool Value)
    {
        m_bEnableAllStop = Value;
    }

    void SetDialCode(const std::string& dialCode)
    {
        m_curDialCode = dialCode;
        FindDialogueCursor();
    }

public:
    bool GetEnableAllStop() const
    {
        return m_bEnableAllStop;
    }

    std::string GetDialCode() const
    {
        return m_curDialCode;
    }

    std::string GetName() const;

public:
    bool Init();


    bool IsPlayDialogue()   const
    {
        return m_IsDialogue;
    }

    template<typename T>
    void SetEndDialogueFunc(T* pObj, void(T::* pFunc)())
    {
        m_pEndDialogueFunc = std::bind(pFunc, pObj);
    }

    std::string GetText();

    DECLARE_SINGLE(CDialogueManager)
};

