#include "ArcherAI.h"
#include "../Animation2D/ArcherAnimation.h"
#include "Component/HPComponent.h"
#include "Scene/Scene.h"
#include "Scene\SceneResource.h"
#include "../UI/TrashEnemyWidget.h"
#include "Skul.h"
#include "AISelectArcher.h"
#include "Effect.h"
#include "ArcherArrow.h"

CArcherAI::CArcherAI()
{
    m_AttackDistance = 30.f;
    m_TraceDistance = 500.f;
    m_StanbyTime = 2.f;
}

CArcherAI::CArcherAI(const CArcherAI& obj)
{
}

CArcherAI::~CArcherAI()
{
}

void CArcherAI::CheckTargetInAttackRange()
{
}

void CArcherAI::Attack()
{
    if (m_bIsAttack || m_pAIController->GetBoolData("IsAttack")) return;

    m_bIsAttack = true;
    m_Anim->SetAttack(true);

    m_pAIController->SetBoolData("IsAttack", true);

    
    CEffect* Effect = m_pScene->SpawnObject<CEffect>("ArcherAttackSign");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("ArcherAttackSign");
    if (m_View == Object_View::Right)
        Effect->SetPivot(0.f, 0.5f, 0.f);
    else
        Effect->SetPivot(1.f, 0.5f, 0.f);
    Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 60.f, 0.f);
    Effect->SetAutoDestroy();
}

void CArcherAI::SpawnArrow()
{
    CArcherArrow* Arrow = m_pScene->SpawnObject<CArcherArrow>("ArcherArrow");
    Arrow->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 60.f, 0.f);
    Arrow->SetView(m_View);
}

void CArcherAI::AttackEnd()
{
    m_bIsAttack = false;
    m_bIsStanby = true;
    m_pAIController->SetBoolData("IsStanby", true);
    m_Anim->SetAttack(false);
    m_pAIController->SetBoolData("IsAttack", false);
}

void CArcherAI::StanbyEnd()
{
    m_bIsStanby = false;
}

void CArcherAI::StunEnd()
{
    m_Anim->SetHit(false);
    m_pAIController->SetBoolData("IsStun", false);
}

void CArcherAI::Die()
{
    CTrashEnemyAI::Die();

    m_pAIController->SetEnable(false);
    m_Anim->SetDie(true);
    //m_Widget->Enable(false);
}

float CArcherAI::TakeDamage(float Damage, CGameObject* DamageObject)
{
    m_Anim->SetHit(true);

    AttackEnd();

    return CTrashEnemyAI::TakeDamage(Damage, DamageObject);
}

void CArcherAI::Start()
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

bool CArcherAI::Init()
{
    if (!CTrashEnemyAI::Init())
        return false;

    // Load Animation
    {
        CSceneResource* SceneResource = m_pScene->GetResource();
        SceneResource->LoadTexture("ArcherAltas", TEXT("Enemy/Archer/ArcherAtalss.png"));
        SceneResource->LoadTexture("AttackSign", TEXT("Effect/AttackSign.png"));

        SceneResource->CreateAnimationSequence2D("ArcherIdle", 0.4f, "ArcherAltas", 0, 0, 4, Vector2(400.f, 400.f));
        SceneResource->CreateAnimationSequence2D("ArcherRun", 0.7f, "ArcherAltas", 0, 1, 6, Vector2(400.f, 400.f));
        SceneResource->CreateAnimationSequence2D("ArcherAttack", 1.3f, "ArcherAltas", 0, 2, 13, Vector2(400.f, 400.f));
        SceneResource->CreateAnimationSequence2D("ArcherHit", 0.4f, "ArcherAltas", 0, 3, 1, Vector2(400.f, 400.f));
        SceneResource->CreateAnimationSequence2D("ArcherHit2", 0.4f, "ArcherAltas", 0, 4, 1, Vector2(400.f, 400.f));
        SceneResource->CreateAnimationSequence2D("ArcherDead", 0.4f, "ArcherAltas", 0, 5, 1, Vector2(400.f, 400.f));

        SceneResource->CreateAnimationSequence2D("ArcherAttackSign", 0.7f, "AttackSign", 0, 0, 11, Vector2(400.f, 60.f));

    }

    SetMoveSpeed(150.f);

    m_HPComponent->SetMaxHp(100.f);

    m_Sprite->CreateAnimation2D<CArcherAnimation>();
    m_Anim = (CArcherAnimation*)m_Sprite->GetAnimation2D();

    m_Anim->SetFrameEndFunction<CArcherAI>("ArcherHit", this, &CArcherAI::StunEnd);
    m_Anim->SetFrameEndFunction<CArcherAI>("ArcherHit2", this, &CArcherAI::StunEnd);
    m_Anim->SetNotifyFunction<CArcherAI>("ArcherAttack", 1, this, &CArcherAI::Attack);
    m_Anim->SetNotifyFunction<CArcherAI>("ArcherAttack", 8, this, &CArcherAI::SpawnArrow);
    m_Anim->SetFrameEndFunction<CArcherAI>("ArcherAttack", this, &CArcherAI::AttackEnd);

    m_Body->SetExtent(30.f, 40.f);
    m_Body->SetCollisionProfile("Monster");
    m_Body->AddCollisionOverlapCallbackFunction<CArcherAI>(Collision_State::Begin, this, &CArcherAI::CollisionBeginOverlap);
    m_Body->AddCollisionBlockCallbackFunction<CArcherAI>(Collision_State::Begin, this, &CArcherAI::CollisionBeginBlock);

    CreateAISelect<CAISelectArcher>("ArcherSelect");

    m_AD = 5.f;

    return true;
}

void CArcherAI::Update(float DeltaTime)
{
    CTrashEnemyAI::Update(DeltaTime);
}

void CArcherAI::PostUpdate(float DeltaTime)
{
    CTrashEnemyAI::PostUpdate(DeltaTime);
}

void CArcherAI::Collision(float DeltaTime)
{
    CTrashEnemyAI::Collision(DeltaTime);
}

void CArcherAI::Render(float DeltaTime)
{
    CTrashEnemyAI::Render(DeltaTime);
}

CArcherAI* CArcherAI::Clone() const
{
    return nullptr;
}

void CArcherAI::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    CAICharacter::CollisionBeginOverlap(Result, Collider);
}

void CArcherAI::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
    CAICharacter::CollisionBeginBlock(Result, Collider);
}
