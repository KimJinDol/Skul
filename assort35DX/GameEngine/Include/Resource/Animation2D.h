#pragma once

#include "../Ref.h"
#include "AnimationSequence2D.h"

struct Animation2DNotify
{
	std::function<void()>   Function;
	class CAnimationSequence2D* pOwner;
	int				Frame;
	bool			Call;

	Animation2DNotify()
	{
		Call = false;
	}
};

struct Sequence2DInfo
{
	CSharedPtr<CAnimationSequence2D>	Sequence;
	bool				Loop;
	float				PlayRate;
	CSharedPtr<CAnimationSequence2D>	NextSequence;
	std::vector<Animation2DNotify*>	m_vecNotify;
	std::function<void()>	m_FrameEndFunction;


	Sequence2DInfo() :
		Loop(true),
		PlayRate(1.f),
		m_FrameEndFunction(nullptr)
	{
		m_vecNotify.clear();
	}

	~Sequence2DInfo()
	{
		auto	iter = m_vecNotify.begin();
		auto	iterEnd = m_vecNotify.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
	}
};


class CAnimation2D :
	public CRef
{
	friend class CSpriteComponent;
	friend class CMouseWidget;

protected:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& anim);
	virtual ~CAnimation2D();

protected:
	class CScene* m_pScene;
	class CSpriteComponent* m_Owner;
	class CWidget* m_OwnerWidget;
	class CAnimation2DConstantBuffer* m_pCBuffer;
	std::unordered_map<std::string, Sequence2DInfo*>	m_mapSequence;
	Sequence2DInfo* m_CurrentSequence;
	float		m_FrameTime;
	int			m_Frame;
	std::function<void(const std::string&)>	m_FrameEndFunction;

public:
	bool IsEndFunction() const
	{
		return m_FrameEndFunction != nullptr ? true : false;
	}

public:
	void SetAddRenderPos(const std::string& Name, const Vector2& AddPos);
	void SetAddRenderPos(const std::string& Name, float x, float y);

	void SetScene(class CScene* pScene)
	{
		m_pScene = pScene;
	}

	void SetOwner(class CSpriteComponent* Owner)
	{
		m_Owner = Owner;
	}

	void SetOwner(class CWidget* Owner)
	{
		m_OwnerWidget = Owner;
	}

	std::string GetCurrentSequenceName()	const
	{
		return m_CurrentSequence->Sequence->GetName();
	}


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CAnimation2D* Clone();
	void SetShader();
	void ResetShader();

public:
	void AddAnimationSequence2D(const std::string& Name, bool Loop = true, bool Inverse = false);
	void AddAnimationSequence2D(CAnimationSequence2D* Sequence, bool Loop = true, bool Inverse = false);
	void SetSequencePlayRate(const std::string& Name, float PlayRate);
	void AddSequencePlayRate(const std::string& Name, float PlayRate);
	void ChangeAnimation(const std::string& Name);
	void SetNextSequence2D(const std::string& SrcName, const std::string& DestName);

public:
	Sequence2DInfo* FindSequence(const std::string& Name);

public:
	template <typename T>
	void SetNotifyFunction(const std::string& Name, unsigned int Frame, T* pObj, void(T::* pFunc)())
	{
		Sequence2DInfo* pInfo = FindSequence(Name);

		if (!pInfo)
			return;

		Animation2DNotify* pNotify = new Animation2DNotify;

		pNotify->Frame = Frame;
		pNotify->pOwner = pInfo->Sequence;
		pNotify->Function = std::bind(pFunc, pObj);

		pInfo->m_vecNotify.push_back(pNotify);
	}

	template <typename T>
	void SetFrameEndFunction(T* pObj, void(T::* pFunc)())
	{
		m_FrameEndFunction = std::bind(pFunc, pObj);
	}

	template <typename T>
	void SetFrameEndFunction(const std::string& Name, T* pObj, void(T::* pFunc)())
	{
		Sequence2DInfo* pInfo = FindSequence(Name);

		if (!pInfo)
			return;

		pInfo->m_FrameEndFunction = std::bind(pFunc, pObj);
	}
};

