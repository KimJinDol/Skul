#include "FireBall.h"
#include "Scene\Scene.h"
#include "Scene\SceneResource.h"
#include "Effect.h"

CFireBall::CFireBall()  :
    m_Velocity(800.f),
    m_MoveXMaxPos(900.f),
    m_MoveXPos(0.f)
{
}

CFireBall::~CFireBall()
{
}

void CFireBall::Start()
{
}

bool CFireBall::Init()
{
    CGameObject::Init();

    m_AD = 44.f;

    m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
    SetRootComponent(m_Sprite);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
    m_Sprite->SetRelativeScale(200.f, 200.f, 1.f);
    m_Sprite->CreateAnimation2D<CAnimation2D>();

    CAnimation2D* Anim = m_Sprite->GetAnimation2D();
    Anim->AddAnimationSequence2D("FireBall");

    CTexture* SkulHeadTex = m_pScene->GetResource()->FindTexture("Effect_Altas");
    CMaterial* Mtrl = m_Sprite->GetMaterial(0);
    Mtrl->AddTexture("SkulAtlas", SkulHeadTex);
    Mtrl->SetStartEndUV(12, 12, Vector2(400.f, 300.f));

    m_Body = CreateSceneComponent<CColliderCircle>("Body");
    m_Sprite->AddChild(m_Body);
    m_Body->SetRadius(60.f);
    m_Body->SetPivot(0.5f, 0.5f, 0.f);
    m_Body->SetCollisionProfile("PlayerAttack");
    m_Body->AddCollisionOverlapCallbackFunction<CFireBall>(Collision_State::Begin, this, &CFireBall::CollisionBeginOverlap);

    return true;
}

void CFireBall::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    float MovePos = m_Velocity * DeltaTime;
    if (m_View == Object_View::Left)
        MovePos *= -1.f;

    m_MoveXPos += MovePos;

    AddMove(MovePos, 0.f, 0.f);

    if(m_MoveXPos >= m_MoveXMaxPos || m_MoveXPos <= m_MoveXMaxPos * -1.f)
    {
        CEffect* Effect = m_pScene->SpawnObject<CEffect>("Hit_Fire");
        Effect->SetWorldScale(200.f, 200.f, 1.f);
        CAnimation2D* Anim = Effect->GetAnim();
        Anim->AddAnimationSequence2D("Hit_Fire");
        Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y, 0.f);
        Effect->SetPivot(0.5f, 0.5f, 0.f);
        Effect->SetAutoDestroy();

        Active(false);
    }
}

void CFireBall::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CFireBall::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CFireBall::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

void CFireBall::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{

}

void CFireBall::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    IHitable* Hitable = Cast<IHitable*>(Result.DestObject);

    if (Hitable)
    {
        Hitable->TakeDamage(m_AD, this);
        //CEffect* Effect = m_pScene->SpawnObject<CEffect>("Hit_Fire");
        //Effect->SetWorldScale(200.f, 200.f, 1.f);
        //CAnimation2D* Anim = Effect->GetAnim();
        //Anim->AddAnimationSequence2D("Hit_Fire");
        //Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y, 0.f);
        //Effect->SetPivot(0.5f, 0.5f, 0.f);
        //Effect->SetAutoDestroy();
    }
}
