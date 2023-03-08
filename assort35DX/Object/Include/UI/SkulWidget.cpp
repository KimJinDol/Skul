#include "SkulWidget.h"
#include "UI/Image.h"
#include "../Object/Skul.h"
#include "UI/CoolTime.h"
#include "Engine.h"
#include "Timer.h"
#include "Component\HPComponent.h"
#include "BasicMath.h"

CSkulWidget::CSkulWidget()	:
	m_DamageLerpSpeed(5.f),
	m_bOnChangeHP(false),
	m_DamageChangeWaitTerm(0.2f),
	m_CurDamageChangeWaitTerm(0.f)
{
}

CSkulWidget::CSkulWidget(const CSkulWidget& widget)
{
}

CSkulWidget::~CSkulWidget()
{
}

void CSkulWidget::SetSkill1Enable(bool Enable)
{
    m_Skill1CoolTime->Enable(Enable);
    m_Skill1Icon->Enable(Enable);
}

void CSkulWidget::SetSkill2Enable(bool Enable)
{
    m_Skill2CoolTime->Enable(Enable);
    m_Skill2Icon->Enable(Enable);
	m_Skill2Frame->Enable(Enable);
}

void CSkulWidget::SetSwitchEnable(bool Enable)
{
	m_SubFaceIcon->Enable(Enable);
	m_SubFrameImage->Enable(Enable);
	m_SpaceKeyImage->Enable(Enable);
}

void CSkulWidget::SwitchSkulState(ESkulState State)
{
	switch (State)
	{
	case ESkulState::Default:
		m_FaceIcon->SetStartEndUV(0, 0, Vector2(90.f, 90.f));
		m_Skill1Icon->SetStartEndUV(0, 2, Vector2(90.f, 90.f));
		m_Skill2Icon->SetStartEndUV(0, 3, Vector2(90.f, 90.f));
		break;
	case ESkulState::NoHead:
		//m_SubFaceIcon->Enable(false);
		break;
	case ESkulState::ChiefGuard:
		m_FaceIcon->SetStartEndUV(1, 0, Vector2(90.f, 90.f));
		m_Skill1Icon->SetStartEndUV(1, 2, Vector2(90.f, 90.f));
		m_Skill2Icon->SetStartEndUV(1, 3, Vector2(90.f, 90.f));
		break;
	}
}

void CSkulWidget::SwitchSkulSubState(ESkulState State)
{
	switch (State)
	{
	case ESkulState::None:
		m_SubFaceIcon->Enable(false);
		m_SubFrameImage->Enable(false);
		m_SpaceKeyImage->Enable(false);
		break;
	case ESkulState::Default:
		m_SubFaceIcon->Enable(true);
		m_SubFrameImage->Enable(true);
		m_SpaceKeyImage->Enable(true);
		m_SubFaceIcon->SetStartEndUV(0, 1, Vector2(90.f, 90.f));
		break;
	case ESkulState::NoHead:
		m_SubFaceIcon->Enable(true);
		m_SubFrameImage->Enable(true);
		m_SpaceKeyImage->Enable(true);
		m_SubFaceIcon->Enable(false);
		break;
	case ESkulState::ChiefGuard:
		m_SubFaceIcon->Enable(true);
		m_SubFrameImage->Enable(true);
		m_SpaceKeyImage->Enable(true);
		m_SubFaceIcon->SetStartEndUV(1, 1, Vector2(90.f, 90.f));
		break;
	}
}

