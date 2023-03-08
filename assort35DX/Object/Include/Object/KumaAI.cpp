#include "KumaAI.h"
#include "Component/HPComponent.h"
#include "AISelectKuma.h"
#include "../UI/BossWidget.h"
#include "Scene/Viewport.h"
#include "Scene/Scene.h"
#include "Effect.h"
#include "CAITask_Ultimate.h"
#include "AITask_EarthHit.h"
#include "AITask_Stamping.h"
#include "AITask_JumpStamping.h"
#include "AITask_Shield.h"
#include "AITask_Tackle.h"
#include "AITask_Intro.h"
#include "Component/ColliderCircle.h"
#include "Skul.h"
#include "Engine.h"
#include "Timer.h"
#include "Render\RenderManager.h"
#include "BasicMath.h"
#include "KumaRock.h"
#include "KumaShield.h"
#include "../Animation2D/KumaShieldAnimation.h"
#include "Scene/CameraManager.h"
#include "Component\Camera.h"
#include "Scene\SceneResource.h"

CKumaAI::CKumaAI() :
    m_bIsUltimate(false),
    m_bIsEarthHit(false),
    m_EarthHitCheckRange(50.f),
    m_EarthHitAD(30.f),
    m_UltimateAD(10.f),
    m_EarthHitFadeOutBackgroundTime(0.7f),
    m_CurEarthHitFadeOutBackgroundTime(0.f),
    m_bFadeOutBackground(false),
    m_bFadeInBackground(false),
    m_StampingMoveX(100.f),
    m_StampingMoveSpeed(0.2f),
    m_JumpStampingMoveSpeed(1.3f),
    m_bIsShieldDefense(false),
    m_bIsShieldDash(false),
    m_bCanShieldDash(false),
    m_CurShieldDashCount(0),
    m_ShieldDashMaxCount(4),
    m_ShieldDashMoveDistance(400.f),
    m_ShieldDashSpeed(4.f),
    m_ShieldDashRetryDistance(100.f),
    m_ShieldDashTotalMove(0.f),
    m_bIsIntroJumpStampingCheck(false)
{
    m_StanbyTime = 3.f;
    m_EarthHitFadeOutBackgroundColor = Vector4(-0.3f, -0.3f, -0.3f, 0.f);
}

CKumaAI::CKumaAI(const CKumaAI& obj)
{
}

CKumaAI::~CKumaAI()
{
}

void CKumaAI::IntroEnd()
{
    SetEnableController(true);
    m_Anim->SetIntro(false);
    m_Widget->Enable(true);
    m_Widget->OnBossHealthWidget(true);

    BehaviorEnd();
}

void CKumaAI::CheckTargetInAttackRange()
{
}

void CKumaAI::Attack()
{
}

void CKumaAI::AttackCheck()
{
}

void CKumaAI::OffAttackCollision()
{
}

void CKumaAI::AttackEnd()
{
}

void CKumaAI::StanbyEnd()
{
}

void CKumaAI::Die()
{
    m_Anim->SetDie(true);
}

void CKumaAI::Ultimate()
{
    CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaUltimateEffect");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("KumaUltimateAura");
    Anim->AddAnimationSequence2D("KumaUltimateComplete");
    Anim->SetNextSequence2D("KumaUltimateAura", "KumaUltimateComplete");
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y, 0.f);
    Effect->SetAutoDestroy("KumaUltimateComplete");

    m_Anim->SetUltimate(true);

    m_pScene->GetResource()->SoundPlay("AdventurerWarrior_Voice_Casting");

    m_bFadeOutBackground = true;
}

void CKumaAI::UltimateStart()
{
    m_bIsUltimate = true;
    m_UltimateCollision->Enable(true);
}

void CKumaAI::UltimateAttackCheck()
{
    m_UltimateCollision->DeleteAllPrevCollision();

}

