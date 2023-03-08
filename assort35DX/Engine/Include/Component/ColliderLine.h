#pragma once
#include "Collider.h"
class CColliderLine :
    public CCollider
{
	friend class CGameObject;

protected:
	CColliderLine();
	CColliderLine(const CColliderLine& com);
	virtual ~CColliderLine();

protected:
	LineInfo	m_Info;

public:
	LineInfo GetInfo()	const
	{
		return m_Info;
	}

public:
	void SetStart(const Vector2& Start)
	{
		m_Info.Start = Start;
	}

	void SetEnd(const Vector2& End)
	{
		m_Info.End = End;

		Vector2 Scale = m_Info.End - m_Info.Start;
		SetWorldScale(Scale.x, Scale.y, 1.f);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CColliderLine* Clone();

public:
	virtual bool Collision(CCollider* Dest);
	virtual bool CollisionMouse(const Vector2& MousePos);
};

