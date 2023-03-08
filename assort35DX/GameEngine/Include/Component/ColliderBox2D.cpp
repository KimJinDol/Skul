
#include "ColliderBox2D.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "ColliderCircle.h"
#include "ColliderPixel.h"
#include "ColliderLine.h"

CColliderBox2D::CColliderBox2D()
{
	m_ColliderShape = Collider_Shape::Box2D;
	m_ColliderSpace = Collider_Space::Collider2D;

	m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
	m_2DType = RT2D_Default;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& com) :
	CCollider(com)
{
	m_Info = com.m_Info;
}

CColliderBox2D::~CColliderBox2D()
{
}

void CColliderBox2D::Start()
{
	CCollider::Start();

	m_DebugMesh = m_pScene->GetResource()->FindMesh("ColliderBox2D");
}

bool CColliderBox2D::Init()
{
	if (!CCollider::Init())
		return false;

	m_Info.Axis[AXIS_X] = Vector2(1.f, 0.f);
	m_Info.Axis[AXIS_Y] = Vector2(0.f, 1.f);

	SetInheritRotZ(true);

	SetUpdatePosZ(true);

	return true;
}

void CColliderBox2D::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderBox2D::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	m_Info.Center.x = GetWorldPos().x + ((0.5f - GetPivot().x) * GetWorldScale().x);
	m_Info.Center.y = GetWorldPos().y + ((0.5f - GetPivot().y) * GetWorldScale().y);

	m_Info.Axis[AXIS_X].x = GetAxis(AXIS_X).x;
	m_Info.Axis[AXIS_X].y = GetAxis(AXIS_X).y;

	m_Info.Axis[AXIS_Y].x = GetAxis(AXIS_Y).x;
	m_Info.Axis[AXIS_Y].y = GetAxis(AXIS_Y).y;
	 

	// 사각형 꼭짓점의 위치는 센터에서 -x축 방향으로 x/2 길이 만큼, -y축 방향으로 y/2 길이 만큼 가면 왼쪽 상단의 위치를 구할 수 있다.
	// 이런 방식으로 네 꼭짓점의 위치를 찾아준다. (-x-y방향, -x+y방향, +x-y방향, +x+y방향)
	m_Info.Point[0] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0] +
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];
	m_Info.Point[1] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0] +
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];

	m_Info.Point[2] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0] -
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];
	m_Info.Point[3] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0] -
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];

	m_Min.x = m_Info.Point[0].x;
	m_Min.y = m_Info.Point[0].y;

	m_Max.x = m_Info.Point[0].x;
	m_Max.y = m_Info.Point[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (m_Min.x > m_Info.Point[i].x)
			m_Min.x = m_Info.Point[i].x;

		if (m_Min.y > m_Info.Point[i].y)
			m_Min.y = m_Info.Point[i].y;

		if (m_Max.x < m_Info.Point[i].x)
			m_Max.x = m_Info.Point[i].x;

		if (m_Max.y < m_Info.Point[i].y)
			m_Max.y = m_Info.Point[i].y;
	}

	m_Info.Min = m_Min;
	m_Info.Max = m_Max;
}

void CColliderBox2D::Collision(float DeltaTime)
{
	CCollider::Collision(DeltaTime);
}

void CColliderBox2D::PrevRender(float DeltaTime)
{
	CCollider::PrevRender(DeltaTime);
}

void CColliderBox2D::Render(float DeltaTime)
{
	CCollider::Render(DeltaTime);
}

CColliderBox2D* CColliderBox2D::Clone()
{
	return new CColliderBox2D(*this);
}

