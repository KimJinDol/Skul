
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
        // �� ������ ������ ������ ���� ������ ������ ������ ��ȯ.
        float	Angle = XMConvertToRadians(m_ViewAngle);
        // �þ߸� ������� �޼� ���� ���� ����� ����ϴ�.
        // 1 ���� : ���� ������ ����� �þ߰�
        // 2 ���� : X : Y�� ��Ⱦ��
        // 3 ���� : ����� Ŭ���� �������� �Ÿ�. 0���� Ŀ����
        // 4 ���� : �� Ŭ���� �������� �Ÿ�. 0���� Ŀ����
        m_matProj = XMMatrixPerspectiveFovLH(Angle,
            RS.Width / (float)RS.Height, 0.1f, m_Distance);
	}
		break;
	case Camera_Type::Cam2D:
	case Camera_Type::CamUI:
    {
        // �޼� ��ǥ�迡 ���� ����� ���� ���� ���� ����� ����ϴ�.
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



	// ī�޶��� ���� ��� * ī�޶��� ���� ����� �����(��ġ ���) = ���� ��� (�� ��ǥ��)
	m_matView.Identity();

	for (int i = 0; i < AXIS_END; ++i)
	{
		// ī�޶��� ���� ����� ����ϴ� �����̴�.
		// �� �࿡ ī�޶��� ���� ���� �־��ش�.
		Vector3	Axis = GetAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// ȸ���κ��� ��ġ�ؼ� ���� View�� ȸ������ ������ش�.
	// �� ��ǥ�迡���� ��� ���� ���� ������ �ϹǷ� ��������� ��������µ�,
	// ���� ����� ��ġ ��İ� ������� ���� ������**
	// �� ��ȯ�� ���Ǵ� ����� ī�޶��� ���� ����� ��ġ����̴�.
	m_matView.Transpose();

	for (int i = 0; i < AXIS_END; ++i)
	{
		Vector3	Axis = GetAxis((AXIS)i);
		Vector3	Pos = GetWorldPos() * -1.f;
		m_matView[3][i] = Axis.Dot(Pos);
	}

	/*
	1 0 0 0		// 1��~3�� ������ ȸ�� ��� �κ��̱� ������ ī�޶� �����̷��� ȸ������ 0, �� ��� ���� ���� ���Ϳ��� �Ѵ�.
	0 1 0 0
	0 0 1 0
	-x -y -z 1	// �̵���� �κ�. ī�޶� �����̾�� �ϹǷ�, ������ ���� ���� ��ġ ���� ���ָ� �ȴ�.

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

	���� ������ �ϴ� ����� ��������̶�� �Ѵ�.
	��������� ��ġ��İ� ������� ����.
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
