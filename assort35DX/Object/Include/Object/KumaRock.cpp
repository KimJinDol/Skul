#include "KumaRock.h"
#include "Scene\Scene.h"
#include "Scene\SceneResource.h"
#include <random>
#include "Effect.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"

CKumaRock::CKumaRock()  :
    m_Angle(0),
    m_CurTotalTime(0.f),
    m_InitVelocity(800.f)
{
    m_bIsMoveable = true;
}

CKumaRock::CKumaRock(const CKumaRock& obj)
{
}

CKumaRock::~CKumaRock()
{
}

void CKumaRock::Start()
{
    CGameObject::Start();

    m_InitPos = GetWorldPos();
}

bool CKumaRock::Init()
{
    if (!CGameObject::Init())
        return false;

    m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
    SetRootComponent(m_Sprite);
    CTexture* Texture = m_pScene->GetResource()->FindTexture("KumaRock");
    m_Sprite->GetMaterial(0)->AddTexture("KumaRock", Texture);                                                                  
    m_Sprite->SetWorldScale(44.f, 38.f, 1.f);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
    
    m_Body = CreateSceneComponent<CColliderCircle>("Body");
    m_Sprite->AddChild(m_Body);
    m_Body->SetRadius(30.f);
    m_Body->SetCollisionProfile("MonsterAttackBlockStatic");
    m_Body->SetPivot(0.5f, 0.5f, 0.f);
    m_Body->AddCollisionOverlapCallbackFunction<CKumaRock>(Collision_State::Begin, this, &CKumaRock::CollisionBeginOverlap);
    m_Body->AddCollisionBlockCallbackFunction<CKumaRock>(Collision_State::Begin, this, &CKumaRock::CollisionBeginBlock);

    m_RootComponent->SetJumpVelocity(50.f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(45.f, 135.f);
    m_Angle = dis(gen);
    m_Angle = DegreeToRadian(m_Angle);

    return  true;
}

void CKumaRock::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    m_CurTotalTime += DeltaTime;

    float X = m_InitVelocity * cos(m_Angle) * m_CurTotalTime;
    float Y = m_InitVelocity * sin(m_Angle) * m_CurTotalTime - (0.5 * GRAVITY * 100.f * pow(m_CurTotalTime, 2)); // 마이너스 뒷부분은 자유낙하 공식만큼 빼준 것.

    AddMove(m_InitPos.x - GetWorldPos().x + X, m_InitPos.y - GetWorldPos().y + Y, 0.f);
}

void CKumaRock::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CKumaRock::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CKumaRock::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CKumaRock* CKumaRock::Clone()
{
    return nullptr;
}

void CKumaRock::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
    if (GetMove().y < 0.f)
    {
        CEffect* Effect = m_pScene->SpawnObject<CEffect>("KumaRockSmoke");
        CAnimation2D* Anim = Effect->GetAnim();
        Anim->AddAnimationSequence2D("KumaRockSmoke", false);
        Effect->SetPivot(0.5f, 0.5f, 0.f);
        Effect->SetWorldPos2D(GetWorldPos().x, m_Body->GetInfo().Min.y);
        Effect->SetAutoDestroy();

        m_pScene->GetCameraManager()->GetCurrentCamera()->SetEnableShake(true, 0.3f);

        Active(false);
    }
}

void CKumaRock::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
    IHitable* Hitable = Cast<IHitable*>(Result.DestObject);

    if (Hitable)
        Hitable->TakeDamage(m_AD, this);
}
