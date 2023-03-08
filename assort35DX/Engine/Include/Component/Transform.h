#pragma once

#include "../GameEngine.h"

class CTransform
{
	friend class CSceneComponent;

private:
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();

private:
	class CScene* m_pScene;
	class CGameObject* m_pOwnerObject;
	class CSceneComponent* m_pOwner;

private:
	CTransform* m_pParent;
	std::vector<CTransform*>    m_vecChild;
	class CTransformConstantBuffer* m_pCBuffer;
	float		m_DefaultZ;
	float		m_AddedZ;

private:
	bool    m_Animation2DEnable;

public:
	void SetDefaultZ(float Z);
	void SetAddedZ(float Z);

	float GetDefaultZ()	const
	{
		return m_DefaultZ;
	}

	void SetAnimation2DEnable(bool Enable)
	{
		m_Animation2DEnable = Enable;
	}

private:
	Vector3 m_VelocityScale;
	Vector3 m_VelocityRot;
	Vector3 m_Velocity;
	Vector3 m_Axis[AXIS_END];
	bool    m_InheritScale;
	bool    m_InheritRotX;
	bool    m_InheritRotY;
	bool    m_InheritRotZ;
	bool    m_InheritPosZ;
	bool    m_UpdateScale;
	bool    m_UpdateRot;
	bool    m_UpdatePos;
	bool	m_UpdatePosZ;
	bool	m_PhysicsSimulate;
	bool	m_BounceGravity;
	bool	m_IsBounce;

	Vector3		m_vMoveDir;
	Vector3		m_vMove;
	Vector3		m_vPrevMove;
	Vector3		m_Force;
	float		m_MoveSpeed;
	bool		m_IsGround;
	float		m_FallTime;
	bool		m_IsFalling;
	float		m_GravityAccel;
	/*bool		m_bUseAccel;*/
	float		m_FallStartY;
	bool		m_Jump;
	int			m_JumpMaxCount;
	int			m_CurrentJumpCount;
	float		m_JumpVelocity;
	float		m_VelocityY;
	float		m_VelocityMax;
	bool		m_IsInAir;
	float		m_BounceForce;
	float		m_Elasticity;
	float		m_VelocityX;
	float		m_Friction;

public:
	void Jump();
	void JumpEnd();
	void Falling();
	void InAir();

public:
	void UpdateSRT();

public:
	int GetCurrentJumpCount() const
	{
		return m_CurrentJumpCount;
	}

	float GetFriction()	const
	{
		return m_Friction;
	}

	float GetBounceForce()	const
	{
		return m_BounceForce;
	}

	bool IsInAir()	const
	{
		return m_IsInAir;
	}

	bool IsFalling()	const
	{
		return m_IsFalling;
	}

	bool IsGround()	const
	{
		return m_IsGround;
	}

	bool IsJump()	const
	{
		return m_Jump;
	}

	float GetMoveSpeed()	const
	{
		return m_MoveSpeed;
	}

	Vector3 GetMove()	const
	{
		return m_vMove;
	}

	Vector3 GetPrevMove()	const
	{
		return m_vPrevMove;
	}
	
	Vector3 GetMoveDir()	const
	{
		return m_vMoveDir;
	}

	bool GetPhysicsSimulate()	const
	{
		return m_PhysicsSimulate;
	}

	Vector3 GetVelocityScale()  const
	{
		return m_VelocityScale;
	}

	Vector3 GetVelocityRot()    const
	{
		return m_VelocityRot;
	}

	Vector3 GetVelocity()   const
	{
		return m_Velocity;
	}

	Vector3 GetAxis(AXIS Axis)  const
	{
		return m_Axis[Axis];
	}

	Vector2 GetWorldPos2D()	const
	{
		return Vector2(m_WorldPos.x, m_WorldPos.y);
	}

public:
	void RelativePosFlip(bool x, bool y, bool z)
	{
		if(x)
			m_RelativePos.x *= -1.f;

		if (y)
			m_RelativePos.y *= -1.f;

		if (z)
			m_RelativePos.z *= -1.f;
	}

	void SetFriction(float Friction)
	{
		m_Friction = Friction;
	}

	void SetElasticity(float Elasticity)
	{
		m_Elasticity = Elasticity;
	}

	void SetBounceGravity(bool Value)
	{
		m_BounceGravity = Value;
	}

	void SetJumpMaxCount(int JumpCount)
	{
		m_JumpMaxCount = JumpCount;
	}

	void SetJumpVelocity(float Velocity)
	{
		m_JumpVelocity = Velocity;
	}

	void SetMoveX(float x)
	{
		m_vMove.x = x;
	}
	void SetMoveY(float y)
	{
		m_vMove.y = y;
	}
	void SetMoveZ(float z)
	{
		m_vMove.z = z;
	}

	void SetMove(float x, float y, float z)
	{
		m_vMove.x = x;
		m_vMove.y = y;
		m_vMove.z = z;
	}

	void SetMove(const Vector3& Move)
	{
		m_vMove = Move;
	}

	void SetPrevMove(const Vector3& PrevMove)
	{
		m_vPrevMove = PrevMove;
	}

