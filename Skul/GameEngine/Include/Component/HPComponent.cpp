#include "HPComponent.h"
#include "../BasicMath.h"

CHPComponent::CHPComponent()    :
    m_Hp(100.f),
    m_MaxHp(100.f),
    m_bIsDie(false)
{
}

CHPComponent::~CHPComponent()
{
}

float CHPComponent::SetHP(float Hp)
{
    return m_Hp = BasicMath::Clamp<float>(Hp, 0.0f, m_MaxHp);
}

void CHPComponent::SetMaxHp(float Hp)
{
    m_MaxHp = Hp;
    m_Hp = m_MaxHp;
}

void CHPComponent::SetToMaxHp()
{
    m_Hp = m_MaxHp;
}

void CHPComponent::AddHp(float Hp)
{
    m_Hp = BasicMath::Clamp<float>(m_Hp + Hp, 0.0f, m_MaxHp);
}

void CHPComponent::AddMaxHp(float Hp)
{
    m_MaxHp += Hp;
    m_Hp += Hp;
}

float CHPComponent::GetHp() const
{
    return m_Hp;
}

float CHPComponent::GetMaxHp()  const
{
    return m_MaxHp;
}

float CHPComponent::GetHpRatio()    const
{
    return (m_Hp <= KINDA_SMALL_NUMBER) ? 0.0f : (m_Hp / m_MaxHp);
}

bool CHPComponent::IsDie()  const
{
    return m_bIsDie;
}

float CHPComponent::TakeDamage(float Damage)
{
    m_Hp = BasicMath::Clamp<float>(m_Hp - Damage, 0.0f, m_MaxHp);

    if (m_Hp <= KINDA_SMALL_NUMBER)
    {
        m_bIsDie = true;

        if (m_OnDieDelegate)
            m_OnDieDelegate();
    }

    return m_Hp;
}
