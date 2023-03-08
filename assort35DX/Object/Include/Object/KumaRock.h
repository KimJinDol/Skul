#pragma once
#include "GameObject.h"
#include "Component\ColliderCircle.h"
#include "Component\SpriteComponent.h"
#include "Interface.h"

class CKumaRock :
    public CGameObject, public IAttackable
{
	friend CScene;

protected:
	CKumaRock();
	CKumaRock(const CKumaRock& obj);
	virtual ~CKumaRock();

protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderCircle> m_Body;

private:
	float	m_Angle;
	float	m_CurTotalTime;
	float	m_InitVelocity;

	Vector3 m_InitPos;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CKumaRock* Clone();

public:
	void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
	void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
};