bool CColliderBox2D::Collision(CCollider* Dest)
{
	bool	result = false;

	switch (Dest->GetColliderShape())
	{
	case Collider_Shape::Box2D:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToBox2D(m_HitResult, DestResult,
			this, (CColliderBox2D*)Dest);
		Dest->SetHitResult(DestResult);

		if (result)
		{
			// 서로 Block이라면
			if (m_Profile->vecChannel[(int)Dest->GetProfile()->Channel].Interaction == Collision_Interaction::Block
				&& Dest->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction == Collision_Interaction::Block)
			{
				CGameObject* SrcObj = GetOwner();
				CGameObject* DestObj = Dest->GetOwner();

				Box2DInfo SrcInfo = GetInfo();
				SrcInfo.Center.x -= SrcObj->GetMove().x;
				SrcInfo.Center.y -= SrcObj->GetMove().y;

				CColliderBox2D* DestBox = Cast<CColliderBox2D*>(Dest);
				Box2DInfo DestInfo = DestBox->GetInfo();
				DestInfo.Center.x -= DestObj->GetMove().x;
				DestInfo.Center.y -= DestObj->GetMove().y;
				

				// Src이 Dest의 위에 있을 때 블락 판단
				if (DestInfo.Center.y + DestInfo.Length[1] <= SrcInfo.Center.y - SrcInfo.Length[1] && DestInfo.Center.y < SrcInfo.Center.y)
				{
					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y <= 0)
					{
						SrcObj->SetWorldPosY(DestInfo.Center.y + DestInfo.Length[1] + (GetPivot().y * GetWorldScale().y));
						SrcObj->OnFloor();

						AddGroundCollision(Dest);
					}

					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y > 0)
					{
						// 위에 부딪혔을 때 실행할 함수 실행하기.
						DestObj->SetWorldPosY(SrcInfo.Center.y - SrcInfo.Length[1] - ((1.f - Dest->GetPivot().y) * Dest->GetWorldScale().y));
						DestObj->Falling();
					}
				}
				// Src이 Dest의 아래에 있을 때 블락 판단
				else if (SrcInfo.Center.y + SrcInfo.Length[1] <= DestInfo.Center.y - DestInfo.Length[1] && SrcInfo.Center.y < DestInfo.Center.y)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y <= 0)
					{
						DestObj->SetWorldPosY(SrcInfo.Center.y + SrcInfo.Length[1] + (Dest->GetPivot().y * Dest->GetWorldScale().y));
						DestObj->OnFloor();

						AddGroundCollision(Dest);
					}

					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y > 0)
					{
						// 위에 부딪혔을 때 실행할 함수 실행하기.
						SrcObj->SetWorldPosY(DestInfo.Center.y - DestInfo.Length[1] - ((1.f - GetPivot().y) * GetWorldScale().y));
						SrcObj->Falling();
					}
				}
				// Dest가 왼쪽 Src가 오른쪽에 있을 때
				else if (DestInfo.Center.x + DestInfo.Length[0] <= SrcInfo.Center.x - SrcInfo.Length[0] && DestInfo.Center.x < SrcInfo.Center.x)
				{
					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().x < 0)
					{
						SrcObj->SetWorldPosX(DestInfo.Center.x + DestInfo.Length[0] + (GetPivot().x * GetWorldScale().x));
					}

					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().x > 0)
					{
						DestObj->SetWorldPosX(SrcInfo.Center.x - SrcInfo.Length[0] - ((1.f - Dest->GetPivot().x) * Dest->GetWorldScale().x));
					}
				}
				// Src가 왼쪽 Dest가 오른쪽에 있을 때
				else if (SrcInfo.Center.x + SrcInfo.Length[0] <= DestInfo.Center.x - DestInfo.Length[0] &&  SrcInfo.Center.x < DestInfo.Center.x)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().x < 0)
					{
						DestObj->SetWorldPosX(SrcInfo.Center.x + SrcInfo.Length[0] + (Dest->GetPivot().x * Dest->GetWorldScale().x));
					}

					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().x > 0)
					{
						SrcObj->SetWorldPosX(DestInfo.Center.x - DestInfo.Length[0] - ((1.f - GetPivot().x) * GetWorldScale().x));
					}
				}
			}
		}
	}
	break;
	case Collider_Shape::Circle:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToCircle(m_HitResult, DestResult,
			this, (CColliderCircle*)Dest);
		Dest->SetHitResult(DestResult);

		if (result)
		{
			if (m_Profile->vecChannel[(int)Dest->GetProfile()->Channel].Interaction == Collision_Interaction::Block
				&& Dest->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction == Collision_Interaction::Block)
			{
				CGameObject* SrcObj = GetOwner();
				CGameObject* DestObj = Dest->GetOwner();

				Box2DInfo SrcInfo = GetInfo();
				SrcInfo.Center.x -= SrcObj->GetMove().x;
				SrcInfo.Center.y -= SrcObj->GetMove().y;

				CColliderCircle* DestCircle = Cast<CColliderCircle*>(Dest);
				CircleInfo DestInfo = DestCircle->GetInfo();
				DestInfo.Center.x -= DestObj->GetMove().x;
				DestInfo.Center.y -= DestObj->GetMove().y;

				// Src이 Dest의 위에 있을 때 블락 판단
				if (DestInfo.Center.y + DestInfo.Radius <= SrcInfo.Center.y - SrcInfo.Length[1] && DestInfo.Center.y < SrcInfo.Center.y)
				{
					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y < 0)
					{
						SrcObj->SetWorldPosY(DestInfo.Center.y + DestInfo.Radius + (GetPivot().y * GetWorldScale().y));
						SrcObj->OnFloor();

						AddGroundCollision(Dest);
					}

					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y > 0)
					{
						// 위에 부딪혔을 때 실행할 함수 실행하기.
						DestObj->SetWorldPosY(SrcInfo.Center.y - SrcInfo.Length[1] - ((1.f - Dest->GetPivot().y) * Dest->GetWorldScale().y * 2.f));
						if (DestObj->GetPhysicsSimulate())
							DestObj->Falling();
					}
				}

				// Src이 Dest의 아래에 있을 때 블락 판단
				else if (SrcInfo.Center.y + SrcInfo.Length[1] <= DestInfo.Center.y - DestInfo.Radius && SrcInfo.Center.y < DestInfo.Center.y)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y < 0)
					{
						DestObj->SetWorldPosY(SrcInfo.Center.y + SrcInfo.Length[1] + (Dest->GetPivot().y * Dest->GetWorldScale().y * 2.f));
						DestObj->OnFloor();

						Dest->AddGroundCollision(this);
					}

					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y > 0)
					{
						// 위에 부딪혔을 때 실행할 함수 실행하기.
						SrcObj->SetWorldPosY(DestInfo.Center.y - DestInfo.Radius - ((1.f - GetPivot().y) * GetWorldScale().y));
						if (SrcObj->GetPhysicsSimulate())
							SrcObj->Falling();
					}
				}
				// Dest가 왼쪽 Src가 오른쪽에 있을 때
				else if (DestInfo.Center.x + DestInfo.Radius <= SrcInfo.Center.x - SrcInfo.Length[0] && DestInfo.Center.x < SrcInfo.Center.x)
				{
					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().x < 0)
					{
						SrcObj->SetWorldPosX(DestInfo.Center.x + DestInfo.Radius + (GetPivot().x * GetWorldScale().x));
					}

					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().x > 0)
					{
						DestObj->SetWorldPosX(SrcInfo.Center.x - SrcInfo.Length[0] - ((1.f - Dest->GetPivot().x) * Dest->GetWorldScale().x * 2.f));
					}
				}
				// Src가 왼쪽 Dest가 오른쪽에 있을 때
				else if (SrcInfo.Center.x + SrcInfo.Length[0] <= DestInfo.Center.x - DestInfo.Radius && SrcInfo.Center.x < DestInfo.Center.x)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().x < 0)
					{
						DestObj->SetWorldPosX(SrcInfo.Center.x + SrcInfo.Length[0] + (Dest->GetPivot().x * Dest->GetWorldScale().x * 2.f));
					}

					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().x > 0)
					{
						SrcObj->SetWorldPosX(DestInfo.Center.x - DestInfo.Radius - ((1.f - GetPivot().x) * GetWorldScale().x));
					}
				}
			}
		}
	}
	break;
	case Collider_Shape::Pixel:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToPixel(m_HitResult, DestResult,
			this, (CColliderPixel*)Dest);
		Dest->SetHitResult(DestResult);
	}
	break;
	case Collider_Shape::Line:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionLineToBox2D(DestResult, m_HitResult,
			(CColliderLine*)Dest, this);
		Dest->SetHitResult(DestResult);

		if (result)
		{
			// 서로 Block이라면
			if (m_Profile->vecChannel[(int)Dest->GetProfile()->Channel].Interaction == Collision_Interaction::Block
				&& Dest->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction == Collision_Interaction::Block)
			{
				CGameObject* SrcObj = GetOwner();
				CGameObject* DestObj = Dest->GetOwner();

				Box2DInfo SrcInfo = GetInfo();
				SrcInfo.Min.y -= SrcObj->GetMove().y;
				CColliderLine* DestLine = Cast<CColliderLine*>(Dest);
				LineInfo DestInfo = DestLine->GetInfo();
				


				// Dest가 Src의 아래에 있을 때 블락 판단
				if (DestInfo.Start.y <= SrcInfo.Min.y)
				{
					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y < 0)
					{
						SrcObj->SetWorldPosY(DestInfo.Start.y + (GetPivot().y * GetWorldScale().y));
						SrcObj->OnFloor();

						AddGroundLineCollision(DestLine);
					}
				}
			}
		}
	}
	break;
	}

	return result;
}

bool CColliderBox2D::CollisionMouse(const Vector2& MousePos)
{
	HitResult	SrcResult = {};
	HitResult	DestResult = {};

	m_MouseCollision = CCollision::CollisionBox2DToPoint(SrcResult, DestResult,
		m_Info, MousePos);

	return m_MouseCollision;
}
