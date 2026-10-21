#include "WarriorAI.h"
#include "Component/HPComponent.h"
#include "../Animation2D/WarriorAnimation.h"
#include "Scene/Scene.h"
#include "Scene\SceneResource.h"
#include "AISelectWarrior.h"
#include "../UI/TrashEnemyWidget.h"
#include "Skul.h"

CWarriorAI::CWarriorAI()
{
    m_AttackDistance = 30.f;
    m_TraceDistance = 500.f;
    m_StanbyTime = 1.f;
}

CWarriorAI::CWarriorAI(const CWarriorAI& obj)
{
}

CWarriorAI::~CWarriorAI()
{
}

void CWarriorAI::CheckTargetInAttackRange()
{
}

void CWarriorAI::Attack()
{
    if (m_bIsAttack) return;

    m_bIsAttack = true;
    m_Anim->SetAttack(true);

    m_pAIController->SetBoolData("IsAttack", true);
}

void CWarriorAI::AttackCheck()
{
    m_AttackCollision->Enable(true);
}

void CWarriorAI::OffAttackCollision()
{
    m_AttackCollision->Enable(false);
}

void CWarriorAI::AttackEnd()
{
    m_bIsAttack = false;
    m_bIsStanby = true;
    m_Anim->SetAttack(false);
    m_AttackCollision->Enable(false);
    m_pAIController->SetBoolData("IsAttack", false);
}

void CWarriorAI::StanbyEnd()
{
    m_bIsStanby = false;
}

void CWarriorAI::StunEnd()
{
    m_Anim->SetHit(false);
    m_pAIController->SetBoolData("IsStun", false);
}

void CWarriorAI::Die()
{
    CTrashEnemyAI::Die();

    m_pAIController->SetEnable(false);
    m_Anim->SetDie(true);
    //m_Widget->Enable(false);
}

void CWarriorAI::Start()
{
    CTrashEnemyAI::Start();

    CGameObject* Player = m_pScene->GetSceneMode()->GetPlayer();

    assert(Player);

    m_pAIController->SetGameObjectData("Target", Player);
    m_pAIController->SetFloatData("AttackDistance", m_AttackDistance);
    m_pAIController->SetFloatData("TraceDistance", m_TraceDistance);
    m_pAIController->SetFloatData("StanbyTime", m_StanbyTime);
    m_pAIController->SetBoolData("IsStanby", false);
    m_pAIController->SetBoolData("IsAttack", false);
}

bool CWarriorAI::Init()
{
    if (!CTrashEnemyAI::Init())
        return false;

    // Load Animation
    {
        CSceneResource* SceneResource = m_pScene->GetResource();
        SceneResource->LoadTexture("WarriorAltas", TEXT("Enemy/Warrior/WarriorAtlas.png"));

        SceneResource->CreateAnimationSequence2D("WarriorIdle", 0.4f, "WarriorAltas", 0, 0, 4, Vector2(300.f, 300.f));
        SceneResource->CreateAnimationSequence2D("WarriorRun", 0.7f, "WarriorAltas", 0, 1, 7, Vector2(300.f, 300.f));
        SceneResource->CreateAnimationSequence2D("WarriorAttack", 0.5f, "WarriorAltas", 0, 2, 5, Vector2(300.f, 300.f));
        SceneResource->CreateAnimationSequence2D("WarriorHit", 0.4f, "WarriorAltas", 0, 3, 1, Vector2(300.f, 300.f));
        SceneResource->CreateAnimationSequence2D("WarriorHit2", 0.4f, "WarriorAltas", 0, 4, 1, Vector2(300.f, 300.f));
        SceneResource->CreateAnimationSequence2D("WarriorDead", 0.4f, "WarriorAltas", 0, 5, 1, Vector2(300.f, 300.f));
    }

    SetMoveSpeed(150.f);

    m_HPComponent->SetMaxHp(100.f);

    m_Sprite->CreateAnimation2D<CWarriorAnimation>();
    m_Anim = (CWarriorAnimation*)m_Sprite->GetAnimation2D();

    m_Anim->SetFrameEndFunction<CWarriorAI>("WarriorHit", this, &CWarriorAI::StunEnd);
    m_Anim->SetFrameEndFunction<CWarriorAI>("WarriorHit2", this, &CWarriorAI::StunEnd);
    m_Anim->SetFrameEndFunction<CWarriorAI>("WarriorAttack", this, &CWarriorAI::AttackEnd);
    m_Anim->SetNotifyFunction<CWarriorAI>("WarriorAttack", 2, this, &CWarriorAI::AttackCheck);
    m_Anim->SetNotifyFunction<CWarriorAI>("WarriorAttack", 4, this, &CWarriorAI::OffAttackCollision);
    
    m_Body->SetExtent(30.f, 40.f);
    m_Body->SetCollisionProfile("Monster");
    m_Body->AddCollisionOverlapCallbackFunction<CWarriorAI>(Collision_State::Begin, this, &CWarriorAI::CollisionBeginOverlap);
    m_Body->AddCollisionBlockCallbackFunction<CWarriorAI>(Collision_State::Begin, this, &CWarriorAI::CollisionBeginBlock);

    m_AttackCollision = CreateSceneComponent<CColliderBox2D>("AttackCollision");
    m_Sprite->AddChild(m_AttackCollision);
    m_AttackCollision->SetPivot(0.5f, 0.f, 0.f);
    m_AttackCollision->SetExtent(50.f, 50.f);
    m_AttackCollision->SetRelativePos(20.f, 20.f, 0.f);
    m_AttackCollision->SetCollisionProfile("MonsterAttack");
    m_AttackCollision->AddCollisionOverlapCallbackFunction<CWarriorAI>(Collision_State::Begin, this, &CWarriorAI::CollisionBeginOverlap);
    m_AttackCollision->AddCollisionBlockCallbackFunction<CWarriorAI>(Collision_State::Begin, this, &CWarriorAI::CollisionBeginBlock);
    m_AttackCollision->Enable(false);

    CreateAISelect<CAISelectWarrior>("WarriorSelect");

    m_AD = 5.f;

    return true;
}

void CWarriorAI::Update(float DeltaTime)
{
    CTrashEnemyAI::Update(DeltaTime);

    if (GetMove().x > 0 && m_View == Object_View::Left)
        m_AttackCollision->RelativePosFlip(true, false, false);

    if (GetMove().x < 0 && m_View == Object_View::Right)
        m_AttackCollision->RelativePosFlip(true, false, false);
}

void CWarriorAI::PostUpdate(float DeltaTime)
{
    CTrashEnemyAI::PostUpdate(DeltaTime);
}

void CWarriorAI::Collision(float DeltaTime)
{
    CTrashEnemyAI::Collision(DeltaTime);
}

void CWarriorAI::Render(float DeltaTime)
{
    CTrashEnemyAI::Render(DeltaTime);
}

CWarriorAI* CWarriorAI::Clone() const
{
    return nullptr;
}

void CWarriorAI::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    CAICharacter::CollisionBeginOverlap(Result, Collider);
}

void CWarriorAI::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
    CAICharacter::CollisionBeginBlock(Result, Collider);
}

float CWarriorAI::TakeDamage(float Damage, CGameObject* DamageObject)
{
    m_Anim->SetHit(true);

    AttackEnd();

   return CTrashEnemyAI::TakeDamage(Damage, DamageObject);
}