void CKumaAI::UltimateEnd()
{
    m_bIsUltimate = false;
    m_Anim->SetUltimate(false);
    m_UltimateCollision->Enable(false);
    BehaviorEnd();

    m_bFadeOutBackground = false;
    m_bFadeInBackground = true;
}

void CKumaAI::EarthHit()
{
    CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaEarthHitEffect");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("KumaEarthHitCharging");
    Effect->SetPivot(0.5f, 0.f, 0.f);
    Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y, 0.f);
    Effect->SetAutoDestroy();

    m_Anim->SetEarthHitReady(true);

    m_bFadeOutBackground = true;
}

void CKumaAI::EarthHitStart()
{
    m_Anim->SetEarthHitReady(false);
    m_Anim->SetEarthHit(true);

    m_bIsEarthHit = true;
}

void CKumaAI::EarthHitJump()
{
    CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaEarthSignEffect");
    CAnimation2D* Anim = Effect->GetAnim(); 
    Anim->AddAnimationSequence2D("KumaEarthHitInSign", false);
    Effect->SetPivot(0.5f, 0.f, 0.f);
    Effect->SetWorldPos2D(2540.f, 440.f);
    Effect->SetAutoDestroy();


    CCharacter::Jump();
}

void CKumaAI::EarthHitAttackCheck()
{
    CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaEarthHitSmoke");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("KumaEarthHitSmoke", false);
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    Effect->SetWorldPos2D(GetWorldPos2D());
    Effect->SetAutoDestroy();

    CGameObject* Target = m_pAIController->GetGameObjectData("Target");

    assert(Target);

    if (Target->GetWorldPos().y <= GetWorldPos().y + m_EarthHitCheckRange)
    {
        CSkul* Skul = (CSkul*)Target;
        Skul->TakeDamage(m_EarthHitAD, this);
    }

    m_pScene->GetCameraManager()->GetCurrentCamera()->SetEnableShake(true, 0.5f);
    m_pScene->GetResource()->SoundPlay("Ambience_Earthquake_Small");

    CEngine::GetInst()->GetTimer()->StartTimer<CKumaAI>("KumaEarthHitRest", 2.f, this, &CKumaAI::EarthHitEnd);
}

void CKumaAI::EarthHitEnd()
{
    m_bIsEarthHit = false;
    m_Anim->SetEarthHit(false);
    m_CurEarthHitFadeOutBackgroundTime = 0.f;
    m_pAIController->SetBoolData("EarthHitJump", false);
    m_pAIController->SetBoolData("IsEarthHit", false);
    BehaviorEnd();

    m_bFadeOutBackground = false;
    m_bFadeInBackground = true;
}

void CKumaAI::Stamping()
{
    m_Anim->SetStamping(true);

    if (m_pAIController->GetGameObjectData("Target")->GetWorldPos().x > GetWorldPos().x)
        m_View = Object_View::Right;
    else
        m_View = Object_View::Left;
}

void CKumaAI::StampingStart()
{
    m_pAIController->SetBoolData("IsStampingMove", true);
}

void CKumaAI::StampingAttackCheck()
{
    m_AttackCollision->Enable(true);

    CEngine::GetInst()->GetTimer()->StartTimer<CKumaAI>("KumaStampingRest", 2.f, this, &CKumaAI::StampingEnd);

    CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaStampingEffect");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("KumaRockSmoke");
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    float EffectOffset = (float)m_View * 50.f;
    Effect->SetWorldPos(GetWorldPos().x + EffectOffset, GetWorldPos().y, 0.f);
    Effect->SetAutoDestroy();

    m_pScene->GetResource()->SoundPlay("AdventurerWarrior_Voice_Middle");
}

void CKumaAI::StampingCollisionOff()
{
    m_AttackCollision->Enable(false);
}

void CKumaAI::StampingEnd()
{
    //m_bIsEarthHit = false;
    m_Anim->SetStamping(false);
    m_pAIController->SetBoolData("IsStampingMove", false);
    m_pAIController->SetBoolData("IsStamping", false);
    BehaviorEnd();
}

