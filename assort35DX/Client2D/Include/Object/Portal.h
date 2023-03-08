#pragma once
#include "GameObject.h"
#include "Component/SpriteComponent.h"

class CPortal :
	public CGameObject
{
	friend class CScene;

protected:
	CPortal();
	CPortal(const CPortal& obj);
	virtual ~CPortal();

private:
	CSharedPtr<CSpriteComponent> m_Sprite;
	class CAnimation2D* m_Anim;
	bool		m_bCanInteraction;
	bool		m_bIsActive;



public:
	void SetActivate();
	bool GetEnableInteraction()	const
	{
		return m_bCanInteraction;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CPortal* Clone()	const;

public:
	void BeginCollision(const HitResult& Result, CCollider* Collider);
	void EndCollision(const HitResult& Result, CCollider* Collider);
};
