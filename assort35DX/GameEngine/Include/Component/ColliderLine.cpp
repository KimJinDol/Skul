#include "ColliderLine.h"
#include "ColliderBox2D.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "ColliderCircle.h"
#include "ColliderPixel.h"

CColliderLine::CColliderLine()
{
    m_ColliderShape = Collider_Shape::Line;
    m_ColliderSpace = Collider_Space::Collider2D;

    m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
    m_2DType = RT2D_Particle;
}

CColliderLine::CColliderLine(const CColliderLine& com)
{
    m_Info = com.m_Info;
}

CColliderLine::~CColliderLine()
{

}

void CColliderLine::Start()
{
    CCollider::Start();

    m_DebugMesh = m_pScene->GetResource()->FindMesh("ColliderLine");
}

bool CColliderLine::Init()
{
    if (!CCollider::Init())
        return false;

    //m_Info.[AXIS_X] = Vector2(1.f, 0.f);
    //m_Info.Axis[AXIS_Y] = Vector2(0.f, 1.f);

    SetInheritRotZ(true);

    SetUpdatePosZ(true);

    return true;
}

void CColliderLine::Update(float DeltaTime)
{
    CCollider::Update(DeltaTime);
}

void CColliderLine::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	//m_Info.Start.x = GetWorldPos().x - ((GetPivot().x) * GetWorldScale().x);
	//m_Info.End.x = GetWorldPos().x + ((1.f - GetPivot().x) * GetWorldScale().x);


	m_Min.x = m_Info.Start.x;
	m_Min.y = m_Info.Start.y;

	m_Max.x = m_Info.End.x;
	m_Max.y = m_Info.End.y;


	if (m_Min.x > m_Max.x)
	{
		float Temp = m_Min.x;
		m_Min.x = m_Max.x;
		m_Max.x = Temp;
	}
	if (m_Min.y > m_Max.y)
	{
		float Temp = m_Min.y;
		m_Min.y = m_Max.y;
		m_Max.y = Temp;
	}
}

void CColliderLine::Collision(float DeltaTime)
{
	CCollider::Collision(DeltaTime);
}

void CColliderLine::PrevRender(float DeltaTime)
{
	CCollider::PrevRender(DeltaTime);
}

void CColliderLine::Render(float DeltaTime)
{
	CCollider::Render(DeltaTime);
}

CColliderLine* CColliderLine::Clone()
{
	return new CColliderLine(*this);
}

bool CColliderLine::CollisionMouse(const Vector2& MousePos)
{
	return false;
}


bool CColliderLine::Collision(CCollider* Dest)
{
	bool	result = false;

	switch (Dest->GetColliderShape())
	{
	case Collider_Shape::Box2D:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionLineToBox2D(m_HitResult, DestResult,
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

				LineInfo SrcInfo = GetInfo();

				CColliderBox2D* DestBox = Cast<CColliderBox2D*>(Dest);
				Box2DInfo DestInfo = DestBox->GetInfo();
				DestInfo.Min.y -= DestObj->GetMove().y;
				


				// Src이 Dest의 아래에 있을 때 블락 판단
				if (SrcInfo.Start.y <= DestInfo.Min.y)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y < 0)
					{
						DestObj->SetWorldPosY(SrcInfo.Start.y + (Dest->GetPivot().y * Dest->GetWorldScale().y));
						DestObj->OnFloor();

						Dest->AddGroundLineCollision(this);
					}
				}
			}
		}
	}
	break;
	case Collider_Shape::Circle:
	{
        HitResult	DestResult = {};
        result = CCollision::CollisionLineToCircle(m_HitResult, DestResult,
            this, (CColliderCircle*)Dest);
        Dest->SetHitResult(DestResult);

		if (result)
		{
			if (m_Profile->vecChannel[(int)Dest->GetProfile()->Channel].Interaction == Collision_Interaction::Block
				&& Dest->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction == Collision_Interaction::Block)
			{
				CGameObject* DestObj = Dest->GetOwner();
				CircleInfo DestInfo = ((CColliderCircle*)Dest)->GetInfo();

				if (DestInfo.Center.x >= GetInfo().Start.x && DestInfo.Center.x <= GetInfo().End.x)
				{
					if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y < 0)
					{
						DestObj->SetWorldPosY(GetInfo().Start.y + DestInfo.Radius);
						DestObj->OnFloor();

						Dest->AddGroundLineCollision(this);
					}
				}

				else
				{
                    if (DestObj->GetMoveable() && DestObj->GetRootComponent()->GetMove().y < 0)
                    {
                        /*DestObj->SetWorldPosY(GetInfo().Start.y + DestInfo.Radius);*/
                        if (DestInfo.Center.x < GetInfo().Start.x)
                        {
							
                            Vector2 CenterToStart = GetInfo().Start - DestInfo.Center;
							Vector2 CenterToStart2 = CenterToStart * DestInfo.Radius / CenterToStart.Length();
							float Angle = Vector3(1.f, 0.f, 0.f).Angle(Vector3(CenterToStart.x, CenterToStart.y, 0.f));
							
							float CosTheta = cos(DegreeToRadian(Angle));
							Vector2 Move = (CenterToStart2 - CenterToStart) * -1.f;
							Move *= CosTheta;
							DestObj->AddWorldPos(Move.x , 0.f, 0.f);
							
							//DestObj->AddWorldPos(-Vector2(1.f, 0.f).Dot(CenterToStart) / CenterToStart.Length(), 0.f, 0.f);
                        }
                        else
                        {
                            Vector2 CenterToEnd = GetInfo().End - DestInfo.Center;
                            Vector2 CenterToEnd2 = CenterToEnd * DestInfo.Radius / CenterToEnd.Length();
                            float Angle = Vector3(1.f, 0.f, 0.f).Angle(Vector3(CenterToEnd.x, CenterToEnd.y, 0.f));

                            float CosTheta = cos(DegreeToRadian(Angle));
                            Vector2 Move = CenterToEnd2 - CenterToEnd;
							Move *= CosTheta;
							DestObj->AddWorldPos(Move.x, 0.f, 0.f);
							//DestObj->AddWorldPos(Vector2(1.f, 0.f).Dot(CenterToEnd) / CenterToEnd.Length(), 0.f, 0.f);
                        }
                        DestObj->OnFloor();

						Dest->AddGroundLineCollision(this);
                    }
				}
			}
		}
	}
	break;
	}

	return result;
}