void CKumaAI::JumpStamping()
{
    m_Anim->SetJumpStamping(true);
    CGameObject* Target = m_pAIController->GetGameObjectData("Target");
    float TargetPosX = Target->GetWorldPos().x;
    m_pAIController->SetFloatData("JumpStampingTargetPos", TargetPosX);

    m_pScene->GetResource()->SoundPlay("AdventurerWarrior_Jump");

    CCharacter::Jump();
}

void CKumaAI::JumpStampingStart()
{

}

void CKumaAI::JumpStampingAttackCheck()
{
    m_AttackCollision->Enable(true);

    CEngine::GetInst()->GetTimer()->StartTimer<CKumaAI>("KumaStampingRest", 2.f, this, &CKumaAI::JumpStampingEnd);

    CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaStampingEffect");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("KumaStampingSmoke");
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y, 0.f);
    Effect->SetAutoDestroy();

    // 돌덩이 소환
    CKumaRock* Rock;
    Vector3 Pos = GetWorldPos();
    for (int i = 0; i < 5; i++)
    {
        Rock = m_pScene->SpawnObject<CKumaRock>("Rock");
        Rock->SetWorldPos(Pos.x, Pos.y, 0.f);
    }

   m_pScene->GetCameraManager()->GetCurrentCamera()->SetEnableShake(true, 0.3f);

}

void CKumaAI::JumpStampingEnd()
{
    m_Anim->SetJumpStamping(false);
    m_pAIController->SetBoolData("IsJumpStamping", false);
    BehaviorEnd();

    if (!m_bIsIntroJumpStampingCheck)
        m_bIsIntroJumpStampingCheck = true;
}

void CKumaAI::Shield(bool Dash)
{
    m_bCanShieldDash = Dash;

    if (Dash)
        ShieldDash();
    else
        m_Anim->SetShield(true);
    m_ShieldSprite->Enable(true);
    m_ShieldAnim->SetOn(true);
}

void CKumaAI::ShieldStart()
{
    m_bIsShieldDefense = true;

    CEngine::GetInst()->GetTimer()->StartTimer<CKumaAI>("KumaShield", 2.f, this, &CKumaAI::ShieldOff);
}

void CKumaAI::ShieldOff()
{
    m_bIsShieldDefense = false;
    m_ShieldAnim->SetOn(false);
}

void CKumaAI::ShieldEnd()
{
    m_bIsShieldDash = false;
    m_Anim->SetShield(false);
    m_Anim->SetShieldDash(false);
    m_Anim->SetShieldDashLoop(false);
    m_ShieldSprite->Enable(false);
    m_pAIController->SetBoolData("IsShield", false);
    m_pAIController->SetBoolData("IsTackle", false);
    m_pAIController->SetBoolData("IsStanby", true);

    m_CurShieldDashCount = 0;

    BehaviorEnd();
}

void CKumaAI::ShieldDash()
{
    m_Anim->SetShieldDash(true);
    m_Anim->SetShieldDashLoop(false);
}

void CKumaAI::ShieldDashStart()
{
    m_bIsShieldDash = true;
    m_Anim->SetShieldDashLoop(true);

    CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaShieldDashaEffect");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("KumaShieldDashAura");
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    Effect->SetOffset(-30.f, 80.f);
    Effect->SetOwner(this);
    Effect->SetUseLocalSpace(true);
    Effect->SetAutoDestroy();

    CGameObject* Target = m_pAIController->GetGameObjectData("Target");
    if (Target->GetWorldPos().x < GetWorldPos().x)
    {
        m_ShieldDashTargetPos = GetWorldPos().x - m_ShieldDashMoveDistance;
        Effect->SetView(Object_View::Left);
    }
    else
    {
        m_ShieldDashTargetPos = GetWorldPos().x + m_ShieldDashMoveDistance;
        Effect->SetView(Object_View::Right);
    }
    m_ShieldDashTotalMove = GetWorldPos().x;

    m_CurShieldDashCount++;
}

