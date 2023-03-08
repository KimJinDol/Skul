#include "KumaShield.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Animation2D/KumaShieldAnimation.h"

CKumaShield::CKumaShield()
{
}

CKumaShield::CKumaShield(const CKumaShield& obj)
{
}

CKumaShield::~CKumaShield()
{
}

void CKumaShield::OnShield()
{
    Enable(true);

    m_Anim->SetOn(true);
}

void CKumaShield::OffShield()
{
    Enable(true);

    m_Anim->SetOn(false);
}

void CKumaShield::Start()
{
    CGameObject::Start();

    m_Anim->SetOn(true);
}

bool CKumaShield::Init()
{
    if (!CGameObject::Init())
        return false;



    return true;
}

void CKumaShield::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CKumaShield::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CKumaShield::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CKumaShield::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CKumaShield* CKumaShield::Clone()
{
    return nullptr;
}

void CKumaShield::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
}

void CKumaShield::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
}
