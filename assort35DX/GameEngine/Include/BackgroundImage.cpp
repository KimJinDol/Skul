#include "BackgroundImage.h"
#include "Component/SpriteComponent.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"
#include "Scene/Scene.h"
#include "Device.h"
#include "Resource/Material.h"

CBackgroundImage::CBackgroundImage() :
	m_SpriteComponent(nullptr),
	m_CameraInfluend(0.f)
{
}

CBackgroundImage::CBackgroundImage(const CBackgroundImage& obj)
{
}

CBackgroundImage::~CBackgroundImage()
{
}

void CBackgroundImage::SetCameraInfluend(float Influend)
{
	m_CameraInfluend = Influend;
}

void CBackgroundImage::SetInitUV(float x, float y)
{
	m_InitUV.x = x;
	m_InitUV.y = y;

	m_SpriteComponent->GetMaterial(0)->SetAddUV(m_InitUV);
}

bool CBackgroundImage::Init()
{
	m_SpriteComponent = CreateSceneComponent<CSpriteComponent>("Sprite");
	//SetRootComponent(m_SpriteComponent);
	m_SpriteComponent->GetMaterial(0)->SetShader("BackgroundShader");
	m_SpriteComponent->SetRender2DType(Render_Type_2D::RT2D_Background);
	
	m_RootComponent->AddChild(m_SpriteComponent);

	Resolution RS = GET_RESOLUTION;
	Vector3 Scale(RS.Width, RS.Height, 1.f);
	m_SpriteComponent->SetWorldScale(Scale);
	m_SpriteComponent->SetPivot(0.f, 0.f, 0.f);

	
	return true;
}

void CBackgroundImage::Start()
{
	CGameObject::Start();

	m_PrevCameraPos = m_pScene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();
}

void CBackgroundImage::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBackgroundImage::PrevRender(float DeltaTime)
{
	Vector2 Pos(GetWorldPos().x, GetWorldPos().y);
	CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();
	Vector2 CameraPos(Camera->GetWorldPos().x, Camera->GetWorldPos().y);

	m_SpriteComponent->SetWorldPos2D(CameraPos);

	Vector3 GapCameraPos = m_pScene->GetCameraManager()->GetCurrentCamera()->GetWorldPos() - m_PrevCameraPos;
	GapCameraPos *= m_CameraInfluend;
	GapCameraPos.y *= -1.f;
	m_SpriteComponent->GetMaterial(0)->AddUV(Vector2(GapCameraPos.x, GapCameraPos.y));

	CGameObject::PrevRender(DeltaTime);

	m_PrevCameraPos = m_pScene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();
};