void CKumaAI::Intro()
{
    m_Anim->SetIntro(true);
    m_pScene->GetResource()->SoundPlay("Adventurer_Drink");
}

void CKumaAI::IntroJumpStamping()
{
    m_Anim->SetJumpStamping(true);
    float TargetPosX = 3000.f;
    m_pAIController->SetFloatData("JumpStampingTargetPos", TargetPosX);

    CCharacter::Jump();
}

void CKumaAI::BehaviorEnd()
{
    m_bIsStanby = true;

    if (m_pAIController->GetGameObjectData("Target")->GetWorldPos().x > GetWorldPos().x)
        m_View = Object_View::Right;
    else
        m_View = Object_View::Left;
}

float CKumaAI::TakeDamage(float Damage, CGameObject* DamageObject)
{
    bool bDamage = false;

    float ShieldCenter = m_ShieldBody->GetInfo().Center.x;

    if (!m_bIsShieldDefense)
        bDamage = true;

    // shield 가 켜진 상태일 때 이하 실행
    else if ((m_View == Object_View::Right && DamageObject->GetWorldPos().x < ShieldCenter) ||
        (m_View == Object_View::Left && DamageObject->GetWorldPos().x > ShieldCenter))
        bDamage = true;
    
    if (bDamage)
        return CAICharacter::TakeDamage(Damage, DamageObject);
    else
        return m_HPComponent->GetHp();
}

void CKumaAI::Start()
{
    CAICharacter::Start();

    SetEnableController(false);

    m_View = Object_View::Left;
    /*m_Anim->SetIntro(true);*/
}

