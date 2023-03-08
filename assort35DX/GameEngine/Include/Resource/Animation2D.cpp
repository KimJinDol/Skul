
#include "Animation2D.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "AnimationSequence2D.h"
#include "ResourceManager.h"
#include "../Component/SpriteComponent.h"
#include "Animation2DConstantBuffer.h"
#include "../UI/Widget.h"
#include "../Character.h"

CAnimation2D::CAnimation2D()	:
	m_pScene(nullptr),
	m_Owner(nullptr),
	m_OwnerWidget(nullptr),
	m_FrameTime(0.f),
	m_Frame(0),
	m_CurrentSequence(nullptr)
{
}

CAnimation2D::CAnimation2D(const CAnimation2D& anim)
{
	*this = anim;

	m_RefCount = 1;

	m_pCBuffer = anim.m_pCBuffer->Clone();

	m_pScene = nullptr;
	m_Owner = nullptr;
	m_OwnerWidget = nullptr;

	m_FrameTime = 0.f;
	m_Frame = 0;

	m_mapSequence.clear();

	auto	iter = anim.m_mapSequence.begin();
	auto	iterEnd = anim.m_mapSequence.end();

	for (; iter != iterEnd; ++iter)
	{
		Sequence2DInfo* pInfo = new Sequence2DInfo;

		pInfo->Sequence = iter->second->Sequence;
		pInfo->Loop = iter->second->Loop;

		m_mapSequence.insert(std::make_pair(iter->first, pInfo));

		if (anim.m_CurrentSequence == iter->second)
			m_CurrentSequence = pInfo;
	}
}

CAnimation2D::~CAnimation2D()
{
	auto	iter = m_mapSequence.begin();
	auto	iterEnd = m_mapSequence.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	SAFE_DELETE(m_pCBuffer);
}

void CAnimation2D::SetAddRenderPos(const std::string& Name, const Vector2& AddPos)
{
	Sequence2DInfo* pInfo = FindSequence(Name);

	if (!pInfo)
		return;

	pInfo->Sequence->AddRenderPos(AddPos);
}

void CAnimation2D::SetAddRenderPos(const std::string& Name, float x, float y)
{
	Sequence2DInfo* pInfo = FindSequence(Name);

	if (!pInfo)
		return;

	Vector2 AddPos(x, y);

	pInfo->Sequence->AddRenderPos(AddPos);
}

void CAnimation2D::Start()
{
	m_Owner->SetWorldScale(m_CurrentSequence->Sequence->GetFrameSize().x, m_CurrentSequence->Sequence->GetFrameSize().y, 1.f);
}

bool CAnimation2D::Init()
{
	m_pCBuffer = new CAnimation2DConstantBuffer;

	m_pCBuffer->Init();

	return true;
}

void CAnimation2D::Update(float DeltaTime)
{
	if (m_mapSequence.empty())
		return;

	m_FrameTime += DeltaTime;

	float	FrameTime = m_CurrentSequence->Sequence->m_FrameTime /
		m_CurrentSequence->PlayRate;

	
	if (m_FrameTime >= FrameTime)
	{
		m_FrameTime -= FrameTime;

		int EndFrame = 0;
		bool Inverse = m_CurrentSequence->Sequence->GetInverse();
		if (!Inverse)
		{
			++m_Frame;
			EndFrame = (int)m_CurrentSequence->Sequence->m_vecAnimFrame.size();
		}
		else
		{
			--m_Frame;
			EndFrame = -1;
		}

		// 끝 프레임일 때 실행
		if (m_Frame == EndFrame)
		{
			auto	iter = m_CurrentSequence->m_vecNotify.begin();
			auto	iterEnd = m_CurrentSequence->m_vecNotify.end();

			for (; iter != iterEnd; ++iter)
			{
				(*iter)->Call = false;
			}

			if (m_CurrentSequence->Loop)
			{
				if (!m_CurrentSequence->Sequence->GetInverse())
					m_Frame = 0;
				else
					m_Frame = (int)m_CurrentSequence->Sequence->m_vecAnimFrame.size() - 1;
			}
			else
			{
				if (!m_CurrentSequence->Sequence->GetInverse())
					m_Frame = (int)m_CurrentSequence->Sequence->m_vecAnimFrame.size() - 1;
				else
					m_Frame = 0;
			}


			if (m_CurrentSequence->m_FrameEndFunction)
				m_CurrentSequence->m_FrameEndFunction();

			if (m_CurrentSequence->NextSequence)
				ChangeAnimation(m_CurrentSequence->NextSequence->GetName());
		}
	}

	auto	iter = m_CurrentSequence->m_vecNotify.begin();
	auto	iterEnd = m_CurrentSequence->m_vecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Frame == m_Frame && !(*iter)->Call)
		{
			(*iter)->Call = true;

			(*iter)->Function();
			//if (m_Owner)
			//	m_Owner->CallNotify((*iter)->Name);

			//else
			//	m_OwnerWidget->CallAnimNotify((*iter)->Name);
		}
	}
}

void CAnimation2D::PostUpdate(float DeltaTime)
{
}


CAnimation2D* CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}

