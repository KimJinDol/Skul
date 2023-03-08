
#include "AnimationSequence2D.h"

CAnimationSequence2D::CAnimationSequence2D()	:
	m_Type(Animation2D_Type::Atlas),
	m_Loop(true),
	m_PlayRate(1.f),
	m_PlayTime(1.f),
	m_FrameTime(1.f),
	m_pScene(nullptr),
	m_pOwner(nullptr),
	m_Inverse(false)
{
}

CAnimationSequence2D::CAnimationSequence2D(const CAnimationSequence2D& anim)
{

}

CAnimationSequence2D::~CAnimationSequence2D()
{

}

CAnimationSequence2D* CAnimationSequence2D::Clone()
{
	return new CAnimationSequence2D(*this);
}

void CAnimationSequence2D::AddRenderPos(const Vector2 AddPos)
{
	m_AddRenderPos = AddPos;
}

void CAnimationSequence2D::SetFrameSize(const Vector2& Size)
{
	m_FrameSize = Size;
}

void CAnimationSequence2D::SetTexture(CTexture* pTexture)
{
	m_Texture = pTexture;

	switch (pTexture->GetImageType())
	{
	case Image_Type::Atlas:
		m_Type = Animation2D_Type::Atlas;
		break;
	case Image_Type::Frame:
		m_Type = Animation2D_Type::Frame;
		break;
	case Image_Type::Array:
		m_Type = Animation2D_Type::Array;
		break;
	}
}

void CAnimationSequence2D::SetPlayTime(float PlayTime)
{
	m_PlayTime = PlayTime;
}

void CAnimationSequence2D::SetPlayRate(float PlayRate)
{
	m_PlayRate = PlayRate;
}

void CAnimationSequence2D::AddFrame(const Vector2& Start, const Vector2& End)
{
	Animation2DFrame	Frame;
	Frame.Start = Start;
	Frame.End = End;
	m_vecAnimFrame.push_back(Frame);

	m_FrameTime = m_PlayTime / (float)m_vecAnimFrame.size();
}

void CAnimationSequence2D::AddFrame(const Vector2& Start, const Vector2& End, int Count)
{
	m_vecAnimFrame.resize(Count);

	for (int i = 0; i < Count; ++i)
	{
		m_vecAnimFrame[i].Start = Start;
		m_vecAnimFrame[i].End = End;
	}

	m_FrameTime = m_PlayTime / (float)Count;
}