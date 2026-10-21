#include "BossWidget.h"
#include "AICharacter.h"
#include "Component\HPComponent.h"
#include "BasicMath.h"
#include "Scene/Scene.h"
#include "../Object/Skul.h"
#include "Engine.h"

CBossWidget::CBossWidget() :
    m_DamageLerpSpeed(5.f),
    m_bOnChangeHP(false),
    m_DamageChangeWaitTerm(0.2f),
    m_CurDamageChangeWaitTerm(0.f),
    m_bShowUnderName(false)

{
    m_TextColor = Vector3(225.f, 208.f, 175.f);
    m_FadeInDuration = 1.f;
    m_UnderInfoDuration = 2.f;
    m_CurShowInfoTime = 0.f;

    m_CameraAreaRatio = 0.7f;
}

CBossWidget::CBossWidget(const CBossWidget& widget)
{
}

CBossWidget::~CBossWidget()
{
}

void CBossWidget::OnBossHealthWidget(bool Value)
{
    m_NameText->Enable(Value);
    m_SubNameText->Enable(Value);
    m_HPBar->Enable(Value);
    m_DamageBar->Enable(Value);
    m_BackgroundImage->Enable(Value);
}

void CBossWidget::OnBossInfoWidget(bool Value, float FadeIn, float Duration)
{
    m_bShowUnderName = Value;
    m_FadeInDuration = FadeIn;
    m_UnderInfoDuration = Duration;
    CSkul* Skul = (CSkul*)m_Scene->GetSceneMode()->GetPlayer();
    Skul->SetEnable(false, false, false);
}

void CBossWidget::OffBossInfoWidget()
{
    OnBossHealthWidget(true);
    m_bShowUnderName = false;
    m_pOwnerAI->SetEnableController(true);
    CSkul* Skul = (CSkul*)m_Scene->GetSceneMode()->GetPlayer();
    Skul->SetEnable();
}

void CBossWidget::BossInfoOpacityAnimation(float DeltaTime)
{
    m_CurShowInfoTime += DeltaTime;

    float Opacity = 0.f;
    if (m_CurShowInfoTime <= m_FadeInDuration)
    {
        Opacity = BasicMath::Clamp<float>(m_CurShowInfoTime / m_FadeInDuration, 0.f, 1.f);
        m_UnderNameText->SetOpacity(Opacity);
        m_UnderSubNameText->SetOpacity(Opacity);
        CEngine::GetInst()->SetCameraAreaRatio(1.f - (Opacity * (1.f -  m_CameraAreaRatio)));
    }
    else if (m_CurShowInfoTime - m_FadeInDuration > m_UnderInfoDuration)
    {
        Opacity = BasicMath::Clamp<float>(1.f - ((m_CurShowInfoTime - m_FadeInDuration - m_UnderInfoDuration) / m_FadeInDuration), 0.f, 1.f);
        m_UnderNameText->SetOpacity(Opacity);
        m_UnderSubNameText->SetOpacity(Opacity);
        CEngine::GetInst()->SetCameraAreaRatio(1.f - (Opacity * (1.f - m_CameraAreaRatio)));

        if (Opacity <= 0.f)
            OffBossInfoWidget();
    }
}

void CBossWidget::SetOwnerAI(CAICharacter* Owner)
{
    m_pOwnerAI = Owner;
}

void CBossWidget::SetBossName(const TCHAR* Name)
{
    m_NameText->SetText(Name);
    m_UnderNameText->SetText(Name);
}

void CBossWidget::SetSubName(const TCHAR* Name)
{
    m_SubNameText->SetText(Name);
    m_UnderSubNameText->SetText(Name);
}

