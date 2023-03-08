
#include "Collider.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../CollisionManager.h"
#include "../Resource/ColliderConstantBuffer.h"
#include "../Component/Transform.h"

CCollider::CCollider()
{
	m_SceneComponentType = SceneComponent_Type::Primitive;
	m_ColliderSpace = Collider_Space::Collider3D;
	m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;
	m_3DType = RT3D_Default;

	m_Profile = nullptr;
	m_CurrentSectionCheck = false;

	m_DebugRender = false;
#ifdef _DEBUG
	m_DebugRender = true;
#endif

	m_CBuffer = nullptr;

	m_MouseCollision = false;

	m_GroundCollisionList.clear();
}

CCollider::CCollider(const CCollider& com) :
	CPrimitiveComponent(com)
{
	m_MouseCollision = false;

	m_ColliderShape = com.m_ColliderShape;
	m_ColliderSpace = com.m_ColliderSpace;
	m_Profile = com.m_Profile;
	m_CurrentSectionCheck = false;
	m_DebugMesh = com.m_DebugMesh;
	m_DebugShader = com.m_DebugShader;
	m_CBuffer = com.m_CBuffer->Clone();
	m_DebugRender = com.m_DebugRender;
}

CCollider::~CCollider()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();


	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeletePrevCollider(this);

		Collision_Interaction SrcInteraction = GetProfile()->vecChannel[(int)(*iter)->GetProfile()->Channel].Interaction;
		Collision_Interaction DestInteraction = (*iter)->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction;

		if (SrcInteraction == Collision_Interaction::Overlap || DestInteraction == Collision_Interaction::Overlap)
		{
			(*iter)->CallCollisionOverlapCallback(Collision_State::End);
			CallCollisionOverlapCallback(Collision_State::End);
		}
		else
		{
			(*iter)->CallCollisionBlockCallback(Collision_State::End);
			CallCollisionBlockCallback(Collision_State::End);
		}
	}

	SAFE_DELETE(m_CBuffer);
}				

void CCollider::AddGroundCollision(CCollider* Collider)
{
	m_GroundCollisionList.push_back(Collider);
}

void CCollider::AllRemoveGroundCollision()
{
	m_GroundCollisionList.clear();
}

void CCollider::AddGroundLineCollision(CColliderLine* Collider)
{
	m_GroundLineCollisionList.push_back(Collider);
}

void CCollider::AllRemoveGroundLineCollision()
{
	m_GroundLineCollisionList.clear();
}

void CCollider::SetColliderPivot2D(const Vector3& Pivot)
{
	//m_ColliderPivot = Pivot;
	//Vector3 Pos = (m_ColliderPivot - Vector3(0.5f, 0.5f, 0.f)) * GetWorldScale();
	//SetRelativePosX(Pos.x * -1.f);
	//SetRelativePosY(Pos.y * -1.f);
}

void CCollider::SetColliderPivot2D(float x, float y, float z)
{
	//m_ColliderPivot.x = x;
	//m_ColliderPivot.y = y;
	//m_ColliderPivot.z = z;
	//Vector3 Pos = (m_ColliderPivot - Vector3(0.5f, 0.5f, 0.f)) * GetWorldScale();
	//SetRelativePosX(Pos.x * -1.f);
	//SetRelativePosY(Pos.y * -1.f);
}


void CCollider::SetCollisionProfile(const std::string& Name)
{
	m_Profile = CCollisionManager::GetInst()->FindProfile(Name);
}

void CCollider::CheckPrevColliderSection()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd;)
	{
		bool	Check = false;

		// 현재 호출된 충돌체의 섹션과 가지고 있는 이전프레임에 충돌한 충돌체의 섹션을 
		// 비교하여 겹치는 섹션이 있는지 판단한다.
		size_t	Size = m_vecSectionIndex.size();

		for (size_t i = 0; i < Size; ++i)
		{
			size_t DestSize = (*iter)->m_vecSectionIndex.size();

			for (size_t j = 0; j < DestSize; ++j)
			{
				if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				{
					Check = true;
					break;
				}
			}

			if (Check)
				break;
		}

		// 충돌체끼리 서로 겹치는 충돌영역이 없다면 충돌 되었다가 떨어지는 처리를 한다.
		if (!Check)
		{

			Collision_Interaction SrcInteraction = GetProfile()->vecChannel[(int)(*iter)->GetProfile()->Channel].Interaction;
			Collision_Interaction DestInteraction = (*iter)->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction;

			if (SrcInteraction == Collision_Interaction::Overlap || DestInteraction == Collision_Interaction::Overlap)
			{
				CallCollisionOverlapCallback(Collision_State::End);
				(*iter)->CallCollisionOverlapCallback(Collision_State::End);
			}

			else
			{
				CallCollisionBlockCallback(Collision_State::End);
				(*iter)->CallCollisionOverlapCallback(Collision_State::End);
			}

			// 상대 충돌체에서 나를 이전충돌목록에서 제거해준다.
			(*iter)->DeletePrevCollider(this);

			iter = m_PrevCollisionList.erase(iter);
			iterEnd = m_PrevCollisionList.end();
			continue;
		}

		++iter;
	}
}

void CCollider::AddPrevCollider(CCollider* Collider)
{
	m_PrevCollisionList.push_back(Collider);
}

