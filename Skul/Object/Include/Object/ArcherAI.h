#pragma once
#include "TrashEnemyAI.h"

class CArcherAI :
    public CTrashEnemyAI
{
	friend class CScene;

protected:
	CArcherAI();
	CArcherAI(const CArcherAI& obj);
	virtual ~CArcherAI();

private:
	CSharedPtr<class CArcherAnimation>	m_Anim;

	class CTrashEnemyWidget* m_Widget;

private:
	void CheckTargetInAttackRange();

public:
	virtual void Attack();
	void SpawnArrow();
	virtual void AttackEnd();

	virtual void StanbyEnd();

	virtual void StunEnd();

	virtual void Die();


public:
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CArcherAI* Clone() const;

public:
	virtual void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
	virtual void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
};