bool CSkulWidget::Init()
{
	CWidgetWindow::Init();


	CImage* Image = CreateWidget<CImage>("Background");
	Image->SetTexture("Player_Normal_Frame", TEXT("UI/Player_Normal_Frame.png"));
	Image->SetPos(0.f, 0.f);
	Image->SetSize(336.f, 132.f);
	
	//m_NameText = CreateWidget<CText>("Name");
	//
	//m_NameText->SetText(TEXT("Test"));
	//m_NameText->SetPos(0.f, 20.f);
	//m_NameText->SetSize(200.f, 50.f);
	//m_NameText->SetFontSize(30.f);
	//m_NameText->SetShadowEnable(true);
	//m_NameText->SetShadowColor(1.f, 1.f, 0.f);
	//m_NameText->SetShadowAlphaEnable(true);
	//m_NameText->SetShadowOpacity(0.5f);
	//m_NameText->SetAlignH(TEXT_ALIGN_H::Center);
	//m_NameText->SetAlignV(TEXT_ALIGN_V::Bottom);
	m_FaceIcon = CreateWidget<CImage>("FaceIcon");
	m_FaceIcon->SetTexture("UI_Atlas", TEXT("UI/UI_Atlas.png"));
	m_FaceIcon->SetStartEndUV(0, 0, Vector2(90.f, 90.f));
	m_FaceIcon->SetPos(13.f, 40.f);
	m_FaceIcon->SetSize(90.f, 90.f);

	m_SubFrameImage = CreateWidget<CImage>("SubFrame");
	m_SubFrameImage->SetTexture("Player_Subskull_Frame", TEXT("UI/Player_Subskull_Frame.png"));
	m_SubFrameImage->SetPos(9.f, 17.f);
	m_SubFrameImage->SetSize(54.f, 56.f);

	m_SwitchCoolTime = CreateWidget<CCoolTime>("SwitchCool");
	m_SwitchCoolTime->SetTexture("CoolTimeImage", TEXT("UI/CoolTimeImage.png"));
	m_SwitchCoolTime->SetPos(13.f, 42.f);
	m_SwitchCoolTime->SetSize(87.f, 87.f);
	m_SwitchCoolTime->SetColorTint(1.f, 1.f, 1.f, 0.8f);
	m_SwitchCoolTime->SetCircle(1);

	m_SubFaceIcon = CreateWidget<CImage>("PrevFaceIcon");
	m_SubFaceIcon->SetTexture("UI_Atlas", TEXT("UI/UI_Atlas.png"));
	m_SubFaceIcon->SetStartEndUV(0, 1, Vector2(90.f, 90.f));
	m_SubFaceIcon->SetPos(-9.f, 0.f);
	m_SubFaceIcon->SetSize(90.f, 90.f);

	m_SpaceKeyImage = CreateWidget<CImage>("SpaceKeyImage");
	m_SpaceKeyImage->SetTexture("UI_Space", TEXT("UI/UI_Space.png"));
	m_SpaceKeyImage->SetPos(7.f, 8.f);
	m_SpaceKeyImage->SetSize(60.f, 22.f);

	m_Skill1Icon = CreateWidget<CImage>("Skill1_Icon");
	m_Skill1Icon->SetTexture("UI_Atlas", TEXT("UI/UI_Atlas.png"));
	m_Skill1Icon->SetStartEndUV(0, 2, Vector2(90.f, 90.f));
	m_Skill1Icon->SetPos(95.f, 33.f);
	m_Skill1Icon->SetSize(90.f, 90.f);

	m_Skill1CoolTime = CreateWidget<CCoolTime>("Skill1Cool");
	m_Skill1CoolTime->SetTexture("CoolTimeImage", TEXT("UI/CoolTimeImage.png"));
	m_Skill1CoolTime->SetPos(115.f, 53.f);
	m_Skill1CoolTime->SetSize(50.f, 50.f);
	m_Skill1CoolTime->SetColorTint(1.f, 1.f, 1.f, 0.8f);

    m_Skill2Frame = CreateWidget<CImage>("Skill2Frame");
    m_Skill2Frame->SetTexture("Skill2FrameImage", TEXT("UI/Player_Skill2_Frame.png"));
    m_Skill2Frame->SetPos(173.f, 50.f);
    m_Skill2Frame->SetSize(56.f, 56.f);

    m_Skill2Icon = CreateWidget<CImage>("Skill2_Icon");
    m_Skill2Icon->SetTexture("UI_Atlas", TEXT("UI/UI_Atlas.png"));
    m_Skill2Icon->SetStartEndUV(0, 3, Vector2(90.f, 90.f));
    m_Skill2Icon->SetPos(156.f, 33.f);
    m_Skill2Icon->SetSize(90.f, 90.f);

    m_Skill2CoolTime = CreateWidget<CCoolTime>("Skill2Cool");
    m_Skill2CoolTime->SetTexture("CoolTimeImage", TEXT("UI/CoolTimeImage.png"));
    m_Skill2CoolTime->SetPos(176.f, 53.f);
    m_Skill2CoolTime->SetSize(50.f, 50.f);
    m_Skill2CoolTime->SetColorTint(1.f, 1.f, 1.f, 0.8f);

	m_DamageBar = CreateWidget<CProgressBar>("DamageBar");
	m_DamageBar->SetPos(88.f, 25.f);
	m_DamageBar->SetSize(233.f, 18.f);
	m_DamageBar->SetTexture("DamageBar", TEXT("UI/Player_HealthBar_Damage.png"));
	m_DamageBar->SetBackTint(0.f, 0.f, 0.f, 0.f);

	m_HPBar = CreateWidget<CProgressBar>("HPBar");
	m_HPBar->SetPos(88.f, 25.f);  
	m_HPBar->SetSize(233.f, 18.f);
	m_HPBar->SetTexture("HPBar", TEXT("UI/Player_HealthBar.png"));
	m_HPBar->SetBackTint(0.f, 0.f, 0.f, 0.f);

	// m_NumberWidget = CreateWidget<CNumberWidget>("Number");
	// 
	// m_NumberWidget->SetPos(0.f, 70.f);
	// m_NumberWidget->SetNumberSize(50.f, 50.f);
	// m_NumberWidget->SetTexture("Number", TEXT("Number.png"));
	// m_NumberWidget->SetNumber(1234);
	// m_NumberWidget->SetFrameCount(10, 2);

	return true;
}

