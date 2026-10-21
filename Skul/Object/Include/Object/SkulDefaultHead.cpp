#include "SkulDefaultHead.h"
#include "Scene/SceneResource.h"
#include "Scene/Scene.h"
#include "BasicMath.h"

CSkulDefaultHead::CSkulDefaultHead()    :
    m_Velocity(1000.f),
    m_Dir(0.f),
    m_bIsBlock(false),
    m_MoveVel(80.f)
{
    m_bCanMoveOutsideWorld = false;
}

CSkulDefaultHead::CSkulDefaultHead(const CSkulDefaultHead& obj)
{
}

CSkulDefaultHead::~CSkulDefaultHead()
{
}

void CSkulDefaultHead::SetDirection(int Dir)
{
    m_Dir = Dir;
}

void CSkulDefaultHead::Start()
{
    CGameObject::Start();
}

bool CSkulDefaultHead::Init()
{
    CGameObject::Init();

    m_bIsMoveable = true;

    m_Body = CreateSceneComponent<CColliderCircle>("Body");
    SetRootComponent(m_Body);
    m_Body->SetRadius(20.f);
    m_Body->SetPivot(0.5f, 0.5f, 0.f);
    m_Body->SetCollisionProfile("Head");
    m_Body->AddCollisionBlockCallbackFunction<CSkulDefaultHead>(Collision_State::Begin, this, &CSkulDefaultHead::CollisionBeginBlock);
    m_Body->AddCollisionOverlapCallbackFunction<CSkulDefaultHead>(Collision_State::Begin, this, &CSkulDefaultHead::CollisionBeginOverlap);

    m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
    m_Body->AddChild(m_Sprite);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
    m_Sprite->SetRelativeScale(400.f, 300.f, 1.f);

    CTexture* SkulHeadTex = m_pScene->GetResource()->FindTexture("SkulAtlas");
    CMaterial* Mtrl = m_Sprite->GetMaterial(0);
    Mtrl->AddTexture("SkulAtlas", SkulHeadTex);
    Mtrl->SetStartEndUV(12, 12, Vector2(400.f, 300.f));

    SetPhysicsSimulate(false);
    SetBounceGravity(true);
    SetFriction(1.3f);

    m_AD = 10.f;


    return true;
}

void CSkulDefaultHead::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (!m_bIsBlock)
        AddMove(m_Dir * m_Velocity * DeltaTime, 0.f, 0.f);
}

void CSkulDefaultHead::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    float Radius = m_Body->GetInfo().Radius;

    float Rotation = BasicMath::CalculateRotationPerMoveLength(GetMove().x, Radius);

    m_Sprite->AddWorldRotation(0.f, 0.f, Rotation);
}

void CSkulDefaultHead::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CSkulDefaultHead::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CSkulDefaultHead* CSkulDefaultHead::Clone()
{
    return new CSkulDefaultHead(*this);
}

void CSkulDefaultHead::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
    // When Collision was block Static colliders...
    if (Result.DestCollider->GetProfile()->Channel == Collision_Channel::Static)
    {
        if (m_bIsBlock) return;

        m_bIsBlock = true;
        if (m_Dir == 1.f)
            AddForce(-1.f * m_MoveVel, 0.f, 0.f);

        else
            AddForce(1.f * m_MoveVel, 0.f, 0.f);

        SetPhysicsSimulate(true);
    }
}

void CSkulDefaultHead::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    IHitable* Hitable = Cast<IHitable*>(Result.DestObject);
    if (Hitable && Result.DestCollider->GetProfile()->Channel != Collision_Channel::Player)
    {
        Hitable->TakeDamage(m_AD, this);

        if (m_bIsBlock) return;

        m_bIsBlock = true;
        if (m_Dir == 1.f)
            AddForce(-1.f * m_MoveVel, 0.f, 0.f);

        else
            AddForce(1.f * m_MoveVel, 0.f, 0.f);

        SetPhysicsSimulate(true);
    }
}
