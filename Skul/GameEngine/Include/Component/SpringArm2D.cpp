
#include "SpringArm2D.h"
#include "../Scene/Scene.h"
#include "../Device.h"
#include "../Scene/CameraManager.h"
#include "../Component/Camera.h"
#include "../BasicMath.h"

CSpringArm2D::CSpringArm2D()
{
	m_SceneComponentType = SceneComponent_Type::SpringArm;
}

CSpringArm2D::CSpringArm2D(const CSpringArm2D& com) :
	CSceneComponent(com)
{
}

CSpringArm2D::~CSpringArm2D()
{
}

void CSpringArm2D::Start()
{
	CSceneComponent::Start();

	if (m_pParent)
	{
		Vector3	Pos = m_pParent->GetWorldPos() + m_Offset;

		Resolution  RS = CDevice::GetInst()->GetResolution();

		m_WorldSize = m_pScene->GetCameraManager()->GetCurrentCamera()->Get2DWorldSize();
		if (Pos.x < 0.f)
			Pos.x = 0.f;

		else if (Pos.x + RS.Width > m_WorldSize.x)
			Pos.x = m_WorldSize.x - RS.Width;

		if (Pos.y < 0.f)
			Pos.y = 0.f;

		else if (Pos.y + RS.Height > m_WorldSize.y)
			Pos.y = m_WorldSize.y - RS.Height;


		SetWorldPos(Pos);
		m_PrevPos = Pos;
	}
}

bool CSpringArm2D::Init()
{
	if (!CSceneComponent::Init())
		return false;
	
	return true;
}

void CSpringArm2D::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CSpringArm2D::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	if (m_pParent)
	{
		Vector3	ParentPos = m_pParent->GetWorldPos();

		if (!GetInheritPosZ())
			ParentPos.z = 0.f;

		Vector3	Pos = ParentPos + m_Offset;


		Resolution  RS = CDevice::GetInst()->GetResolution();

		if (Pos.x < 0.f)
			Pos.x = 0.f;

		else if (Pos.x + RS.Width > m_WorldSize.x)
			Pos.x = m_WorldSize.x - RS.Width;

		if (Pos.y < 0.f)
			Pos.y = 0.f;

		else if (Pos.y + RS.Height > m_WorldSize.y)
			Pos.y = m_WorldSize.y - RS.Height;


		m_WorldSize = m_pScene->GetCameraManager()->GetCurrentCamera()->Get2DWorldSize();

		// Lerp
        Pos.x = BasicMath::FInterpTo(m_PrevPos.x, Pos.x, DeltaTime, 7.f);
        Pos.y = BasicMath::FInterpTo(m_PrevPos.y, Pos.y, DeltaTime, 7.f);

		m_PrevPos = Pos;
		SetWorldPos(Pos);
	}
}

void CSpringArm2D::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CSpringArm2D::PrevRender(float DeltaTime)
{
	CSceneComponent::PrevRender(DeltaTime);
}

void CSpringArm2D::Render(float DeltaTime)
{
	CSceneComponent::Render(DeltaTime);
}

CSpringArm2D* CSpringArm2D::Clone()
{
	return new CSpringArm2D(*this);
}
