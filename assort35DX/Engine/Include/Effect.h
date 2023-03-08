#pragma once
#include "GameObject.h"
#include "Component/SpriteComponent.h"
#include "Interface.h"

class CEffect :
    public CGameObject, public IObjectView
{
	friend CScene;

protected:
	CEffect();
	CEffect(const CEffect& obj);
	virtual ~CEffect();

protected:
	CSharedPtr<CGameObject> m_pOwner;			// 이펙트 생성 객체
	CSharedPtr<CSpriteComponent> m_Sprite;
	class CAnimation2D* m_Anim;

	bool		m_bUseLocalSpace;	// 이펙트가 owner를 따라갈지 정한다.
	bool		m_bIsLoop;
	Vector3		m_Offset;

public:
	CAnimation2D* GetAnim()	const
	{
		return m_Anim;
	}

public:
	void SetOpacity(float Opacity);
	void SetUseLocalSpace(bool value)
	{
		m_bUseLocalSpace = value;
	}

	void SetLoop(bool value)
	{
		m_bIsLoop = value;
	}

	void SetOwner(CGameObject* pOwner)
	{
		m_pOwner = pOwner;
	}

	void SetOffset(float x, float y)
	{
		m_Offset.x = x;
		m_Offset.y = y;
	}

	void SetOffset(const Vector3& Offset)
	{
		m_Offset = Offset;
	}

public:
	void SetAutoDestroy();
	void SetAutoDestroy(const std::string& Name);
	void EffectEnd();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(float DeltaTime);
};

