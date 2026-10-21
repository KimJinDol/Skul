#pragma once
#include "GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Interface.h"
#include "Flag.h"

class CCharacter :
    public CGameObject, public IObjectView
{
    friend CScene;

protected:
    CCharacter();
    CCharacter(const CCharacter& obj);
    virtual ~CCharacter();

protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderBox2D> m_Body;
	class CAnimation2D* m_Animation2D;

	Vector2 m_BodyExtent;


public:
	void SetJumpMaxCount(int JumpCount);
	void SetJumpVelocity(float Velocity);
	bool IsJump() const;
	void Jump();
	int GetCurrentJumpCount()	const;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CCharacter* Clone();

};