bool CKumaAI::Init()
{
    if (!CAICharacter::Init())
        return false;

    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Drinking",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Drinking.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Jump",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Jump.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Voice_Casting",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Voice_Casting.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Voice_Short",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Voice_Short.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Voice_Middle",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Voice_Middle.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "Ambience_Earthquake_Small",
        "AudioClip/Effect/Warrior/Ambience_Earthquake_Small.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Shield_Hit",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Shield_Hit.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Drinking",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Drinking.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "AdventurerWarrior_Drinking",
        "AudioClip/Effect/Warrior/AdventurerWarrior_Drinking.wav");

    SetMoveSpeed(200.f);

    m_HPComponent->SetMaxHp(300.f);

    m_Sprite->CreateAnimation2D<CKumaAnimation>();
    m_Anim = (CKumaAnimation*)m_Sprite->GetAnimation2D();

    m_Body->SetExtent(75.f, 75.f);
    m_Body->SetCollisionProfile("Monster");
    m_Body->AddCollisionOverlapCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginOverlap);
    m_Body->AddCollisionBlockCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginBlock);

    m_AttackCollision = CreateSceneComponent<CColliderBox2D>("AttackCollision");
    m_Sprite->AddChild(m_AttackCollision);
    m_AttackCollision->SetPivot(0.5f, 0.f, 0.f);
    m_AttackCollision->SetExtent(70.f, 70.f);
    m_AttackCollision->SetRelativePos(80.f, 0.f, 0.f);
    m_AttackCollision->SetCollisionProfile("MonsterAttack");
    m_AttackCollision->AddCollisionOverlapCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginOverlap);
    m_AttackCollision->AddCollisionBlockCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginBlock);
    m_AttackCollision->Enable(false);

    m_UltimateCollision = CreateSceneComponent<CColliderCircle>("UltimateCollision");
    m_Sprite->AddChild(m_UltimateCollision);
    m_UltimateCollision->SetPivot(0.5f, 0.f, 0.f);
    m_UltimateCollision->SetRadius(130.f);
    m_UltimateCollision->SetRelativePos(0.f, 30.f, 0.f);
    m_UltimateCollision->SetCollisionProfile("MonsterAttack");
    m_UltimateCollision->AddCollisionOverlapCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginOverlap);
    m_UltimateCollision->AddCollisionBlockCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginBlock);
    m_UltimateCollision->Enable(false);

    CreateAI<CAITask_Ultimate>("Ultimate");
    CreateAI<CAITask_EarthHit>("EarthHit");
    CreateAI<CAITask_Stamping>("Stamping");
    CreateAI<CAITask_Stamping>("Stamping");
    CreateAI<CAITask_JumpStamping>("JumpStamping");
    CreateAI<CAITask_Shield>("Shield");
    CreateAI<CAITask_Tackle>("Tackle");
    CreateAI<CAITask_Intro>("Intro");

    m_pAIController->SetBoolData("IsStanby", false);
    m_pAIController->SetFloatData("StampingMoveX", m_StampingMoveX);
    m_pAIController->SetFloatData("StampingMoveSpeed", m_StampingMoveSpeed);
    m_pAIController->SetFloatData("JumpStampingMoveSpeed", m_JumpStampingMoveSpeed);
     
    CreateAISelect<CAISelectKuma>("KumaSelect");


    m_Widget = m_pScene->GetViewport()->AddWindow<CBossWidget>("KumaWidget");
    m_Widget->SetOwnerAI(this);
    m_Widget->SetBossName(TEXT("전사"));
    m_Widget->SetSubName(TEXT("칼레온의 정의"));

    m_ShieldSprite = CreateSceneComponent<CSpriteComponent>("ShieldSprite");
    m_Sprite->AddChild(m_ShieldSprite);
    m_ShieldSprite->SetRelativePos(80.f, 86.f, 0.f);
    m_ShieldSprite->SetWorldScale(200.f, 180.f, 1.f);
    m_ShieldSprite->SetPivot(0.5f, 0.5f, 0.f);
    m_ShieldSprite->CreateAnimation2D<CKumaShieldAnimation>();
    m_ShieldSprite->Enable(false);

    m_ShieldAnim = (CKumaShieldAnimation*)m_ShieldSprite->GetAnimation2D();


    m_ShieldBody = CreateSceneComponent<CColliderBox2D>("ShieldBody");
    m_ShieldSprite->AddChild(m_ShieldBody);
    m_ShieldBody->SetExtent(25.f, 100.f);
    m_ShieldBody->SetCollisionProfile("MonsterAttack");
    m_ShieldBody->SetPivot(0.5f, 0.5f, 0.f);
    m_ShieldBody->AddCollisionOverlapCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginOverlap);
    m_ShieldBody->AddCollisionBlockCallbackFunction<CKumaAI>(Collision_State::Begin, this, &CKumaAI::CollisionBeginBlock);

    {
        m_Anim->SetFrameEndFunction<CKumaAI>("KumaWhirlWindReady", this, &CKumaAI::UltimateStart);
        m_Anim->SetNotifyFunction<CKumaAI>("KumaWhirlWindLoop", 6, this, &CKumaAI::UltimateAttackCheck);
        m_Anim->SetFrameEndFunction<CKumaAI>("KumaEarthQuakeJumpReady", this, &CKumaAI::EarthHitStart);
        /*m_Anim->SetFrameEndFunction<CKumaAI>("KumaIntro", this, &CKumaAI::IntroEnd);*/
        m_Anim->SetFrameEndFunction<CKumaAI>("KumaStampingUp", this, &CKumaAI::StampingStart);
        m_Anim->SetNotifyFunction<CKumaAI>("KumaStampingHit", 0, this, &CKumaAI::StampingAttackCheck);
        m_Anim->SetNotifyFunction<CKumaAI>("KumaStampingHit", 5, this, &CKumaAI::StampingCollisionOff);
        m_Anim->SetNotifyFunction<CKumaAI>("KumaJumpStampingHit", 0, this, &CKumaAI::JumpStampingAttackCheck);
        m_Anim->SetNotifyFunction<CKumaAI>("KumaJumpStampingHit", 5, this, &CKumaAI::StampingCollisionOff);
        m_Anim->SetFrameEndFunction<CKumaAI>("KumaShieldReady", this, &CKumaAI::ShieldStart);
        m_Anim->SetFrameEndFunction<CKumaAI>("KumaTackleReady", this, &CKumaAI::ShieldDashStart);

        m_ShieldAnim->SetFrameEndFunction<CKumaAI>("KumaShieldOff", this, &CKumaAI::ShieldEnd);
    }

    m_AD = 5.f;

    SetGravityAccel(13.f);
    SetJumpVelocity(80.f);

    return true;
}