	void AddMove(float x, float y, float z)
	{
		m_vMove.x += x;
		m_vMove.y += y;
		m_vMove.z += z;
	}

	void AddMove(const Vector3& Move)
	{
		m_vMove += Move;
	}

	void AddForce(const Vector3& Force)
	{
		m_Force += Force;
	}

public:
	void SetGravityAccel(float Accel)
	{
		m_GravityAccel = Accel;
	}

	void SetMoveSpeed(float Speed)
	{
		m_MoveSpeed = Speed;
	}

	void SetMoveDir(const Vector3 Dir)
	{
		m_vMoveDir = Dir;
	}

	void SetMoveDir(float x, float y, float z)
	{
		m_vMoveDir.x = x;
		m_vMoveDir.y = y;
		m_vMoveDir.z = z;
	}

	void SetPhysicsSimulate(bool Simulate)
	{
		if (!m_PhysicsSimulate && Simulate)
		{
			m_FallStartY = m_WorldPos.y;
			m_FallTime = 0.f;
		}

		m_PhysicsSimulate = Simulate;
	}

	void SetInheritScale(bool Inherit)
	{
		m_InheritScale = Inherit;
	}

	void SetInheritRotX(bool Inherit)
	{
		m_InheritRotX = Inherit;
	}

	void SetInheritRotY(bool Inherit)
	{
		m_InheritRotY = Inherit;
	}

	void SetInheritRotZ(bool Inherit)
	{
		m_InheritRotZ = Inherit;
	}

	void SetInheritPosZ(bool Inherit)
	{
		m_InheritPosZ = Inherit;
	}

	void SetUpdatePosZ(bool UpdatePosZ)
	{
		m_UpdatePosZ = UpdatePosZ;
	}

	void InheritScale();
	void InheritRot();
	void InheritPos();

public:
	bool GetInheritPosZ()	const
	{
		return m_InheritPosZ;
	}

	bool GetUpdatePosZ()	const
	{
		return m_UpdatePosZ;
	}

private:    // Relative Data
	Vector3 m_RelativeScale;
	Vector3 m_RelativeRot;
	Vector3 m_RelativePos;

public:
	Vector3 GetRelativeScale()  const
	{
		return m_RelativeScale;
	}

	Vector3 GetRelativeRotation()    const
	{
		return m_RelativeRot;
	}

	Vector3 GetRelativePos()    const
	{
		return m_RelativePos;
	}

public:
	void SetRelativeScale(const Vector3& Scale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeRotation(const Vector3& Rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativePos(const Vector3& Pos);
	void SetRelativePos(float x, float y, float z);
	void SetRelativePosX(float x);
	void SetRelativePosY(float y);
	void SetRelativePosZ(float z);
	void AddRelativeScale(const Vector3& Scale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeRotation(const Vector3& Rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);
	void AddRelativePos(const Vector3& Pos);
	void AddRelativePos(float x, float y, float z);

private:    // World Data
	Vector3 m_WorldScale;
	Vector3 m_WorldRot;
	Vector3 m_WorldPos;
	Vector3 m_Pivot;
	Vector3 m_MeshSize;

	Matrix  m_matScale;
	Matrix  m_matRot;
	Matrix  m_matPos;
	Matrix  m_matWorld;

public:
	Vector3 GetWorldScale() const
	{
		return m_WorldScale;
	}

	Vector3 GetWorldRotation()  const
	{
		return m_WorldRot;
	}

	Vector3 GetWorldPos()   const
	{
		return m_WorldPos;
	}

	Vector3 GetPivot()  const
	{
		return m_Pivot;
	}

	Vector3 GetMeshSize()   const
	{
		return m_MeshSize;
	}

	Matrix GetScaleMatrix() const
	{
		return m_matScale;
	}

	Matrix GetRotatinMatrix()   const
	{
		return m_matRot;
	}

	Matrix GetTranslationMatrix()   const
	{
		return m_matPos;
	}

	Matrix GetWorldMatrix() const
	{
		return m_matWorld;
	}
	
public:
	void SetPivot(const Vector3& Pivot)
	{
		m_Pivot = Pivot;
	}

	void SetPivot(float x, float y, float z)
	{
		m_Pivot = Vector3(x, y, z);
	}

	void SetMeshSize(const Vector3& Size)
	{
		m_MeshSize = Size;
	}

	void SetMeshSize(float x, float y, float z)
	{
		m_MeshSize = Vector3(x, y, z);
	}

public:
	void SetWorldScale(const Vector3& Scale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRotation(const Vector3& Rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldPos2D(const Vector2& Pos);
	void SetWorldPos2D(float x, float y);
	void SetWorldPos(const Vector3& Pos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldPosX(float x);
	void SetWorldPosY(float y);
	void SetWorldPosZ(float z);
	void AddWorldScale(const Vector3& Scale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldRotation(const Vector3& Rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
	void AddWorldPos(const Vector3& Pos);
	void AddWorldPos(float x, float y, float z);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void PrevRender(float DeltaTime);
	void SetTransform();
	void ComputeWorld();
	CTransform* Clone();
	void ClearState();
};

