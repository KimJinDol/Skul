#pragma once
#include "UI\WidgetWindow.h"
#include "UI/Text.h"
#include "UI/ProgressBar.h"
#include "UI/Button.h"
#include "UI/NumberWidget.h"
#include "UI/Image.h"
#include "../Object/Skul.h"
#include "UI/CoolTime.h"


class CSkulWidget :
    public CWidgetWindow
{
public:
    CSkulWidget();
    CSkulWidget(const CSkulWidget& widget);
    virtual ~CSkulWidget();

private:
    CSkul* m_pSkul;

    CSharedPtr<CText>   m_NameText;
    CSharedPtr<CProgressBar>    m_HPBar;
    CSharedPtr<CProgressBar>    m_DamageBar;
    CSharedPtr<CNumberWidget>    m_NumberWidget;
    CSharedPtr<CImage>  m_FaceIcon;
    CSharedPtr<CImage>  m_SubFrameImage;
    CSharedPtr<CImage>  m_SpaceKeyImage;
    CSharedPtr<CImage>  m_SubFaceIcon;
    CSharedPtr<CImage>  m_Skill1Icon;
    CSharedPtr<CImage>  m_AKeyImage;
    CSharedPtr<CImage>  m_Skill2Icon;
    CSharedPtr<CImage>  m_Skill2Frame;
    CSharedPtr<CImage>  m_SKeyImage;
    CSharedPtr<CCoolTime>  m_SwitchCoolTime;
    CSharedPtr<CCoolTime>  m_Skill1CoolTime;
    CSharedPtr<CCoolTime>  m_Skill2CoolTime;

    bool    m_bOnChangeHP;
    float   m_DamageChangeWaitTerm;
    float   m_CurDamageChangeWaitTerm;
    float   m_DamageLerpSpeed;

public:
    void SetSkill1Enable(bool Enable = true);
    void SetSkill2Enable(bool Enable = true);
    void SetSwitchEnable(bool Enable = true);

public:
    void SetSkill1CoolRatio(float Ratio)
    {
        m_Skill2CoolTime->SetRatio(Ratio);
    }

    void SetSkul(CSkul* Skul)
    {
        m_pSkul = Skul;
    }
    void SwitchSkulState(ESkulState State);
    void SwitchSkulSubState(ESkulState State);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CSkulWidget* Clone();

public:
    void ClickCallback();
};

