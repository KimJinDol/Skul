#pragma once
#include "GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Interface.h"

class CSkulDefaultHead :
    public CGameObject, public IAttackable
{
	friend CScene;

protected:
	CSkulDefaultHead();
	CSkulDefaultHead(const CSkulDefaultHead& obj);
	virtual ~CSkulDefaultHead();

private:
	float	m_Velocity;
	int		m_Dir;
	bool	m_bIsBlock; // ¿ÀºêÁ§Æ®¿¡ ºÎµúÈ÷¸é Æ¢±â°Ô ÇØ¾ß´ï
	float	m_MoveVel;

public:
	void SetDirection(int Dir);

public:
	bool IsBlock()	const
	{
		return m_bIsBlock;
	}

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
	virtual CSkulDefaultHead* Clone();

public:
	void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
	void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
};

