
#include "ColliderCircle.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "ColliderBox2D.h"
#include "ColliderPixel.h"
#include "ColliderLine.h"

CColliderCircle::CColliderCircle()
{
	m_ColliderShape = Collider_Shape::Circle;
	m_ColliderSpace = Collider_Space::Collider2D;

	m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
	m_2DType = RT2D_Default;

	m_Info.Radius = 50.f;
}

CColliderCircle::CColliderCircle(const CColliderCircle& com) :
	CCollider(com)
{
	m_Info = com.m_Info;
}

CColliderCircle::~CColliderCircle()
{
}

void CColliderCircle::Start()
{
	CCollider::Start();

	m_DebugMesh = m_pScene->GetResource()->FindMesh("ColliderCircle");
}

bool CColliderCircle::Init()
{
	if (!CCollider::Init())
		return false;

	SetUpdatePosZ(true);

	return true;
}

void CColliderCircle::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderCircle::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	m_Info.Center.x = GetWorldPos().x + ((0.5f - GetPivot().x) * GetWorldScale().x);
	m_Info.Center.y = GetWorldPos().y + ((0.5f - GetPivot().y) * GetWorldScale().y);

	Vector2	Pos[4] = {};

	Pos[0] = m_Info.Center - Vector2(1.f, 0.f) * m_Info.Radius +
		Vector2(0.f, 1.f) * m_Info.Radius;
	Pos[1] = m_Info.Center + Vector2(1.f, 0.f) * m_Info.Radius +
		Vector2(0.f, 1.f) * m_Info.Radius;

	Pos[2] = m_Info.Center - Vector2(1.f, 0.f) * m_Info.Radius -
		Vector2(0.f, 1.f) * m_Info.Radius;
	Pos[3] = m_Info.Center + Vector2(1.f, 0.f) * m_Info.Radius -
		Vector2(0.f, 1.f) * m_Info.Radius;

	m_Min.x = Pos[0].x;
	m_Min.y = Pos[0].y;

	m_Max.x = Pos[0].x;
	m_Max.y = Pos[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (m_Min.x > Pos[i].x)
			m_Min.x = Pos[i].x;

		if (m_Min.y > Pos[i].y)
			m_Min.y = Pos[i].y;

		if (m_Max.x < Pos[i].x)
			m_Max.x = Pos[i].x;

		if (m_Max.y < Pos[i].y)
			m_Max.y = Pos[i].y;
	}

	m_Info.Min = m_Min;
	m_Info.Max = m_Max;
}

void CColliderCircle::Collision(float DeltaTime)
{
	CCollider::Collision(DeltaTime);
}

void CColliderCircle::PrevRender(float DeltaTime)
{
	CCollider::PrevRender(DeltaTime);
}

void CColliderCircle::Render(float DeltaTime)
{
	CCollider::Render(DeltaTime);
}

CColliderCircle* CColliderCircle::Clone()
{
	return new CColliderCircle(*this);
}

