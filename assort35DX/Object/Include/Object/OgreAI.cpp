#include "OgreAI.h"
#include "../Animation2D/OgreAnimation.h"
#include "AISelectOgre.h"
#include "Scene\Scene.h"
#include "Scene\SceneMode.h"
#include <DirectXCollision.h>
#include "../UI/BossWidget.h"
#include "Scene/Viewport.h"
#include "Component/HPComponent.h"
#include "Skul.h"
#include "Scene\SceneResource.h"

COgreAI::COgreAI()
{
    m_AttackDistance = 200.f;
    m_TraceDistance = 5000.f;
    m_StanbyTime = 1.f;
}

COgreAI::COgreAI(const COgreAI& obj)
{
}

COgreAI::~COgreAI()
{
}

void COgreAI::CheckTargetInAttackRange()
{
    CGameObject* Target = m_pAIController->GetGameObjectData("Target");

    float Distance = Target->GetWorldPos2D().Distance(GetWorldPos2D());

    if (Distance >= m_AttackDistance)
        AttackEnd();
}

void COgreAI::Attack()
{
    if (m_bIsAttack) return;

    m_bIsAttack = true;
    m_Anim->SetAttack(true);

    m_pAIController->SetBoolData("IsAttack", true);

    m_pScene->GetResource()->SoundPlay("Tutorial_Ogre_Atk_Ready");
}

void COgreAI::AttackCheck()
{
    //XMVECTOR TrianglePoint[3] = {};
    ////const XMVECTOR 
    //if (m_View == Object_View::Right)
    //{
    //    TrianglePoint[0] = Vector2(GetWorldPos2D().x, GetWorldPos2D().y + 50.f).Convert();
    //    TrianglePoint[1] = Vector2(GetWorldPos2D().x + 100.f, GetWorldPos2D().y + 100.f).Convert();
    //    TrianglePoint[2] = Vector2(GetWorldPos2D().x + 100.f, GetWorldPos2D().y - 100.f).Convert();
    //}
    //else
    //{
    //    TrianglePoint[0] = Vector2(GetWorldPos2D().x, GetWorldPos2D().y + 50.f).Convert();
    //    TrianglePoint[1] = Vector2(GetWorldPos2D().x - 100.f, GetWorldPos2D().y + 100.f).Convert();
    //    TrianglePoint[2] = Vector2(GetWorldPos2D().x - 100.f, GetWorldPos2D().y - 100.f).Convert();
    //}

    //CColliderBox2D* Collider = (CColliderBox2D*)m_pAIController->GetGameObjectData("Target")->FindSceneComponent("Body");
    //Box2DInfo Info = Collider->GetInfo();
    //GXMVECTOR Plane = Vector4(Info.Center.x - Info.Length[0], Info.Center.y - Info.Length[1],
    //    Info.Center.x + Info.Length[0], Info.Center.y + Info.Length[1]).Convert();

    //PlaneIntersectionType Result = DirectX::TriangleTests::Intersects(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], Plane);

    //if (Result == PlaneIntersectionType::INTERSECTING)
    //{
    //    int a = 10;
    //}
    if(m_View == Object_View::Right)
        m_AttackCollision->SetRelativePos(80.f, 0.f, 0.f);
    else
        m_AttackCollision->SetRelativePos(-80.f, 0.f, 0.f);

    m_AttackCollision->Enable(true);

    m_pScene->GetResource()->SoundPlay("Tutorial_Ogre_Confuse");
}

void COgreAI::OffAttackCollision()
{
    m_AttackCollision->Enable(false);
    /*m_AttackCollision->Delete*/
}

void COgreAI::AttackEnd()
{
    m_bIsAttack = false;
    m_bIsStanby = true;
    m_Anim->SetAttack(false);

    m_pAIController->SetBoolData("IsAttack", false);

    CGameObject* Target = m_pAIController->GetGameObjectData("Target");
    if (Target->GetWorldPos().x < GetWorldPos().x)
        m_View = Object_View::Left;
    else
        m_View = Object_View::Right;
}

void COgreAI::StanbyEnd()
{
    m_bIsStanby = false;
}

void COgreAI::Die()
{
    m_pAIController->SetEnable(false);
    m_Anim->SetDie(true);
    m_Widget->Enable(false);
}