void CSkulWidget::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	if (!m_pSkul) return;

	ESkulState State = m_pSkul->GetSkulState();
	SwitchSkulState(State);
	SwitchSkulSubState(m_pSkul->GetSkulSubState());

	float Skill1Cool = 0.f;
	float Skill2Cool = 0.f;
	float SwitchCool = CEngine::GetInst()->GetTimer()->GetTimerRatio("SwitchCool");

	switch (State)
	{
	case ESkulState::Default:
		Skill1Cool = CEngine::GetInst()->GetTimer()->GetTimerRatio("Skill1Cool");
		Skill2Cool = CEngine::GetInst()->GetTimer()->GetTimerRatio("Skill2Cool");
		break;
	case ESkulState::NoHead:
		Skill1Cool = CEngine::GetInst()->GetTimer()->GetTimerRatio("Skill1Cool");
		Skill2Cool = CEngine::GetInst()->GetTimer()->GetTimerRatio("Skill2Cool");
		break;
	case ESkulState::ChiefGuard:
		Skill1Cool = CEngine::GetInst()->GetTimer()->GetTimerRatio("Skill1Cool_ChiefGuard");
		Skill2Cool = CEngine::GetInst()->GetTimer()->GetTimerRatio("Skill2Cool_ChiefGuard");
		break;
	}

	if (Skill1Cool != -1.f)
		m_Skill1CoolTime->SetRatio(Skill1Cool);
	else
		m_Skill1CoolTime->SetRatio(0.f);

	if (Skill2Cool != -1.f)
		m_Skill2CoolTime->SetRatio(Skill2Cool);
	else
		m_Skill2CoolTime->SetRatio(0.f);

    if (SwitchCool != -1.f)
        m_SwitchCoolTime->SetRatio(SwitchCool);
    else
        m_SwitchCoolTime->SetRatio(0.f);
	


	// HP ¹Ù Á¶Á¤
	{
		float HPPercent = ((CHPComponent*)(m_pSkul->FindObjectComponent("HPComponent")))->GetHpRatio();

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
	}
}

void CSkulWidget::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CSkulWidget::Render()
{
	CWidgetWindow::Render();
}

CSkulWidget* CSkulWidget::Clone()
{
	return new CSkulWidget(*this);
}

void CSkulWidget::ClickCallback()
{
}