void CKumaAI::Update(float DeltaTime)
{
    CAICharacter::Update(DeltaTime);

    if (m_bFadeOutBackground)
    {
        m_CurEarthHitFadeOutBackgroundTime = BasicMath::Clamp<float>(m_CurEarthHitFadeOutBackgroundTime + DeltaTime, 0.f, m_EarthHitFadeOutBackgroundTime);
        Vector4 LerpTint = BasicMath::V4Lerp(Vector4(0.f, 0.f, 0.f, 0.f), m_EarthHitFadeOutBackgroundColor, m_CurEarthHitFadeOutBackgroundTime / m_EarthHitFadeOutBackgroundTime);
        CRenderManager::GetInst()->SetRenderTypeTint(Render_Type_2D::RT2D_Background, LerpTint);
    }

    if (m_bFadeInBackground)
    {
        m_CurEarthHitFadeOutBackgroundTime = BasicMath::Clamp<float>(m_CurEarthHitFadeOutBackgroundTime + DeltaTime, 0.f, m_EarthHitFadeOutBackgroundTime);
        Vector4 LerpTint = BasicMath::V4Lerp(m_EarthHitFadeOutBackgroundColor, Vector4(0.f, 0.f, 0.f, 0.f), m_CurEarthHitFadeOutBackgroundTime / m_EarthHitFadeOutBackgroundTime);
        CRenderManager::GetInst()->SetRenderTypeTint(Render_Type_2D::RT2D_Background, LerpTint);

        if (LerpTint == Vector4(0.f, 0.f, 0.f, 0.f))
        {
            m_bFadeInBackground = false;
            m_CurEarthHitFadeOutBackgroundTime = 0.f;
        }
    }

    if (m_bIsShieldDash)
    {
        float TargetPos = BasicMath::FInterpTo(m_ShieldDashTotalMove, m_ShieldDashTargetPos, DeltaTime, m_ShieldDashSpeed);
        m_ShieldDashTotalMove = TargetPos;
        AddMove(m_ShieldDashTotalMove - GetWorldPos().x, 0.f, 0.f);

        if (fabs(TargetPos - m_ShieldDashTargetPos) <= m_ShieldDashRetryDistance)
        {
            if (m_CurShieldDashCount < m_ShieldDashMaxCount)
                ShieldDash();
            else
                ShieldOff();
        }
    }

}

void CKumaAI::PostUpdate(float DeltaTime)
{
    CAICharacter::PostUpdate(DeltaTime);

    if (m_PostView != m_View)
    {
        m_AttackCollision->RelativePosFlip(true, false, false);
        m_ShieldSprite->RelativePosFlip(true, false, false);
    }
}

void CKumaAI::Collision(float DeltaTime)
{
    CAICharacter::Collision(DeltaTime);
}

void CKumaAI::Render(float DeltaTime)
{
    CAICharacter::Render(DeltaTime);
}

CKumaAI* CKumaAI::Clone() const
{
    return nullptr;
}

void CKumaAI::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    if (Collider == m_ShieldBody)
    {
        m_pScene->GetCameraManager()->GetCurrentCamera()->SetEnableShake(true, 0.3f);
        m_pScene->GetResource()->SoundPlay("AdventurerWarrior_Shield_Hit");
    }

    CAICharacter::CollisionBeginOverlap(Result, Collider);
}

void CKumaAI::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
    CAICharacter::CollisionBeginBlock(Result, Collider);
}
