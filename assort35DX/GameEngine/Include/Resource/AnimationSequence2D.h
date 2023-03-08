#pragma once

#include "../Ref.h"
#include "Texture.h"

struct Animation2DFrame
{
	Vector2	Start;
	Vector2	End;
};

class CAnimationSequence2D :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;
	friend class CAnimation2D;

private:
	CAnimationSequence2D();
	CAnimationSequence2D(const CAnimationSequence2D& anim);
	~CAnimationSequence2D();

public:
	CAnimationSequence2D* Clone();

private:
	class CScene* m_pScene;
	class CGameObject*		m_pOwner;
	CSharedPtr<CTexture>    m_Texture;
	Animation2D_Type        m_Type;
	bool					m_Loop;
	float                   m_PlayRate;
	float                   m_PlayTime;
	float					m_FrameTime;
	Vector2					m_AddRenderPos;
	std::vector<Animation2DFrame>	m_vecAnimFrame;
	Vector2					m_FrameSize;

	bool					m_Inverse;


public:
	void AddRenderPos(const Vector2 AddPos);
	void SetFrameSize(const Vector2& Size);
	void SetInverse(bool Inverse)
	{
		m_Inverse = Inverse;
	}

public:
	bool GetInverse()	const
	{
		return m_Inverse;
	}

	Vector2 GetFrameSize() const
	{
		return m_FrameSize;
	}

	CTexture* GetTexture() const
	{
		return m_Texture.Get();
	}

	Animation2D_Type GetAnimation2DType()	const
	{
		return m_Type;
	}

	const Animation2DFrame& GetAnimation2DFrame(int Index = 0)	const
	{
		return m_vecAnimFrame[Index];
	}

	Animation2DFrame GetAnimation2DFrameUV(int Index)	const
	{
		Animation2DFrame	Frame = {};

		unsigned int Width = m_Texture->GetWidth();
		unsigned int Height = m_Texture->GetHeight();

		Frame.Start = m_vecAnimFrame[Index].Start / Vector2((float)Width, (float)Height);
		Frame.End = m_vecAnimFrame[Index].End / Vector2((float)Width, (float)Height);

		return Frame;
	}

public:
	void SetTexture(CTexture* pTexture);
	void SetPlayTime(float PlayTime);
	void SetPlayRate(float PlayRate);
	void AddFrame(const Vector2& Start, const Vector2& End);
	void AddFrame(const Vector2& Start, const Vector2& End, int Count);


};

