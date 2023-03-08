#include "DialogueManager.h"
#include "../PathManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneMode.h"
#include "../Input.h"
#include "../UI/DialogueWindow.h"
#include "../Engine.h"
#include "../Scene/Viewport.h"

DEFINITION_SINGLE(CDialogueManager)

CDialogueManager::CDialogueManager()    :
    m_IsDialogue(false)
{
}

CDialogueManager::~CDialogueManager()
{
    if (m_File.is_open())
        m_File.close();
}

std::string CDialogueManager::GetName() const
{
    return m_Speaker;
}

bool CDialogueManager::Init()
{
    return true;
}

void CDialogueManager::SetDialogueWindow(CDialogueWindow* Window)
{
    m_DialogueWindow = Window;
}

void CDialogueManager::FindDialogueCursor()
{
    if (m_File.is_open())
        m_File.close();

    char    FullPath[MAX_PATH] = {};

    const char* pPath = CPathManager::GetInst()->FindPath(STRING_PATH)->pPathMultibyte;

    if (pPath)
        strcpy_s(FullPath, pPath);

    strcat_s(FullPath, "strings.txt");

    m_File.open(FullPath);

    std::string Line;
    int Offset = 0;

    while (!m_File.eof())
    {
        std::getline(m_File, Line);

        // 해당 고유번호를 찾았다면 아래 실행
        if (Offset = Line.find(m_curDialCode, 0) != std::string::npos)
            return;
    }
}

void CDialogueManager::EndDialogue()
{
    m_IsDialogue = false;

    if (m_pEndDialogueFunc)
        m_pEndDialogueFunc();

    assert(m_DialogueWindow);

    m_DialogueWindow->Enable(false);
}

void CDialogueManager::StartDialogue()
{
    m_IsDialogue = true;

    assert(m_DialogueWindow);

    CScene* Scene =CSceneManager::GetInst()->GetScene();
    CViewport* Viewport = Scene->GetViewport();
    Viewport->AllWindowEnable(false);

    m_DialogueWindow->Enable(true);
}

std::string CDialogueManager::GetText()
{
    char Line[2000] = {};

    if (!m_IsDialogue) return Line;

    std::string strLine;
    m_File.getline(Line, 2000);                                                                              
    strLine = Line;
    
    if ((strLine.find("# ", 0)) != std::string::npos)
    {
        EndDialogue();

        return "null";
    }
    
    int Len = 0;

    int Param1 = strLine.find('/', 0);

    if (Param1 == std::string::npos)
    {
        EndDialogue();

        return "null";
    }


    m_MapName = strLine.substr(0, Param1);

    Len += Param1;

    int Param2 = strLine.find('/', Param1 + 1);
    std::string DialogueType = strLine.substr(Param1 + 1, Param2 - Param1 - 1);
    if (DialogueType == "dialogue")
        m_Type = DialogueType::EDialogue;
    else
        m_Type = DialogueType::ELine;

    Len += Param2;

    int Param3 = strLine.find('/', Param2 + 1);
    m_DialogueName = strLine.substr(Param2 + 1, Param3 - Param2 - 1);

    int Param4 = strLine.find('/', Param3 + 1);
    m_Speaker = strLine.substr(Param3 + 1, Param4 - Param3 - 1);

    int Param5 = strLine.find('/', Param4 + 1);
    m_DialogueNum1 = atoi(strLine.substr(Param4 + 1, Param5 - Param4 - 1).c_str());

    int Param6 = strLine.find('/', Param5 + 1);
    m_DialogueNum2 = atoi(strLine.substr(Param5 + 1, Param6 - Param5 - 1).c_str());
   
    /*m_File.getline(Line, 256);*/
   
    int StrStartIdx = strLine.find(',,', 0) + 1;
    int StrEndIdx = 0;

    if (strLine.at(StrStartIdx + 1) == '\"')
    {
        StrStartIdx += 1;
        StrEndIdx = strLine.find('\"', StrStartIdx + 1);
    }
    else
        StrEndIdx = strLine.find(',', StrStartIdx + 1);
    strLine = strLine.substr(StrStartIdx + 1, StrEndIdx - StrStartIdx - 1);

    return strLine;
}
