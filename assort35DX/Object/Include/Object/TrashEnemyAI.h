#pragma once
#include "AICharacter.h"
class CTrashEnemyAI :
    public CAICharacter
{
	friend class CScene;

protected:
	CTrashEnemyAI();
	CTrashEnemyAI(const CTrashEnemyAI& obj);
	virtual ~CTrashEnemyAI();

private:
	// CSharedPtr<class COgreAnimation>	m_Anim;
	//CSharedPtr<class CColliderBox2D>	m_AttackCollision;

	class CTrashEnemyWidget* m_Widget;
	class CWidgetComponent* m_WidgetComponent;

	float m_PushOutMove; // 맞을 때 밀려나는 정도

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
	virtual CTrashEnemyAI* Clone() const;

public:
	virtual void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
	virtual void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
};

