#include "TrashEnemyWidget.h"
#include "Component/HPComponent.h"
#include "AICharacter.h"
#include "BasicMath.h"

CTrashEnemyWidget::CTrashEnemyWidget() :
    m_DamageLerpSpeed(5.f),
    m_bOnChangeHP(false),
    m_DamageChangeWaitTerm(0.2f),
    m_CurDamageChangeWaitTerm(0.f)
{
}

CTrashEnemyWidget::CTrashEnemyWidget(const CTrashEnemyWidget& widget)
{
}

CTrashEnemyWidget::~CTrashEnemyWidget()
{
}

void CTrashEnemyWidget::SetOwnerAI(CAICharacter* Owner)
{
    m_pOwnerAI = Owner;
}

void CTrashEnemyWidget::Start()
{
    CWidgetWindow::Start();
}

bool CTrashEnemyWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    CImage* HPBarBackground = CreateWidget<CImage>("HPBar_Background");
    HPBarBackground->SetZOrder(1);
    HPBarBackground->SetTexture("EnemyHealthBar_Back", TEXT("UI/EnemyHealthBar_Back.png"));
    //HPBarBackground->SetStartEndUV(0, 0, Vector2(612.f, 200.f));
    HPBarBackground->SetPos(50.f, -75.f);
    HPBarBackground->SetSize(100.f, 12.f);

    m_DamageBar = CreateWidget<CProgressBar>("DamageBar");
    m_DamageBar->SetZOrder(2);
    m_DamageBar->SetBackTint(0.f, 0.f, 0.f, 0.f);
    m_DamageBar->SetPos(56.f, -73.f);
    m_DamageBar->SetSize(88.f, 8.f);
    m_DamageBar->SetTexture("TrashEnemy_DamageBar", TEXT("UI/EnemyHealthBar_Damage.png"));

    m_HPBar = CreateWidget<CProgressBar>("HPBar");
    m_HPBar->SetZOrder(3);
    m_HPBar->SetBackTint(0.f, 0.f, 0.f, 0.f);
    m_HPBar->SetPos(56.f, -73.f);
    m_HPBar->SetSize(88.f, 8.f);
    m_HPBar->SetTexture("TrashEnemy_HPBar", TEXT("UI/EnemyHealthBar.png"));



    return true;
}

void CTrashEnemyWidget::Update(float DeltaTime)
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
}

void CTrashEnemyWidget::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CTrashEnemyWidget::Render()
{
    CWidgetWindow::Render();
}

void CTrashEnemyWidget::ClickCallback()
{
}
