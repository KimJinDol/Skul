#pragma once
#include "AICharacter.h"

class COgreAI :
    public CAICharacter
{
	friend class CScene;

protected:
	COgreAI();
	COgreAI(const COgreAI& obj);
	virtual ~COgreAI();

private:
	CSharedPtr<class COgreAnimation>	m_Anim;
	CSharedPtr<class CColliderBox2D>	m_AttackCollision;
	
	class CBossWidget* m_Widget;

public:
	class CBossWidget* GetWidget()
	{
		return m_Widget;
	}

private:
	void CheckTargetInAttackRange();

public:
	virtual void Attack();
	void AttackCheck();
	void OffAttackCollision();
	virtual void AttackEnd();

	virtual void StanbyEnd();

	virtual void Die();


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual COgreAI* Clone() const;

public:
	virtual void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
	virtual void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
};

