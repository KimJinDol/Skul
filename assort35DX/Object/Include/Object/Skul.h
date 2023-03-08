#pragma once
#include "Character.h"
#include "Component/Camera.h"
#include "Component/SpringArm2D.h"

enum class ESkulState
{
	None,
	Default,
	NoHead,
	ChiefGuard
};

class CSkul :
    public CCharacter, public IHitable, public IAttackable
{
    friend class CScene;
protected:
    CSkul();
    ~CSkul();

private:
	// basic movement
	bool	m_bCanMove;
	bool	m_bCanJump;
	bool	m_bCanAttack;
	bool	m_bCanSkill1;
	bool	m_bCanSkill2;
	bool	m_bCanSwitch;

	bool	m_bCanDownJump;

	CSharedPtr<CSpringArm2D> m_Arm;
	CSharedPtr<CCamera> m_Camera;
	CSharedPtr<class CSkulAnimation>	m_Anim;
	CSharedPtr<CColliderBox2D>	m_AttackCollision;

	class CSkulWidget* m_Widget;
	class CHPComponent* m_HPComponent;

	class CSkulDefaultHead* m_SkulHead;

	ESkulState	m_SkulState;
	ESkulState	m_SubSkulState;

	bool	m_bIsDownJump;
	bool	m_bIsWeapon;

	bool	m_bIsDash;
	float	m_DashTime;
	float	m_CurDashTime;
	float	m_DashVelocity;
	unsigned int	m_DashMaxCount;
	unsigned int	m_CurDashCount;

	// Switch
	bool	m_bIsSwitch;
	float	m_SwitchDefault_MoveSpeed;
	bool	m_bIsSwitchCool;
	bool	m_bIsSwitchCool_ChiefGuard;
	float	m_SwitchChiefGuard_MoveSpeed;
	int		m_SwitchChiefGuard_MoveDir;

	// Attack
	Vector2	m_AttackCollisionExtent_Default;
	Vector2 m_AttackCollisionExtent_ChiefGuard;

	bool	m_bIsAttack;
	int		m_CurCombo;				// current combo count
	int		m_ComboMax;				// max combo count
	bool	m_bNextComboInput;

	bool	m_bIsJumpAttack;

	// Skill
	bool	m_bIsSkill1;
	bool	m_bIsSkill1Cool;
	bool	m_bIsSkill1Cool_ChiefGuard;
	float	m_Skill1ChiefGuard_MoveSpeed;
	bool	m_bIsIlseom;

	bool	m_bIsSkill2;
	bool	m_bIsSkill2Cool;
	bool	m_bIsSkill2Cool_ChiefGuard;
	bool	m_bIsInteraction;

	Object_View m_AttackView;

public:
	class CSkulWidget* GetWidget()	const
	{
		return m_Widget;
	}
	ESkulState GetSkulState()	const;
	ESkulState GetSkulSubState()	const;
	bool IsAttack() const;
	bool GetInteraction() const
	{
		return m_bIsInteraction;
	}

public:
	void SetEnable(bool bMove = true, bool bJump = true, bool bAttack = true)
	{
		m_bCanMove = bMove;
		m_bCanJump = bJump;
		m_bCanAttack = bAttack;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);


public:
	void SetState(ESkulState State);
	void SetSubState(ESkulState State);
	void Switch();

private:
	void Interaction(float DeltaTime);

	void SetSkulHead(CSkulDefaultHead* SkulHead);

	void SwitchInput(float DeltaTime);
	void SwitchEnd();
	void SwitchChiefGuardMoveDirInverse();

	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void DownKeyInput(float DeltaTime);
	void DownKeyUp(float DeltaTime);
	void InitMoveDirX(float DeltaTime);

	void Jump(float DeltaTime);

	void Dash(float DeltaTime);
	void DashEnd();

	void AttackInput(float DeltaTime);
	void AttackComboCheck();
	void Attack();
	void AttackEnd();

	void JumpAttackInput(float DeltaTime);

	void Skill1Input(float DeltaTime);
	void ShotHead();
	void Ilseom();
    void Skill1End();

	void Skill2Input(float DeltaTime);
	void TeleportToHead();
	void FireBall();
    void Skill2End();

	void EquipHead();

	void EndCoolSkill1();
	void EndCoolSwitch();
    void EndCoolSkill2();
	void EndCoolSkill1_ChiefGuard();
	void EndCoolSkill2_ChiefGuard();

	void EndAllBehavior();

public:
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);

public:
	void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
	void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);

};

