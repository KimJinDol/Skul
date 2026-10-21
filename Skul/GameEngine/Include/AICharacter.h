#pragma once
#include "Character.h"
#include "AIController.h"
#include "Interface.h"

class CAICharacter :
    public CCharacter, public IHitable
{
    friend class CScene;
	friend class CAIManager;

protected:
	CAICharacter();
	CAICharacter(const CAICharacter& obj);
	virtual ~CAICharacter();

protected:
	class CAIController* m_pAIController;
	class CHPComponent* m_HPComponent;

	float		m_TraceDistance;
	float		m_StanbyTime;
	float		m_CurStanbyTime;
	bool		m_bIsStanby;
	float		m_AttackDistance;
	bool		m_bIsAttack;

	float		m_AD;

	Object_View m_PostView;

public:
	void SetEnableController(bool Enable);

public:
	virtual void Attack() = 0;
	virtual void AttackEnd() = 0;
	virtual void StanbyEnd() = 0;
	virtual void Die();

public:
	class CHPComponent* GetHPComponent()	const
	{
		return m_HPComponent;
	}

public:
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);
	void DamageEmissiveOff();

public:
	void SetAIIntData(const std::string& Name, int Data);
	void SetAIFloatData(const std::string& Name, float Data);
	void SetAIBoolData(const std::string& Name, bool Data);
	void SetAIStringData(const std::string& Name, std::string Data);
	void SetAIGameObjectData(const std::string& Name, class CGameObject* Data);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CAICharacter* Clone() const = 0;

public:
	virtual void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
	virtual void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);

public:
	template <typename T>
	T* CreateAIController()
	{
		T* pController = new T;

		pController->SetScene(m_pScene);
		pController->SetOwner(this);

		m_pAIController = (CAIController*)pController;

		return pController;
	}

	template <typename T>
	T* CreateAI(const std::string& Name)
	{
		if (!m_pAIController)
			return nullptr;

		return m_pAIController->CreateAI<T>(Name);
	}

	template <typename T>
	T* CreateAISelect(const std::string& Name)
	{
		if (!m_pAIController)
			return nullptr;

		return m_pAIController->CreateAISelect<T>(Name);
	}
};

