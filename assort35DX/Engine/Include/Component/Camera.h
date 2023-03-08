#pragma once

#include "SceneComponent.h"

class CCamera :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;
	friend class CViewport;

protected:
	CCamera();
	CCamera(const CCamera& com);
	virtual ~CCamera();

protected:
	Camera_Type m_CameraType;
	Matrix  m_matView;
	Matrix  m_matProj;
	float   m_ViewAngle;
	float   m_Distance;
	float	m_CameraBottom;	// 2D ����
	float	m_CameraLeft;	// 2D ����
	Resolution	m_RS;
	Vector2 m_WorldSize;

	bool	m_bCameraFocus;	// Owner Object�� �ƴ� ������ ��ġ�� ī�޶� ��Ŀ���� �ű�� �۾�
	bool	m_bEndCameraFocus; // Focus �Ű�� ī�޶� ��ġ ����ġ�� ������ �� �� ����
	Vector2	m_FocusPos;
	Vector2 m_FocusInitPos;
	float	m_FocusLerpTime;
	float	m_FocusTime;
	float	m_CurFocusTime;
	float	m_CurFocusLerpTime;

	/*Camera Shake*/
	int       m_AmplitudeX; // X����ũ ����
	float       m_FrequencyX; // x����ũ ������

	int       m_AmplitudeY; // Y����ũ ����
	float       m_FrequencyY; // x����ũ ������

	float       m_ShakeTime; // ����ũ Ÿ��

	bool        m_bIsShake;

	float       m_curFreqTimeX;
	float       m_curFreqTimeY;
	Vector2     m_ShakePos;

	Vector2     m_OriginPos;

	// -----
	std::function<void()>	m_FocusEndFunction;

public:
	bool IsCameraFocus()	const
	{
		return m_bCameraFocus;
	}

public:
	void CameraShake(float AmplitudeX, float FrequencyX, float AmplitudeY, float FrequencyY, float Time);
	void CameraShake(float DeltaTime);

public:
	void SetEnableShake(bool Value, float Time = 0.3f)
	{
		m_ShakeTime = Time;
		m_bIsShake = Value;
	}

	void UnableCameraShake()
	{
		m_bIsShake = false;
	}
	void StartCameraFocus(const Vector2& FocusPos, float LerpTime, float FocusTime = 0.f);
	void CameraFocus(float DeltaTime);
	void EndCameraFocus();
	void Set2DWorldSize(const Vector2& Size);

public:
	Vector2 Get2DWorldSize()	const
	{
		return m_WorldSize;
	}

	Resolution GetResolution()	const
	{
		return m_RS;
	}

	float GetCameraBottom()	const
	{
		return m_CameraBottom;
	}

	float GetCameraLeft()	const
	{
		return m_CameraLeft;
	}

	Camera_Type GetCameraType() const
	{
		return m_CameraType;
	}

	Matrix GetViewMatrix()  const
	{
		return m_matView;
	}

	Matrix GetProjMatrix()  const
	{
		return m_matProj;
	}

public:
	void SetViewAngle(float Angle)
	{
		m_ViewAngle = Angle;

		if (m_CameraType == Camera_Type::Cam3D)
			CreateProjectionMatrix();
	}

	void SetDistance(float Distance)
	{
		m_Distance = Distance;

		CreateProjectionMatrix();
	}

	void SetCameraType(Camera_Type Type)
	{
		m_CameraType = Type;

		CreateProjectionMatrix();
	}

private:
	void CreateProjectionMatrix();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CCamera* Clone();

public:
	template <typename T>
	void SetFocusEndFunction(T* pObj, void(T::* pFunc)())
	{
		m_FocusEndFunction = std::bind(pFunc, pObj);
	}
};