void COgreAI::Start()
{                                                                
    CAICharacter::Start();

    CGameObject* Player = m_pScene->GetSceneMode()->GetPlayer();

    assert(Player);

    m_pAIController->SetGameObjectData("Target", Player);
    m_pAIController->SetFloatData("AttackDistance", m_AttackDistance);
    m_pAIController->SetFloatData("TraceDistance", m_TraceDistance);
    m_pAIController->SetFloatData("StanbyTime", m_StanbyTime);
    m_pAIController->SetBoolData("IsStanby", false);
    m_pAIController->SetBoolData("IsAttack", false);
}

bool COgreAI::Init()
{
    CAICharacter::Init();

    m_pScene->GetResource()->LoadSound("Effect", false, "Tutorial_Ogre_Atk_Ready",
        "AudioClip/Effect/Ogre/Tutorial_Ogre_Atk_Ready.wav");
    m_pScene->GetResource()->LoadSound("Effect", false, "Tutorial_Ogre_Confuse",
        "AudioClip/Effect/Ogre/Tutorial_Ogre_Confuse.wav");

    SetMoveSpeed(200.f);

    m_HPComponent->SetMaxHp(100.f);

    m_Sprite->CreateAnimation2D<COgreAnimation>();
    m_Anim = (COgreAnimation*)m_Sprite->GetAnimation2D();

    m_Body->SetExtent(75.f, 75.f);
    m_Body->SetCollisionProfile("Monster");
    m_Body->AddCollisionOverlapCallbackFunction<COgreAI>(Collision_State::Begin, this, &COgreAI::CollisionBeginOverlap);
    m_Body->AddCollisionBlockCallbackFunction<COgreAI>(Collision_State::Begin, this, &COgreAI::CollisionBeginBlock);

    m_AttackCollision = CreateSceneComponent<CColliderBox2D>("AttackCollision");
    m_Sprite->AddChild(m_AttackCollision);
    m_AttackCollision->SetPivot(0.5f, 0.f, 0.f);
    m_AttackCollision->SetExtent(40.f, 75.f);
    m_AttackCollision->SetRelativePos(80.f, 0.f, 0.f);
    m_AttackCollision->SetCollisionProfile("MonsterAttack");
    m_AttackCollision->AddCollisionOverlapCallbackFunction<COgreAI>(Collision_State::Begin, this, &COgreAI::CollisionBeginOverlap);
    m_AttackCollision->AddCollisionBlockCallbackFunction<COgreAI>(Collision_State::Begin, this, &COgreAI::CollisionBeginBlock);
    m_AttackCollision->Enable(false);

    CreateAISelect<CAISelectOgre>("OgreSelect");


    m_Anim->SetFrameEndFunction<COgreAI>("OgreAttack", this, &COgreAI::AttackEnd);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 6, this, &COgreAI::CheckTargetInAttackRange);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 12, this, &COgreAI::CheckTargetInAttackRange);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 4, this, &COgreAI::AttackCheck);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 5, this, &COgreAI::OffAttackCollision);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 10, this, &COgreAI::AttackCheck);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 11, this, &COgreAI::OffAttackCollision);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 21, this, &COgreAI::AttackCheck);
    m_Anim->SetNotifyFunction<COgreAI>("OgreAttack", 22, this, &COgreAI::OffAttackCollision);

    m_Widget = m_pScene->GetViewport()->AddWindow<CBossWidget>("OgreWidget");
    m_Widget->SetOwnerAI(this);
    m_Widget->SetBossName(TEXT("몬갈"));
    m_Widget->SetSubName(TEXT("오우거"));

    m_AD = 5.f;

    return true;
}

void COgreAI::Update(float DeltaTime)
{
    CAICharacter::Update(DeltaTime);
}

void COgreAI::PostUpdate(float DeltaTime)
{
    CAICharacter::PostUpdate(DeltaTime);
}

void COgreAI::Collision(float DeltaTime)
{
    CAICharacter::Collision(DeltaTime);
}

void COgreAI::Render(float DeltaTime)
{
    CAICharacter::Render(DeltaTime);
}

COgreAI* COgreAI::Clone() const
{
    return nullptr;
}

void COgreAI::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    CAICharacter::CollisionBeginOverlap(Result, Collider);

}

void COgreAI::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
    CAICharacter::CollisionBeginBlock(Result, Collider);
}
