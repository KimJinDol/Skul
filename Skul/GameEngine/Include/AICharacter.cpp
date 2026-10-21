#include "AICharacter.h"
#include "Scene/Scene.h"
#include "Scene/SceneMode.h"
#include "AITask_Idle.h"
#include "AITask_Trace.h"
#include "AITask_Attack.h"
#include "Component\HPComponent.h"
#include "Timer.h"
#include "Engine.h"
#include "Scene/SceneResource.h"
#include "Effect.h"
#include "AIManager.h"

CAICharacter::CAICharacter()    :
    m_bIsAttack(false),
    m_bIsStanby(false),
    m_CurStanbyTime(0.f),
    m_AD(10.f)
{
    m_PostView = Object_View::Right;
}

CAICharacter::CAICharacter(const CAICharacter& obj)
{
}

CAICharacter::~CAICharacter()
{
    SAFE_DELETE(m_pAIController);
    CEngine::GetInst()->GetTimer()->RemoveAllTimer(this);
}

void CAICharacter::SetAIIntData(const std::string& Name, int Data)
{
    if (m_pAIController)
        m_pAIController->SetIntData(Name, Data);
}

void CAICharacter::SetAIFloatData(const std::string& Name, float Data)
{
    if (m_pAIController)
        m_pAIController->SetFloatData(Name, Data);
}

void CAICharacter::SetAIBoolData(const std::string& Name, bool Data)
{
    if (m_pAIController)
        m_pAIController->SetBoolData(Name, Data);
}

void CAICharacter::SetAIStringData(const std::string& Name, std::string Data)
{
    if (m_pAIController)
        m_pAIController->SetStringData(Name, Data);
}

void CAICharacter::SetAIGameObjectData(const std::string& Name, CGameObject* Data)
{
    if (m_pAIController)
        m_pAIController->SetGameObjectData(Name, Data);
}

bool CAICharacter::Init()
{
    if (!CCharacter::Init())
        return false;

    CSceneResource* SceneResource = m_pScene->GetResource();
    SceneResource->LoadTexture("Effect_Altas", TEXT("Effect/Effect_Altas.png"));

    SceneResource->CreateAnimationSequence2D("EnemyDeadEffect", 0.4f, "Effect_Altas", 0, 24, 6, Vector2(200.f, 200.f));


    m_HPComponent = CreateObjectComponent<CHPComponent>("HPComponent");
    m_HPComponent->SetDieDelegate<CAICharacter>(this, &CAICharacter::Die);

    CreateAIController<CAIController>();

    CreateAI<CAITask_Idle>("Idle");
    CreateAI<CAITask_Trace>("Trace");
    CreateAI<CAITask_Attack>("Attack");

    m_pAIController->SetBoolData("CanNewBehavior", true);

    return true;
}

void CAICharacter::Start()
{
    CCharacter::Start();

    SetAIGameObjectData("Target", m_pScene->GetSceneMode()->GetPlayer());
}

void CAICharacter::Update(float DeltaTime)
{
    if (m_bIsStanby)
    {
        m_CurStanbyTime += DeltaTime;
        if (m_CurStanbyTime >= m_StanbyTime)
        {
            m_bIsStanby = false;
            m_CurStanbyTime = 0.f;
            m_pAIController->SetBoolData("CanNewBehavior", true);
        }
    }

    m_pAIController->SetBoolData("IsStanby", m_bIsStanby);

    CCharacter::Update(DeltaTime);

    if (m_pAIController)
        m_pAIController->Run(DeltaTime);
}

void CAICharacter::PostUpdate(float DeltaTime)
{
    CCharacter::PostUpdate(DeltaTime);
}

void CAICharacter::Collision(float DeltaTime)
{
    CCharacter::Collision(DeltaTime);
    
    m_PostView = m_View;
}

void CAICharacter::Render(float DeltaTime)
{
    CCharacter::Render(DeltaTime);
}

CAICharacter* CAICharacter::Clone() const
{
    return nullptr;
}

void CAICharacter::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    if (Collider->GetProfile()->Name == "MonsterAttack")
    {
        IHitable* Hitable = Cast<IHitable*>(Result.DestObject);

        if (Hitable)
            Hitable->TakeDamage(m_AD, this);
    }
}

void CAICharacter::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
}

void CAICharacter::SetEnableController(bool Enable)
{
    m_pAIController->SetEnable(Enable);
}

void CAICharacter::AttackEnd()
{
}

void CAICharacter::Die()
{
    CEffect* Effect = m_pScene->SpawnObject<CEffect>("EnemyDeadEffect");
    Effect->SetView(m_View);
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("EnemyDeadEffect");
    Box2DInfo Info = m_Body->GetInfo();
    Effect->SetWorldPos(Info.Center.x, Info.Center.y, 0.f);
    Effect->SetAutoDestroy();

    CAIManager::GetInst()->DecreaseEnemy();
}

float CAICharacter::TakeDamage(float Damage, CGameObject* DamageObject)
{
    CTimer* Timer = CEngine::GetInst()->GetTimer();
    Timer->StartTimer<CAICharacter>("TakeDamageEffect", 0.1f, this, &CAICharacter::DamageEmissiveOff);
    m_Sprite->GetMaterial(0)->SetTintColor(1.f, 1.f, 1.f, 1.f);


    return m_HPComponent->TakeDamage(Damage);
}

void CAICharacter::DamageEmissiveOff()
{
    m_Sprite->GetMaterial(0)->SetTintColor(0.f, 0.f, 0.f, 0.f);
}
