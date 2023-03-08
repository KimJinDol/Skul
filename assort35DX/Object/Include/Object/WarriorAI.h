#pragma once
#include "TrashEnemyAI.h"

class CWarriorAI :
    public CTrashEnemyAI
{
	friend class CScene;

protected:
	CWarriorAI();
	CWarriorAI(const CWarriorAI& obj);
	virtual ~CWarriorAI();

private:
	CSharedPtr<class CWarriorAnimation>	m_Anim;
	CSharedPtr<class CColliderBox2D>	m_AttackCollision;

	class CTrashEnemyWidget* m_Widget;

private:
	void CheckTargetInAttackRange();

public:
	virtual void Attack();
	void AttackCheck();
	void OffAttackCollision();
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
	virtual CWarriorAI* Clone() const;

public:
	virtual void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
	virtual void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
};

