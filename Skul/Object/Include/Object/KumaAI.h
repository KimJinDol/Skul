#pragma once
#include "AICharacter.h"
#include "../Animation2D/KumaAnimation.h"

class CKumaAI :
    public CAICharacter
{
	friend class CScene;

protected:
	CKumaAI();
	CKumaAI(const CKumaAI& obj);
	virtual ~CKumaAI();

private:
	CSharedPtr<class CKumaAnimation>	m_Anim;
	CSharedPtr<class CColliderBox2D>	m_AttackCollision;
	CSharedPtr<class CColliderCircle>	m_UltimateCollision;

	class CBossWidget* m_Widget;

	CSharedPtr<CSpriteComponent> m_ShieldSprite;
	CSharedPtr<CColliderBox2D> m_ShieldBody;

	class CKumaShieldAnimation* m_ShieldAnim;

	bool	m_bIsUltimate;
	float	m_UltimateAD;

	bool	m_bIsEarthHit;
	float	m_EarthHitFadeOutBackgroundTime;
	float	m_CurEarthHitFadeOutBackgroundTime;
	Vector4	m_EarthHitFadeOutBackgroundColor;
	float	m_EarthHitCheckRange;
	float	m_EarthHitAD;

	float	m_StampingMoveX;
	float	m_StampingMoveSpeed;

	float	m_JumpStampingMoveSpeed;

	bool	m_bIsShieldDefense;
	bool	m_bCanShieldDash;
	bool	m_bIsShieldDash;
	float	m_ShieldDashMoveDistance;
	float	m_ShieldDashTargetPos;
	float	m_ShieldDashSpeed;
	float	m_ShieldDashRetryDistance;
	int		m_CurShieldDashCount;
	int		m_ShieldDashMaxCount;
	float	m_ShieldDashTotalMove;

	bool	m_bIsIntroJumpStampingCheck;

	bool	m_bFadeOutBackground;
	bool	m_bFadeInBackground;

	
public:
	CKumaAnimation* GetAnim()	const
	{
		return m_Anim;
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

	void Ultimate();
	void UltimateStart();
	void UltimateAttackCheck();
	bool IsUltimate() const
	{
		return m_bIsUltimate;
	}
	void UltimateEnd();

	void EarthHit();
	void EarthHitStart();
	void EarthHitJump();
	void EarthHitAttackCheck();
	bool IsEarthHit() const
	{
		return m_bIsEarthHit;
	}
	void EarthHitEnd();

	void Stamping();
	void StampingStart();
	void StampingAttackCheck();
	void StampingCollisionOff();
	void StampingEnd();

	void JumpStamping();
	void JumpStampingStart();
	void JumpStampingAttackCheck();
	void JumpStampingEnd();
	
	void Shield(bool Dash);
	void ShieldStart();
	void ShieldOff();
	void ShieldEnd();
	
	void ShieldDash();
	void ShieldDashStart();

	void Intro();
	bool IsIntroJumpStampingEnd() const
	{
		return m_bIsIntroJumpStampingCheck;
	}
	void IntroJumpStamping();
	void IntroEnd();

	void BehaviorEnd();

public:
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CKumaAI* Clone() const;

public:
	virtual void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
	virtual void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
};