void CAnimation2D::SetShader()
{
	Vector2	StartUV(0.f, 0.f), EndUV(1.f, 1.f);

	Vector2	TextureSize = Vector2((float)m_CurrentSequence->Sequence->m_Texture->GetWidth(),
		(float)m_CurrentSequence->Sequence->m_Texture->GetHeight());

	const Animation2DFrame&	Frame = m_CurrentSequence->Sequence->m_vecAnimFrame[m_Frame];
	
	switch (m_CurrentSequence->Sequence->m_Type)
	{
	case Animation2D_Type::Atlas:
		m_CurrentSequence->Sequence->m_Texture->SetShader(0, TST_PIXEL);
        // 현재 애니메이션 타입이 아틀라스라면, 시작 UV 지점, 끝 UV 지점을 계산해주어야 한다. 
		StartUV = (Frame.Start + m_CurrentSequence->Sequence->m_AddRenderPos) / TextureSize;
		EndUV = (Frame.End + m_CurrentSequence->Sequence->m_AddRenderPos) / TextureSize;
		break;
	case Animation2D_Type::Frame:
		m_CurrentSequence->Sequence->m_Texture->SetShader(0, TST_PIXEL, m_Frame);
		break;
	case Animation2D_Type::Array:
		m_CurrentSequence->Sequence->m_Texture->SetShader(0, TST_PIXEL);
		break;
	}

	if (m_Owner)
	{
		CCharacter* Character = (CCharacter*)m_Owner->GetOwner();
		if (Character && Character->GetView() == Object_View::Left)
		{
			float TempUV_X = StartUV.x;
			StartUV.x = EndUV.x;
			EndUV.x = TempUV_X;
		}
	}

	m_pCBuffer->SetStartUV(StartUV);
	m_pCBuffer->SetEndUV(EndUV);
	m_pCBuffer->SetAnimation2DType(m_CurrentSequence->Sequence->m_Type);

	m_pCBuffer->UpdateCBuffer();
}

void CAnimation2D::ResetShader()
{
}

void CAnimation2D::AddAnimationSequence2D(const std::string& Name, bool Loop, bool Inverse)
{
	CAnimationSequence2D* pSequence = nullptr;

	if (m_pScene)
	{
		pSequence = m_pScene->GetResource()->FindAnimationSequence2D(Name);

		if (!pSequence)
			pSequence = CResourceManager::GetInst()->FindAnimationSequence2D(Name);
	}

	else
	{
		pSequence = CResourceManager::GetInst()->FindAnimationSequence2D(Name);
	}

	pSequence->SetName(Name);
	pSequence->SetInverse(Inverse);

	Sequence2DInfo* pInfo = new Sequence2DInfo;

	pInfo->Sequence = pSequence;
	pInfo->Loop = Loop;

	m_mapSequence.insert(std::make_pair(Name, pInfo));

	if (!m_CurrentSequence)
	{
		m_CurrentSequence = pInfo;
		if (Inverse)
			m_Frame = pSequence->m_vecAnimFrame.size() - 1;
	}
}

void CAnimation2D::AddAnimationSequence2D(CAnimationSequence2D* Sequence, bool Loop, bool Inverse)
{
	Sequence2DInfo* pInfo = new Sequence2DInfo;

	pInfo->Sequence = Sequence;
	pInfo->Loop = Loop;

	m_mapSequence.insert(std::make_pair(Sequence->m_Name, pInfo));

	if (!m_CurrentSequence)
	{
		m_CurrentSequence = pInfo;
		if (Inverse)
			m_Frame = Sequence->m_vecAnimFrame.size() - 1;
	}
}

void CAnimation2D::SetSequencePlayRate(const std::string& Name, float PlayRate)
{
	Sequence2DInfo* pInfo = FindSequence(Name);

	if (!pInfo)
		return;

	pInfo->PlayRate = PlayRate;
}

void CAnimation2D::AddSequencePlayRate(const std::string& Name, float PlayRate)
{
	Sequence2DInfo* pInfo = FindSequence(Name);

	if (!pInfo)
		return;

	pInfo->PlayRate += PlayRate;
}

void CAnimation2D::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentSequence->Sequence->GetName() == Name)
		return;

	Sequence2DInfo* pInfo = FindSequence(Name);

	if (!pInfo)
		return;


	auto	iter = m_CurrentSequence->m_vecNotify.begin();
	auto	iterEnd = m_CurrentSequence->m_vecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Call = false;
	}

	m_FrameTime = 0.f;

	if (!pInfo->Sequence->GetInverse())
		m_Frame = 0;
	else
		m_Frame = pInfo->Sequence->m_vecAnimFrame.size() - 1;

	m_CurrentSequence = pInfo;
	m_Owner->SetWorldScale(m_CurrentSequence->Sequence->GetFrameSize().x, m_CurrentSequence->Sequence->GetFrameSize().y, 1.f);
}

void CAnimation2D::SetNextSequence2D(const std::string& SrcName, const std::string& DestName)
{
	Sequence2DInfo* pSrcInfo = FindSequence(SrcName);
	Sequence2DInfo* pDestSequence = FindSequence(DestName);

	if (!pSrcInfo || !pDestSequence)
		return;

	pSrcInfo->NextSequence = pDestSequence->Sequence;
}

Sequence2DInfo* CAnimation2D::FindSequence(const std::string& Name)
{
	auto	iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return nullptr;

	return iter->second;
}
