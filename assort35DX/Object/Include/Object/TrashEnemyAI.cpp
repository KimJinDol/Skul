#include "TrashEnemyAI.h"
#include "Scene\Scene.h"
#include "../UI/TrashEnemyWidget.h"
#include "Scene/Viewport.h"
#include "Component/HPComponent.h"
#include "AITask_Scout.h"
#include "AITask_Stun.h"
#include "Component\WidgetComponent.h"
#include "Effect.h"
#include "Scene\SceneResource.h"

CTrashEnemyAI::CTrashEnemyAI()  :
    m_PushOutMove(30.f)
{
}

CTrashEnemyAI::CTrashEnemyAI(const CTrashEnemyAI& obj)
{
}

CTrashEnemyAI::~CTrashEnemyAI()
{
}

void CTrashEnemyAI::CheckTargetInAttackRange()
{
}

void CTrashEnemyAI::Attack()
{
}

void CTrashEnemyAI::AttackCheck()
{
}

void CTrashEnemyAI::OffAttackCollision()
{
}

void CTrashEnemyAI::AttackEnd()
{
}

void CTrashEnemyAI::StanbyEnd()
{
}

void CTrashEnemyAI::StunEnd()
{
}

void CTrashEnemyAI::Die()
{
    CAICharacter::Die();

    CEffect* Effect = m_pScene->SpawnObject<CEffect>("TrashEnemyDieEffect");
    Effect->SetWorldScale(676.f, 200.f, 1.f);
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("TrashEnemyDieEffect");
    Vector2 Pos = m_Body->GetInfo().Center;
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    Effect->SetWorldPos(Pos.x, Pos.y, 0.f);
    Effect->SetAutoDestroy();

    m_Body->SetCollisionProfile("Ragdoll");

    Active(false);
}

float CTrashEnemyAI::TakeDamage(float Damage, CGameObject* DamageObject)
{
    if(GetWorldPos().x < DamageObject->GetWorldPos().x)
        m_pAIController->SetFloatData("PushOutMove", GetWorldPos().x - m_PushOutMove);
    else
        m_pAIController->SetFloatData("PushOutMove", GetWorldPos().x + m_PushOutMove);

    m_pAIController->SetBoolData("IsStun", true);
    
    return CAICharacter::TakeDamage(Damage, DamageObject);
}

void CTrashEnemyAI::Start()
{
    CAICharacter::Start();
}

bool CTrashEnemyAI::Init()
{
    if (!CAICharacter::Init())
        return false;

    // Load Animation
    {
        CSceneResource* SceneResource = m_pScene->GetResource();
        SceneResource->LoadTexture("Effect_Altas", TEXT("Effect/Effect_Altas.png"));

        SceneResource->CreateAnimationSequence2D("TrashEnemyDieEffect", 0.3f, "Effect_Altas", 0, 23, 5, Vector2(200.f, 200.f));
    }

    CreateAI<CAITask_Scout>("Scout");
    CreateAI<CAITask_Stun>("Stun");


    m_WidgetComponent = CreateSceneComponent<CWidgetComponent>("WidgetComponent");
    m_WidgetComponent->SetRelativePos(-100.f, 50.f, 0.f);
    m_WidgetComponent->SetRelativeScale(200.f, 120.f, 1.f);
    m_Sprite->AddChild(m_WidgetComponent);
    m_Widget = m_WidgetComponent->CreateWidget<CTrashEnemyWidget>("TrashEnemyWidget");
    m_Widget->SetOwnerAI(this);

    return true;
}

void CTrashEnemyAI::Update(float DeltaTime)
{
    CAICharacter::Update(DeltaTime);
}

void CTrashEnemyAI::PostUpdate(float DeltaTime)
{
    CAICharacter::PostUpdate(DeltaTime);
}

void CTrashEnemyAI::Collision(float DeltaTime)
{
    CAICharacter::Collision(DeltaTime);
}

void CTrashEnemyAI::Render(float DeltaTime)
{
    CAICharacter::Render(DeltaTime);
}

CTrashEnemyAI* CTrashEnemyAI::Clone() const
{
    return nullptr;
}

void CTrashEnemyAI::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
}

void CTrashEnemyAI::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
}
