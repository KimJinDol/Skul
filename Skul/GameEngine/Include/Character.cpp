#include "Character.h"
#include "Component/SceneComponent.h"
#include "Component/Transform.h"

CCharacter::CCharacter()
{
    m_View = Object_View::Right;
    m_bIsMoveable = true;
    m_bCanMoveOutsideWorld = false;
}

CCharacter::CCharacter(const CCharacter& obj)
{
}

CCharacter::~CCharacter()
{
}

void CCharacter::SetJumpMaxCount(int JumpCount)
{
    m_RootComponent->SetJumpMaxCount(JumpCount);
}

void CCharacter::SetJumpVelocity(float Velocity)
{
    m_RootComponent->SetJumpVelocity(Velocity);
}

bool CCharacter::IsJump() const
{
    return m_RootComponent->IsJump();
}

void CCharacter::Jump()
{
    m_RootComponent->Jump();
}

int CCharacter::GetCurrentJumpCount() const
{
    return m_RootComponent->GetCurrentJumpCount();
}

void CCharacter::Start()
{
    CGameObject::Start();
}

bool CCharacter::Init()
{
    CGameObject::Init();

    m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
    SetRootComponent(m_Sprite);
    SetPhysicsSimulate(true);
    SetPivot(0.5f, 0.5f, 0.f);
    m_Sprite->SetRelativeScale(170.f, 170.f, 1.f);


    m_BodyExtent = Vector2(50.f, 50.f);
    m_Body = CreateSceneComponent<CColliderBox2D>("Body");
    m_Sprite->AddChild(m_Body);
    m_Body->SetExtent(m_BodyExtent.x, m_BodyExtent.y);
    m_Body->SetPivot(0.5f, 0.f, 0.f);
    // m_Body->SetColliderPivot2D(0.5f, 0.f, 0.f);


    return true;
}

void CCharacter::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

}

void CCharacter::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    // Right
    if (GetMove().x > 0)
        m_View = Object_View::Right;
    else if (GetMove().x < 0)
        m_View = Object_View::Left;

}

void CCharacter::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CCharacter::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CCharacter* CCharacter::Clone()
{
    return new CCharacter(*this);
}
