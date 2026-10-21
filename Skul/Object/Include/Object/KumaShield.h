#pragma once
#include "GameObject.h"
#include "Component\SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "interface.h"

class CKumaShield :
    public CGameObject, public IObjectView
{
	friend CScene;

protected:
	CKumaShield();
	CKumaShield(const CKumaShield& obj);
	virtual ~CKumaShield();

protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderBox2D> m_Body;

	class CKumaShieldAnimation* m_Anim;

public:
	void OnShield();
	void OffShield();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CKumaShield* Clone();

public:
	void CollisionBeginBlock(const HitResult& Result, CCollider* Collider);
	void CollisionBeginOverlap(const HitResult& Result, CCollider* Collider);
};

