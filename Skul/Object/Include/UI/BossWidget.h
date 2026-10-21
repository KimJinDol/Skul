#pragma once
#include "UI\WidgetWindow.h"
#include "UI/Text.h"
#include "UI/ProgressBar.h"
#include "UI/Button.h"
#include "UI/NumberWidget.h"
#include "UI/Image.h"

class CBossWidget :
    public CWidgetWindow
{
public:
    CBossWidget();
    CBossWidget(const CBossWidget& widget);
    virtual ~CBossWidget();

private:
    class CAICharacter* m_pOwnerAI;

    CSharedPtr<CImage>   m_BackgroundImage;

    CSharedPtr<CText>   m_NameText;
    CSharedPtr<CText>   m_SubNameText;
    CSharedPtr<CProgressBar>    m_HPBar;
    CSharedPtr<CProgressBar>    m_DamageBar;

    CSharedPtr<CText>   m_UnderNameText;
    CSharedPtr<CText>   m_UnderSubNameText;

    // Fade In Under Info
    Vector3 m_TextColor;
    float   m_FadeInDuration;
    float   m_UnderInfoDuration;
    float   m_CurShowInfoTime;

    bool    m_bShowUnderName;

    bool    m_bOnChangeHP;
    float   m_DamageChangeWaitTerm;
    float   m_CurDamageChangeWaitTerm;
    float   m_DamageLerpSpeed;

    float   m_CameraAreaRatio;

public:
    void OnBossHealthWidget(bool Value);
    void OnBossInfoWidget(bool Value, float FadeIn, float Duration);
    void OffBossInfoWidget();

private:
    void BossInfoOpacityAnimation(float DeltaTime);

public:
    void SetOwnerAI(CAICharacter* Owner);
    void SetBossName(const TCHAR* Name);
    void SetSubName(const TCHAR* Name);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();

public:
    void ClickCallback();
};