void CCollider::DeletePrevCollider(CCollider* Collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter) 
	{
		if (*iter == Collider)
		{
			m_PrevCollisionList.erase(iter);
			return;
		}
	}
}

bool CCollider::EmptyPrevCollision()
{
	return m_PrevCollisionList.empty();
}

bool CCollider::CheckPrevCollision(CCollider* Collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

bool CCollider::CheckCurrentFrameCollision(CCollider* Collider)
{
	auto	iter = m_CurrentCollisionList.begin();
	auto	iterEnd = m_CurrentCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

void CCollider::AddCurrentFrameCollision(CCollider* Collider)
{
	if (!CheckCurrentFrameCollision(Collider))
		m_CurrentCollisionList.push_back(Collider);
}

void CCollider::DeleteCurrentFrameCollision(CCollider* Collider)
{
	if (CheckCurrentFrameCollision(Collider))
		m_CurrentCollisionList.remove(Collider);
}

void CCollider::DeleteCurrentFrameCollision(size_t TypeId)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd;)
	{
		if (typeid(*iter).hash_code() == TypeId)
		{
			iter = m_PrevCollisionList.erase(iter);
			iterEnd = m_PrevCollisionList.end();

			continue;
		}
		iter++;
	}

	iter = m_CurrentCollisionList.begin();
	iterEnd = m_CurrentCollisionList.end();

	for (; iter != iterEnd;)
	{
		if (typeid(*iter).hash_code() == TypeId)
		{
			iter = m_CurrentCollisionList.erase(iter);
			iterEnd = m_CurrentCollisionList.end();

			continue;
		}
		iter++;
	}
}

void CCollider::DeleteAllPrevCollision()
{
	if (m_PrevCollisionList.size() == 0) 
		return;

	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	Collision_Interaction SrcInteraction;
	Collision_Interaction DestInteraction;

	for (; iter != iterEnd;)
	{

		SrcInteraction = GetProfile()->vecChannel[(int)(*iter)->GetProfile()->Channel].Interaction;
		DestInteraction = (*iter)->GetProfile()->vecChannel[(int)GetProfile()->Channel].Interaction;

		if (SrcInteraction == Collision_Interaction::Overlap || DestInteraction == Collision_Interaction::Overlap)
		{
			CallCollisionOverlapCallback(Collision_State::End);
			(*iter)->CallCollisionOverlapCallback(Collision_State::End);
		}
		else
		{
			CallCollisionBlockCallback(Collision_State::End);
			(*iter)->CallCollisionBlockCallback(Collision_State::End);
		}

		(*iter)->DeletePrevCollider(this);

		iter = m_PrevCollisionList.erase(iter);
		iterEnd = m_PrevCollisionList.end();
	}
}

void CCollider::CallCollisionOverlapCallback(Collision_State State)
{
	auto	iter = m_CollisionOverlapCallback[(int)State].begin();
	auto	iterEnd = m_CollisionOverlapCallback[(int)State].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(m_HitResult, this);
	}
}

void CCollider::CallCollisionBlockCallback(Collision_State State)
{
	auto	iter = m_CollisionBlockCallback[(int)State].begin();
	auto	iterEnd = m_CollisionBlockCallback[(int)State].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(m_HitResult, this);
	}
}

void CCollider::CallCollisionMouseCallback(Collision_State State, 
	const Vector2& MousePos)
{
	if (State == Collision_State::End)
		m_MouseCollision = false;

	else
		m_MouseCollision = true;

	auto	iter = m_CollisionMouseCallback[(int)State].begin();
	auto	iterEnd = m_CollisionMouseCallback[(int)State].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(MousePos, this);
	}
}

void CCollider::Start()
{
	CPrimitiveComponent::Start();

	m_pScene->GetCollisionManager()->AddCollider(this);
	if (GetOwner()->GetName() == "Warrior")
 		int a = 10;
}

bool CCollider::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	// 기본으로 Static으로 설정해둔다.
	SetCollisionProfile("Static");

	m_CBuffer = new CColliderConstantBuffer;

	m_CBuffer->Init();

	m_DebugShader = CShaderManager::GetInst()->FindShader("ColliderShader");

	SetPivot(0.f, 0.f, 0.f);

	return true;
}

void CCollider::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);
}

void CCollider::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CCollider::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);

}

void CCollider::PrevRender(float DeltaTime)
{
	CPrimitiveComponent::PrevRender(DeltaTime);
}

void CCollider::Render(float DeltaTime)
{
	if (m_DebugRender)
	{
		CPrimitiveComponent::Render(DeltaTime);

		if (m_PrevCollisionList.empty())
			m_CBuffer->SetColor(Vector4(0.f, 1.f, 0.f, 1.f));

		else
			m_CBuffer->SetColor(Vector4(1.f, 0.f, 0.f, 1.f));

		if (m_MouseCollision)
			m_CBuffer->SetColor(Vector4(1.f, 0.f, 0.f, 1.f));

		m_CBuffer->UpdateCBuffer();

		m_DebugShader->SetShader();

		m_DebugMesh->Render();
	}
}

CCollider* CCollider::Clone()
{
	return nullptr;
}

void CCollider::ClearFrame()
{
	m_vecSectionIndex.clear();
	m_CurrentCollisionList.clear();
	m_CurrentSectionCheck = false;
	m_GroundCollisionList.clear();
	m_GroundLineCollisionList.clear();
}
