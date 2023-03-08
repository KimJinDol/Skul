
#include "CollisionSection.h"
#include "../Component/Collider.h"
#include "../Input.h"
#include "../GameObject.h"

CCollisionSection::CCollisionSection()
{
	m_vecCollider.reserve(300);
}

CCollisionSection::~CCollisionSection()
{
}

bool CCollisionSection::Init(int IndexX, int IndexY, int IndexZ, int Index, 
	const Vector3& SectionSize, const Vector3& SectionTotalSize)
{
	m_IndexX = IndexX;
	m_IndexY = IndexY;
	m_IndexZ = IndexZ;
	m_Index = Index;
	m_SectionSize = SectionSize;
	m_SectionTotalSize = SectionTotalSize;

	return true;
}

void CCollisionSection::SetSectionMin(const Vector3& Min)
{
	m_Min = Min;
}

void CCollisionSection::SetSectionMax(const Vector3& Max)
{
	m_Max = Max;
}

void CCollisionSection::AddCollider(CCollider* Collider)
{
	m_vecCollider.push_back(Collider);

	Collider->AddSectionIndex(m_Index);
}

void CCollisionSection::Clear()
{
	m_vecCollider.clear();
}

void CCollisionSection::Collision(float DeltaTime)
{
	size_t	Size = m_vecCollider.size();

	if (Size < 2)
		return;


	for (size_t i = 0; i < Size - 1; ++i)
	{
		CCollider* Src = m_vecCollider[i];

		if (!Src->IsEnable())
			continue;
		
	
		for (size_t j = i + 1; j < Size; ++j)
		{
			CCollider* Dest = m_vecCollider[j];

			if (!Dest->IsEnable())
				continue;

			// ���� �����ӿ� �ٸ� ���ǿ��� ���� �浹ó���� �غ����� ���� ���
			if (Src->CheckCurrentFrameCollision(Dest))
				continue;

			CollisionProfile* SrcProfile = Src->GetProfile();
			CollisionProfile* DestProfile = Dest->GetProfile();

			Collision_Interaction SrcInteraction = SrcProfile->vecChannel[(int)DestProfile->Channel].Interaction;
			Collision_Interaction DestInteraction = DestProfile->vecChannel[(int)SrcProfile->Channel].Interaction;

	
			if (SrcInteraction == Collision_Interaction::Ignore ||
				DestInteraction == Collision_Interaction::Ignore)
				continue;

			if (Src->Collision(Dest))
			{
				// ������ �� �浹ü�� �浹�ߴ����� �Ǵ��Ѵ�.
				// �����浹ü�� ���ٸ� ���� �� �浹�� �����Ѵٴ� �ǹ��̴�.
				if (!Src->CheckPrevCollision(Dest))
				{
					Src->AddPrevCollider(Dest);
					Dest->AddPrevCollider(Src);

			
					// Callback �Լ� ȣ��
					if (SrcInteraction == Collision_Interaction::Overlap || DestInteraction == Collision_Interaction::Overlap)
					{
						Src->CallCollisionOverlapCallback(Collision_State::Begin);
						Dest->CallCollisionOverlapCallback(Collision_State::Begin);
					}

					else
					{
						Src->CallCollisionBlockCallback(Collision_State::Begin);
						Dest->CallCollisionBlockCallback(Collision_State::Begin);
					}
				}
			}

			// �浹�� �ƴ϶�� ������ �浹�Ǿ��� ��ü������ �Ǵ��Ѵ�.
			// ������ �浹�Ǿ��� ��ü��� �ε��� ��ü�� ���� �������ٴ� �ǹ��̴�.
			else if (Src->CheckPrevCollision(Dest))
			{
				Src->DeletePrevCollider(Dest);
				Dest->DeletePrevCollider(Src);

				// Callback �Լ� ȣ��
				if (SrcInteraction == Collision_Interaction::Overlap || DestInteraction == Collision_Interaction::Overlap)
				{
					Src->CallCollisionOverlapCallback(Collision_State::End);
					Dest->CallCollisionOverlapCallback(Collision_State::End);
				}

				else
				{
					Src->CallCollisionBlockCallback(Collision_State::End);
					Dest->CallCollisionBlockCallback(Collision_State::End);
				}
			}
		}
	}
}

CCollider* CCollisionSection::CollisionMouse(bool Is2D, float DeltaTime)
{
	if (Is2D)
	{
		Vector2	MousePos = CInput::GetInst()->GetMouse2DWorldPos();

		size_t	Size = m_vecCollider.size();

		if (Size > 1)
		{
			qsort(&m_vecCollider[0], Size, (size_t)sizeof(CCollider*),
				CCollisionSection::SortY);
		}

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecCollider[i]->CollisionMouse(MousePos))
				return m_vecCollider[i];
		}
	}

	else
	{
	}

	return nullptr;
}

int CCollisionSection::SortY(const void* Src, const void* Dest)
{
	CCollider* SrcCollider = *((CCollider**)Src);
	CCollider* DestCollider = *((CCollider**)Dest);

	float	SrcY = SrcCollider->GetWorldPos().y;
	float	DestY = SrcCollider->GetWorldPos().y;

	if (SrcY < DestY)
		return -1;

	else if (SrcY > DestY)
		return 1;

	return 0;
}

int CCollisionSection::SortZ(const void* Src, const void* Dest)
{
	return 0;
}