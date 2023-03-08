
#include "Camera.h"
#include "../Render/RenderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Device.h"
#include "../BasicMath.h"

CCamera::CCamera()	:
	m_curFreqTimeX(0.f),
	m_curFreqTimeY(0.f),
	m_AmplitudeX(4.f),
	m_AmplitudeY(4.f),
	m_bIsShake(false)
{
	m_SceneComponentType = SceneComponent_Type::Camera;
	m_CameraType = Camera_Type::Cam2D;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
	m_CameraLeft = 0.f;
	m_CameraBottom = 0.f;
	Resolution RS = CDevice::GetInst()->GetResolution();
	m_WorldSize = Vector2(100000.f, 100000.f);
	m_bCameraFocus = false;
	m_bEndCameraFocus = false;
	m_FocusLerpTime = 0.f;
	m_FocusTime = 0.f;
	m_CurFocusTime = 0.f;
	m_CurFocusLerpTime = 0.f;
}

CCamera::CCamera(const CCamera& com) :
	CSceneComponent(com)
{
	m_CameraType = com.m_CameraType;
	m_ViewAngle = com.m_ViewAngle;
	m_Distance = com.m_Distance;
	m_matView = com.m_matView;
	m_matProj = com.m_matProj;
	m_CameraLeft = com.m_CameraLeft;
	m_CameraBottom = com.m_CameraBottom;
}

CCamera::~CCamera()
{
}

void CCamera::CameraShake(float AmplitudeX, float FrequencyX, float AmplitudeY, float FrequencyY, float Time)
{
	m_AmplitudeX = AmplitudeX;
	m_AmplitudeY = AmplitudeY;

	m_FrequencyX = FrequencyX;
	m_FrequencyY = FrequencyY;

	m_ShakeTime = Time;

	m_bIsShake = true;
}

void CCamera::CameraShake(float DeltaTime)
{
	if (m_bIsShake)
	{
		m_ShakeTime -= DeltaTime;

		if (m_ShakeTime <= 0.f)
			m_bIsShake = false;

		if (m_AmplitudeX)
		{
			m_curFreqTimeX -= DeltaTime;

			if (m_curFreqTimeX <= 0.f)
			{
				m_curFreqTimeX = m_FrequencyX;
				int Dir = rand() % 2 == 1 ? 1 : -1;
				m_ShakePos.x = rand() % m_AmplitudeX * Dir;
			}
		}

		if (m_AmplitudeY)
		{
			m_curFreqTimeY -= DeltaTime;

			if (m_curFreqTimeY <= 0.f)
			{
				m_curFreqTimeY = m_FrequencyY;
				int Dir = rand() % 2 == 1 ? 1 : -1;
				m_ShakePos.y = rand() % m_AmplitudeY * Dir;
			}
		}
	}
	else
		m_ShakePos = Vector2(0.f, 0.f);
}

void CCamera::StartCameraFocus(const Vector2& FocusPos, float LerpTime, float FocusTime)
{
	m_FocusPos = FocusPos;
	m_FocusLerpTime = LerpTime;
	m_FocusTime = FocusTime;
	m_bCameraFocus = true;
	m_bEndCameraFocus = false;
	m_FocusInitPos = GetWorldPos2D();
}

void CCamera::CameraFocus(float DeltaTime)
{
	Vector2 Lerp;
	float Alpha = 0.f;
	m_CurFocusLerpTime += DeltaTime;
	Alpha = BasicMath::Clamp<float>(m_CurFocusLerpTime / m_FocusLerpTime, 0.f, 1.f);
	if (m_bEndCameraFocus)
	{
		Lerp = BasicMath::VLerp(m_FocusPos, m_pParent->GetWorldPos2D(), Alpha);
		if (Alpha >= 1.f)
		{
			m_bEndCameraFocus = false;
			m_bCameraFocus = false;
			m_FocusEndFunction();
		}
	}
	else
	{
		Lerp = BasicMath::VLerp(m_FocusInitPos, m_FocusPos, Alpha);
		if (Alpha >= 1.f && m_FocusTime > 0.f)
		{
			m_CurFocusTime += DeltaTime;

			if (m_CurFocusTime >= m_FocusTime)
				EndCameraFocus();
		}

	}
	SetWorldPos2D(Lerp);
}

void CCamera::EndCameraFocus()
{
	m_bEndCameraFocus = true;
	m_CurFocusTime = 0.f;
	m_CurFocusLerpTime = 0.f;
	//m_FocusInitPos = Vector2(0.f, 0.f);
}