bool CBossWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    m_BackgroundImage = CreateWidget<CImage>("HPBar_Background");
    m_BackgroundImage->SetTexture("UI_Atals02", TEXT("UI/UI_Atlas02.png"));
    m_BackgroundImage->SetStartEndUV(0, 0, Vector2(612.f, 200.f));
    m_BackgroundImage->SetPos(350.f, 517.f);
    m_BackgroundImage->SetSize(612.f, 200.f);

    m_NameText = CreateWidget<CText>("Name");
    m_NameText->SetFont("SkulFont");
    m_NameText->SetPos(606.f, 588.f);
    m_NameText->SetFontSize(18.f);
    m_NameText->SetColor(m_TextColor);
    m_NameText->SetAlignH(TEXT_ALIGN_H::Center);


    m_SubNameText = CreateWidget<CText>("SubName");
    m_SubNameText->SetFont("SkulFont");
    m_SubNameText->SetPos(606.f, 535.f);
    m_SubNameText->SetFontSize(12.f);
    m_SubNameText->SetColor(m_TextColor);
    m_SubNameText->SetAlignH(TEXT_ALIGN_H::Center);

    m_DamageBar = CreateWidget<CProgressBar>("DamageBar");
    m_DamageBar->SetBackTint(0.f, 0.f, 0.f, 0.f);
    m_DamageBar->SetPos(427.f, 639.f);
    m_DamageBar->SetSize(456.f, 18.f);
    m_DamageBar->SetTexture("Boss_DamageBar", TEXT("UI/BossHealthBar_Damage.png"));

    m_HPBar = CreateWidget<CProgressBar>("HPBar");
    m_HPBar->SetBackTint(0.f, 0.f, 0.f, 0.f);
    m_HPBar->SetPos(427.f, 639.f);
    m_HPBar->SetSize(456.f, 18.f);
    m_HPBar->SetTexture("Boss_HPBar", TEXT("UI/BossHealthBar_FirstPhase.png"));


    m_UnderNameText = CreateWidget<CText>("UnderName");
    m_UnderNameText->SetFont("SkulFont");
    m_UnderNameText->SetPos(1200.f, 0.f);
    m_UnderNameText->SetFontSize(35.f);
    m_UnderNameText->SetColor(m_TextColor);
    m_UnderNameText->SetOpacity(0.f);


    m_UnderSubNameText = CreateWidget<CText>("UnderSubName");
    m_UnderSubNameText->SetFont("SkulFont");
    m_UnderSubNameText->SetPos(1200.f, -50.f);
    m_UnderSubNameText->SetFontSize(22.f);
    m_UnderSubNameText->SetColor(m_TextColor);
    m_UnderSubNameText->SetOpacity(0.f);
    
    OnBossHealthWidget(false);
    return true;
}

void CBossWidget::Start()
{
    CWidgetWindow::Start();
}

void CBossWidget::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);

    float HPPercent = ((CHPComponent*)(m_pOwnerAI->FindObjectComponent("HPComponent")))->GetHpRatio();

    if (m_DamageBar->GetPercent() != HPPercent)
        m_bOnChangeHP = true;

    m_HPBar->SetPercent(HPPercent);

    if (m_bOnChangeHP)
    {
        m_CurDamageChangeWaitTerm += DeltaTime;

        if (m_CurDamageChangeWaitTerm >= m_DamageChangeWaitTerm)
        {
            float DamagePercent = BasicMath::FInterpTo(m_DamageBar->GetPercent(), m_HPBar->GetPercent(), DeltaTime, m_DamageLerpSpeed);
            m_DamageBar->SetPercent(DamagePercent);

            if (DamagePercent - HPPercent <= SMALL_NUMBER)
            {
                m_DamageBar->SetPercent(HPPercent);
                m_bOnChangeHP = false;

                m_CurDamageChangeWaitTerm = 0.f;
            }
        }
    }


    if (m_bShowUnderName)
        BossInfoOpacityAnimation(DeltaTime);
}
void CBossWidget::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CBossWidget::Render()
{
    CWidgetWindow::Render();
}

void CBossWidget::ClickCallback()
{
}
