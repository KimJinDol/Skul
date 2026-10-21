#pragma once
#include "GameObject.h"
#include "Interface.h"
#include "Component/ColliderCircle.h"
#include "Component/SpriteComponent.h"

class CFireBall :
    public CGameObject, public IObjectView, public IAttackable
{
	friend CScene;

protected:
	CFireBall();
	virtual ~CFireBall();

private:
	float	m_Velocity;
	bool	m_bIsBlock; // 오브젝트에 부딪히면 튀기게 해야댐
	float	m_MoveXPos;
	float	m_MoveXMaxPos;


protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderCircle> m_Body;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);

public:
	void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
	void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
};