void CCamera::Set2DWorldSize(const Vector2& Size)
{
	m_WorldSize = Size;
}

void CCamera::CreateProjectionMatrix()
{
	Resolution  RS = CDevice::GetInst()->GetResolution();

	switch (m_CameraType)
	{
	case Camera_Type::Cam3D:
	{
        // 도 단위로 측정된 각도를 라디안 단위로 측정된 각도로 변환.
        float	Angle = XMConvertToRadians(m_ViewAngle);
        // 시야를 기반으로 왼손 투시 투영 행렬을 만듭니다.
        // 1 인자 : 라디안 단위의 하향식 시야각
        // 2 인자 : X : Y의 종횡비
        // 3 인자 : 가까운 클리핑 평면까지의 거리. 0보다 커야함
        // 4 인자 : 먼 클리핑 평면까지의 거리. 0보다 커야함
        m_matProj = XMMatrixPerspectiveFovLH(Angle,
            RS.Width / (float)RS.Height, 0.1f, m_Distance);
	}
		break;
	case Camera_Type::Cam2D:
	case Camera_Type::CamUI:
    {
        // 왼손 좌표계에 대한 사용자 지정 직교 투영 행렬을 만듭니다.
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)RS.Width, 0.f, (float)RS.Height,
			0.f, m_Distance);
	}
		break;
	}
}

void CCamera::Start()
{
	CSceneComponent::Start();

	CreateProjectionMatrix();

}

bool CCamera::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_RS = CDevice::GetInst()->GetResolution();

	return true;
}

void CCamera::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);


	Resolution  RS = CDevice::GetInst()->GetResolution();

	m_CameraBottom = GetWorldPos().y - RS.Height / 2.f;
	m_CameraLeft = GetWorldPos().x - RS.Width / 2.f;
}

void CCamera::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	if (m_bCameraFocus)
		CameraFocus(DeltaTime);

	AddWorldPos(-m_ShakePos.x, -m_ShakePos.y, 0.f);
	CameraShake(DeltaTime);



	// 카메라의 월드 행렬 * 카메라의 월드 행렬의 역행렬(전치 행렬) = 단위 행렬 (뷰 좌표계)
	m_matView.Identity();

	for (int i = 0; i < AXIS_END; ++i)
	{
		// 카메라의 월드 행렬을 계산하는 과정이다.
		// 각 행에 카메라의 월드 축을 넣어준다.
		Vector3	Axis = GetAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// 회전부분을 전치해서 최종 View의 회전으로 만들어준다.
	// 뷰 좌표계에서도 모든 축은 서로 정직교 하므로 직교행렬이 만들어지는데,
	// 직교 행렬은 전치 행렬과 역행렬이 같기 때문에**
	// 뷰 변환에 사용되는 행렬은 카메라의 월드 행렬의 전치행렬이다.
	m_matView.Transpose();

	for (int i = 0; i < AXIS_END; ++i)
	{
		Vector3	Axis = GetAxis((AXIS)i);
		Vector3	Pos = GetWorldPos() * -1.f;
		m_matView[3][i] = Axis.Dot(Pos);
	}

	/*
	1 0 0 0		// 1행~3행 까지는 회전 행렬 부분이기 때문에 카메라가 원점이려면 회전값이 0, 즉 모든 축이 단위 벡터여야 한다.
	0 1 0 0
	0 0 1 0
	-x -y -z 1	// 이동행렬 부분. 카메라가 원점이어야 하므로, 포지션 값을 월드 위치 고대로 뺴주면 된다.

	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1

	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	-P.X -P.Y -P.Z 1


	WorldCameraAxisX * ViewMatrix = 1, 0, 0
	WorldCameraAxisY * ViewMatrix = 0, 1, 0
	WorldCAmeraAxisZ * ViewMatrix = 0, 0, 1

	Xx Xy Xz   Xx Yx Zx    1 0 0
	Yx Yy Yz * Xy Yy Zy = 0 1 0
	Zx Zy Zz   Xz Yz Zz    0 0 1

	서로 정직교 하는 행렬을 직교행렬이라고 한다.
	직교행렬은 전치행렬과 역행렬이 같다.
	*/
}

void CCamera::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CCamera::PrevRender(float DeltaTime)
{
	CSceneComponent::PrevRender(DeltaTime);
}

void CCamera::Render(float DeltaTime)
{
	CSceneComponent::Render(DeltaTime);
}

CCamera* CCamera::Clone()
{
	return new CCamera(*this);
}
