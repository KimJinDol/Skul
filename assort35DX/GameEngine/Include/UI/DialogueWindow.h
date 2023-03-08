#pragma once
#include "WidgetWindow.h"
#include "Text.h"
#include "Image.h"

class CDialogueWindow :
    public CWidgetWindow
{
public:
    CDialogueWindow();
    CDialogueWindow(const CDialogueWindow& widget);
    virtual ~CDialogueWindow();

private:
    CSharedPtr<CText>   m_NameText;
    CSharedPtr<CText>   m_Text;
    CSharedPtr<CImage>  m_NameImageLeft;
    CSharedPtr<CImage>  m_NameImageMiddle;
    CSharedPtr<CImage>  m_NameImageRight;
    CSharedPtr<CImage>  m_BackgroundImage;


public:
    void SetNextDialogue(float DeltaTime);
    void SetNameText(std::string& Name);
    void SetDialogueText(std::string& Text);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();

public:
    void ClickCallback();
};

