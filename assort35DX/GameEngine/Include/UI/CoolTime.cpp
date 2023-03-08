#include "CoolTime.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Resource/UITransformConstantBuffer.h"
#include "../Resource/CoolCBuffer.h"


CCoolTime::CCoolTime() :
	m_Ratio(0.f),
	m_Dir(1),
	m_Circle(0)
{
}

CCoolTime::CCoolTime(const CCoolTime& widget) :
	CWidget(widget)
{
	m_Texture = widget.m_Texture;
}

CCoolTime::~CCoolTime()
{
	delete m_CoolTimeCBuffer;
}

void CCoolTime::SetStartEndUV(unsigned int x, unsigned int y, const Vector2& FrameSize)
{
	Vector2 TextureSize(m_Texture->GetWidth(), m_Texture->GetHeight());

	Vector2 StartPos(x * FrameSize.x, y * FrameSize.y);
	Vector2 EndPos = StartPos + FrameSize;

	SetStartUV(StartPos / TextureSize);
	SetEndUV(EndPos / TextureSize);
}

void CCoolTime::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return;

	m_Texture = m_Scene->GetResource()->FindTexture(Name);

	if (m_Texture)
		m_TransformCBuffer->SetTextureEnable(true);

	else
		m_TransformCBuffer->SetTextureEnable(false);
}

bool CCoolTime::Init()
{
	CWidget::Init();

	m_Shader = CShaderManager::GetInst()->FindShader("CoolTimeShader");

	m_CoolTimeCBuffer = new CCoolTimeCBuffer;

	m_CoolTimeCBuffer->Init();

	return true;
}

void CCoolTime::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CCoolTime::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);
}

void CCoolTime::Render()
{
	CWidget::Render();

	m_CoolTimeCBuffer->SetCircle(m_Circle);
	m_CoolTimeCBuffer->SetRatio(m_Ratio);
	m_CoolTimeCBuffer->SetDir(m_Dir);
	m_CoolTimeCBuffer->UpdateCBuffer();

	if (m_Texture)
		m_Texture->SetShader(0, TST_PIXEL);

	m_Mesh->Render();

	if (m_Texture)
		m_Texture->ResetShader(0, TST_PIXEL);
}

CCoolTime* CCoolTime::Clone()
{
	return new CCoolTime(*this);
}
