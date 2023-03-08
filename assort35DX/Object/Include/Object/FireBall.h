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
	bool	m_bIsBlock; // ¿ÀºêÁ§Æ®¿¡ ºÎµúÈ÷¸é Æ¢±â°Ô ÇØ¾ß´ï
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