bool CColliderCircle::Collision(CCollider* Dest)
{
	bool	result = false;

	switch (Dest->GetColliderShape())
	{
	case Collider_Shape::Box2D:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToCircle(DestResult, m_HitResult,
			(CColliderBox2D*)Dest, this);
		Dest->SetHitResult(DestResult);

		if (result)
		{

			if (m_Profile->vecChannel[(int)Dest->GetProfile()->Channel].Interaction == Collision_Interaction::Block
				&& Dest->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction == Collision_Interaction::Block)
			{
				CGameObject* SrcObj = GetOwner();
				CGameObject* DestObj = Dest->GetOwner();

				CircleInfo SrcInfo = GetInfo();
				SrcInfo.Center.x -= SrcObj->GetMove().x;
				SrcInfo.Center.y -= SrcObj->GetMove().y;

				CColliderBox2D* DestBox = Cast<CColliderBox2D*>(Dest);
				Box2DInfo DestInfo = DestBox->GetInfo();
				DestInfo.Center.x -= DestObj->GetMove().x;
				DestInfo.Center.y -= DestObj->GetMove().y;

				// Src이 Dest의 위에 있을 때 블락 판단
				if (DestInfo.Center.y + DestInfo.Length[1] <= SrcInfo.Center.y - SrcInfo.Radius && DestInfo.Center.y < SrcInfo.Center.y)
				{
					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y <= 0)
					{
						SrcObj->SetWorldPosY(DestInfo.Center.y + DestInfo.Length[1] + (GetPivot().y * GetWorldScale().y * 2.f));
						SrcObj->OnFloor();

						AddGroundCollision(Dest);
					}

					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y >= 0)
					{
						// 위에 부딪혔을 때 실행할 함수 실행하기.
						DestObj->SetWorldPosY(SrcInfo.Center.y - SrcInfo.Radius - ((1.f - Dest->GetPivot().y) * Dest->GetWorldScale().y));
						if (DestObj->GetPhysicsSimulate())
							DestObj->Falling();
					}
				}

				// Src이 Dest의 아래에 있을 때 블락 판단
				else if (SrcInfo.Center.y + SrcInfo.Radius <= DestInfo.Center.y - DestInfo.Length[1] && SrcInfo.Center.y < DestInfo.Center.y)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y < 0)
					{
						DestObj->SetWorldPosY(SrcInfo.Center.y + SrcInfo.Radius + (Dest->GetPivot().y * Dest->GetWorldScale().y));
						DestObj->OnFloor();

						Dest->AddGroundCollision(this);
					}

					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y > 0)
					{
						// 위에 부딪혔을 때 실행할 함수 실행하기.
						SrcObj->SetWorldPosY(DestInfo.Center.y - DestInfo.Length[1] - ((1.f - GetPivot().y) * GetWorldScale().y * 2.f));
						if (SrcObj->GetPhysicsSimulate())
							SrcObj->Falling();
					}
				}
				// Dest가 왼쪽 Src가 오른쪽에 있을 때
				else if (DestInfo.Center.x + DestInfo.Length[0] <= SrcInfo.Center.x - SrcInfo.Radius && DestInfo.Center.x < SrcInfo.Center.x)
				{
					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().x < 0)
					{
						SrcObj->SetWorldPosX(DestInfo.Center.x + DestInfo.Length[0] + (GetPivot().x * GetWorldScale().x * 2.f));
					}

					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().x > 0)
					{
						DestObj->SetWorldPosX(SrcInfo.Center.x - SrcInfo.Radius - ((1.f - Dest->GetPivot().x) * Dest->GetWorldScale().x));
					}
				}
				// Src가 왼쪽 Dest가 오른쪽에 있을 때
				else if (SrcInfo.Center.x + SrcInfo.Radius <= DestInfo.Center.x - DestInfo.Length[0] && SrcInfo.Center.x < DestInfo.Center.x)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().x < 0)
					{
						DestObj->SetWorldPosX(SrcInfo.Center.x + SrcInfo.Radius + (Dest->GetPivot().x * Dest->GetWorldScale().x));
					}

					if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().x > 0)
					{
						SrcObj->SetWorldPosX(DestInfo.Center.x - DestInfo.Length[0] - ((1.f - GetPivot().x) * GetWorldScale().x * 2.f));
					}
				}

				else
					int a = 10;
			}
		}
	}
		break;
	case Collider_Shape::Circle:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionCircleToCircle(m_HitResult, DestResult,
			this, (CColliderCircle*)Dest);
		Dest->SetHitResult(DestResult);
	}
		break;
	case Collider_Shape::Pixel:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionCircleToPixel(m_HitResult, DestResult,
			this, (CColliderPixel*)Dest);
		Dest->SetHitResult(DestResult);
	}
	break;
	case Collider_Shape::Line:
	{
        HitResult	DestResult = {};
        result = CCollision::CollisionLineToCircle(m_HitResult, DestResult,
            (CColliderLine*)Dest, this);
        Dest->SetHitResult(DestResult);

        if (result)
        {
            if (m_Profile->vecChannel[(int)Dest->GetProfile()->Channel].Interaction == Collision_Interaction::Block
                && Dest->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction == Collision_Interaction::Block)
            {
				CGameObject* SrcObj = GetOwner();
                CGameObject* DestObj = Dest->GetOwner();
                LineInfo DestInfo = ((CColliderLine*)Dest)->GetInfo();

                if (GetInfo().Center.x >= DestInfo.Start.x && GetInfo().Center.x <= DestInfo.End.x)
                {
                    if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y < 0)
                    {
                        SetWorldPosY(DestInfo.Start.y + GetInfo().Radius);
                        OnFloor();

						AddGroundCollision(Dest);
                    }
                }

                else
                {
                    if (SrcObj->GetMoveable() && SrcObj->GetRootComponent()->GetMove().y < 0)
                    {
                        /*DestObj->SetWorldPosY(GetInfo().Start.y + DestInfo.Radius);*/
						if (GetInfo().Center.x < DestInfo.Start.x)
						{
							Vector2 CenterToStart = DestInfo.Start - GetInfo().Center;
                            Vector2 CenterToStart2 = CenterToStart * GetInfo().Radius / CenterToStart.Length();
         
                            Vector2 Move = (CenterToStart2 - CenterToStart) * -1.f;\
                            DestObj->AddWorldPos(Move.x, 0.f, 0.f);

							//SrcObj->AddWorldPos(-Vector2(1.f, 0.f).Dot(CenterToStart) / CenterToStart.Length() * 0.1f, 0.f, 0.f);
						}
						else
						{
                            Vector2 CenterToEnd= DestInfo.End - GetInfo().Center;
                            Vector2 CenterToEnd2 = CenterToEnd * GetInfo().Radius / CenterToEnd.Length();\
                            Vector2 Move = CenterToEnd2 - CenterToEnd;\

                            DestObj->AddWorldPos(Move.x, 0.f, 0.f);
                            //SrcObj->AddWorldPos(Vector2(1.f, 0.f).Dot(CenterToEnd) / CenterToEnd.Length() * 0.1f, 0.f, 0.f);
						}
						OnFloor();

						AddGroundCollision(Dest);
                    }
                }
            }
        }
	}
	break;
	}

	return result;
}

bool CColliderCircle::CollisionMouse(const Vector2& MousePos)
{
	HitResult	SrcResult = {};
	HitResult	DestResult = {};

	m_MouseCollision = CCollision::CollisionCircleToPoint(SrcResult, DestResult,
		m_Info, MousePos);

	return m_MouseCollision;
}
