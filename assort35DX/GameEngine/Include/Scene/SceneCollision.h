#pragma once

#include "../GameEngine.h"
#include "../Component/Collider.h"
#include "../UI/Widget.h"

//struct RaycastInfo
//{
//	class CGameObject* Owner;
//	class CColliderLine* Line; // 레이캐스트를 실행할 Line 콜라이더
//
//	RaycastInfo(HitResult& _hitResult) :
//		Line(nullptr),
//		Owner(nullptr)
//	{
//	}
//	~RaycastInfo()
//	{
//		SAFE_DELETE(Line);
//	}
//};

struct CollisionSectionInfo
{
	std::vector<class CCollisionSection*>	vecSection;
	Vector3	SectionSize;
	Vector3	Center;
	Vector3	SectionTotalSize;
	Vector3	Min;
	Vector3	Max;
	int		CountX;
	int		CountY;
	int		CountZ;

	CollisionSectionInfo() :
		CountX(1),
		CountY(1),
		CountZ(1)
	{
	}
};

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	class CScene* m_pScene;
	std::list<CSharedPtr<CCollider>>	m_ColliderList;
	CollisionSectionInfo* m_Section2D;
	CollisionSectionInfo* m_Section3D;
	Vector3		m_Center;			// 씬 콜리전의 기준점
	Vector3		m_SectionTotalSize;
	Vector3		m_SectionSize;
	bool		m_Move;				// m_Center 값에 변동이 있으면 true
	std::vector<CSharedPtr<CWidget>>	m_vecWidget;
	CSharedPtr<CCollider>	m_MouseCollisionCollider;
	CSharedPtr<CWidget>		m_MouseCollisionWidget;


public:
	void AddCollider(class CCollider* Collider);
	void AddWidget(CWidget* Widget);
	void SetCenter(const Vector3& Center)
	{
		if (m_Center != Center)
		{
			m_Move = true;
			m_Center = Center;
		}

		if (m_Section2D)
			m_Section2D->Center = Center;

		else if (m_Section3D)
			m_Section3D->Center = Center;
	}

private:
	void CheckCollisionSection();
	void CreateSection2D();
	void CreateSection3D();
	void CalculateSectionInfo();

public:
	void Start();
	bool Init();
	void Collision(float DeltaTime);

private:
	void CollisionMouse(float DeltaTime);

	// UI 및 World 물체 정렬함수.
private:
	static int SortZOrder(const void* Src, const void* Dest);
};

