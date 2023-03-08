#include "ArcherArrow.h"
#include "Component\ColliderBox2D.h"
#include "Component\SpriteComponent.h"
#include "Scene\Scene.h"
#include "Scene\SceneResource.h"
#include "Effect.h"

CArcherArrow::CArcherArrow()    :
    m_Speed(700.f)
{
}

CArcherArrow::CArcherArrow(const CArcherArrow& obj)
{
}

CArcherArrow::~CArcherArrow()
{
}

void CArcherArrow::Start()
{
    CGameObject::Start();
}

bool CArcherArrow::Init()
{
    if(!CGameObject::Init())
    return false;

    m_pScene->GetResource()->LoadSound("Effect", false, "arrow_hit",
        "AudioClip/Effect/Archer/arrow_hit.wav");

    CSceneResource* SceneResource = m_pScene->GetResource();
    SceneResource->LoadTexture("ArcherArrow", TEXT("Object/Caerleon_Archer.png"));
    SceneResource->LoadTexture("ArcherArrowEffect", TEXT("Effect/ShurkenHit_Atlas.png"));

    SceneResource->CreateAnimationSequence2D("ArcherArrowEffect", 0.3f, "ArcherArrowEffect", 0, 0, 8, Vector2(100.f, 100.f));

    m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
    SetRootComponent(m_Sprite);
    CTexture* Texture = m_pScene->GetResource()->FindTexture("ArcherArrow");
    m_Sprite->GetMaterial(0)->AddTexture("ArcherArrow", Texture);
    m_Sprite->SetWorldScale(50.f, 8.f, 1.f);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

    m_Body = CreateSceneComponent<CColliderBox2D>("Body");
    m_Sprite->AddChild(m_Body);
    m_Body->SetExtent(25.f, 4.f);
    m_Body->SetCollisionProfile("MonsterAttackBlockStatic");
    m_Body->SetPivot(0.5f, 0.5f, 0.f);
    m_Body->AddCollisionOverlapCallbackFunction<CArcherArrow>(Collision_State::Begin, this, &CArcherArrow::CollisionBeginOverlap);
    m_Body->AddCollisionBlockCallbackFunction<CArcherArrow>(Collision_State::Begin, this, &CArcherArrow::CollisionBeginBlock);

    SetLifeTime(0.5f);

    return true;
}

void CArcherArrow::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    AddMove((float)m_View * m_Speed * DeltaTime, 0.f, 0.f);
}

CArcherArrow* CArcherArrow::Clone()
{
    return nullptr;
}

void CArcherArrow::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
    CEffect* Effect = m_pScene->SpawnObject<CEffect>("ArcherArrowEffect");
    CAnimation2D* Anim = Effect->GetAnim();
    Anim->AddAnimationSequence2D("ArcherArrowEffect", false);
    Effect->SetPivot(0.5f, 0.5f, 0.f);
    Effect->SetAutoDestroy();

    if (m_View == Object_View::Right)
        Effect->SetWorldPos2D(m_Body->GetInfo().Max.x, m_Body->GetInfo().Center.y);
    else
        Effect->SetWorldPos2D(m_Body->GetInfo().Min.x, m_Body->GetInfo().Center.y);

    m_pScene->GetResource()->SoundPlay("arrow_hit");

    Active(false);
}

void CArcherArrow::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    IHitable* Hitable = Cast<IHitable*>(Result.DestObject);

    if (Hitable)
        Hitable->TakeDamage(m_AD, this);

    CollisionBeginBlock(Result, Collider);
}
