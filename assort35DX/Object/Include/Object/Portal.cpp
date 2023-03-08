#include "Portal.h"
#include "Component/ColliderBox2D.h"
#include "Resource\Animation2D.h"
#include "Scene\SceneManager.h"

CPortal::CPortal() :
	m_bCanInteraction(false),
	m_bIsActive(false)
{
}

CPortal::CPortal(const CPortal& obj)
{
}

CPortal::~CPortal()
{
}

void CPortal::SetActivate()
{
	m_bIsActive = true;
	m_Anim->ChangeAnimation("PortalActivate");
}

void CPortal::ChangeScene()
{
	switch (m_NextSceneType)
	{
	case SceneType::Scene02:
		CSceneManager::GetInst()->SetSceneMode<CScene02>();
		break;
	case SceneType::Scene03:
		CSceneManager::GetInst()->SetSceneMode<CScene02>();
		break;
	}
}

bool CPortal::Init()
{
	CGameObject::Init();

	m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
	SetRootComponent(m_Sprite);
	SetPivot(0.5f, 0.f, 0.f);
	m_Sprite->SetRender2DType(Render_Type_2D::RT2D_Back);
	m_Sprite->SetWorldScale(352.f, 256.f, 1.f);
	m_Sprite->CreateAnimation2D<CAnimation2D>();
	m_Anim = m_Sprite->GetAnimation2D();

	m_Anim->AddAnimationSequence2D("PortalDeactivate");
	m_Anim->AddAnimationSequence2D("PortalActivate");


	CSharedPtr<CColliderBox2D>	PortalBody = CreateSceneComponent<CColliderBox2D>("Body");
	m_Sprite->AddChild(PortalBody);

	PortalBody->SetExtent(352.f / 2.f, 256.f / 2.f);
	PortalBody->SetPivot(0.5f, 0.f, 0.f);
	PortalBody->SetCollisionProfile("Trigger");

	PortalBody->AddCollisionOverlapCallbackFunction<CPortal>(Collision_State::Begin, this, &CPortal::BeginCollision);
    return true;
}

void CPortal::Start()
{
	CGameObject::Start();
}

void CPortal::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPortal::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CPortal::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CPortal::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CPortal* CPortal::Clone() const
{
    return nullptr;
}

void CPortal::BeginCollision(const HitResult& Result, CCollider* Collider)
{
	if (m_bIsActive)
		m_bCanInteraction = true;
}

void CPortal::EndCollision(const HitResult& Result, CCollider* Collider)
{
	if (m_bIsActive)
		m_bCanInteraction = false;
}
