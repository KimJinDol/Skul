
#include "Transform.h"
#include "../Resource/TransformConstantBuffer.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "Camera.h"
#include "../BasicMath.h"

CTransform::CTransform()    :
    m_Animation2DEnable(false),
    m_pParent(nullptr),
    m_InheritScale(false),
    m_InheritRotX(false),
    m_InheritRotY(false),
    m_InheritRotZ(true),
    m_InheritPosZ(true),
    m_UpdateScale(true),
    m_UpdateRot(true),
    m_UpdatePos(true),
    m_UpdatePosZ(false),
    m_DefaultZ(0.f),
    m_AddedZ(0.f),
    m_PhysicsSimulate(false),
    m_BounceGravity(false),
    m_BounceForce(3.f),
    m_IsBounce(false),
    m_Elasticity(0.5f),
    m_MoveSpeed(30.f),
    m_IsGround(false),
    m_FallTime(0.f),
    m_IsFalling(false),
    m_GravityAccel(13.f),
    m_FallStartY(0.f),
    m_Jump(false),
    m_JumpMaxCount(1),
    m_CurrentJumpCount(0),
    m_JumpVelocity(0.f),
    m_VelocityY(0.f),
    m_VelocityMax(-800.f),
    m_IsInAir(false),
    m_VelocityX(0.f),
    m_Friction(0.f)
{
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i];
    }

    m_MeshSize = Vector3(1.f, 1.f, 1.f);
    m_RelativeScale = Vector3(1.f, 1.f, 1.f);
    m_WorldScale = Vector3(1.f, 1.f, 1.f);
}

CTransform::CTransform(const CTransform& transform)
{
    *this = transform;

    m_pCBuffer = transform.m_pCBuffer->Clone();
}

CTransform::~CTransform()
{
    SAFE_DELETE(m_pCBuffer);
}

void CTransform::SetDefaultZ(float Z)
{
    m_DefaultZ = Z;
    //m_DefaultZ = BasicMath::Clamp<float>(Z, 0.f, 1.f);
}

void CTransform::SetAddedZ(float Z)
{
    m_AddedZ = Z;
}

void CTransform::Jump()
{
    if (m_CurrentJumpCount >= m_JumpMaxCount) return;

    m_CurrentJumpCount++;
    m_IsGround = false;
    m_Jump = true;
    m_FallStartY = m_WorldPos.y;
    m_FallTime = 0.f;
    m_vMoveDir.y = 1.f;
    m_IsInAir = true;
}

void CTransform::JumpEnd()
{
    m_CurrentJumpCount = 0;
    m_IsGround = true;
    m_Jump = false;
    m_FallStartY = m_WorldPos.y;
    m_FallTime = 0.f;
    m_IsFalling = false;
    m_IsInAir = false;
    m_vMoveDir.y = 0.f;
}

void CTransform::Falling()
{
    m_FallStartY = m_WorldPos.y;
    m_FallTime = 0.f;
    m_IsGround = false;
    m_IsFalling = true;
    m_IsInAir = true;
    m_Jump = false;
    m_vMoveDir.y = -1.f;
}

void CTransform::InAir()
{
    m_IsInAir = true;
    m_FallStartY = m_WorldPos.y;
    m_FallTime = 0.f;
    m_IsGround = false;
}

void CTransform::UpdateSRT()
{
    if (m_UpdateScale)
        m_matScale.Scaling(m_WorldScale);

    if (m_UpdateRot)
        m_matRot.Rotation(m_WorldRot);

    if (m_UpdatePos)
        m_matPos.Translation(m_WorldPos);

    m_matWorld = m_matScale * m_matRot * m_matPos;
}

void CTransform::InheritScale()
{
    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::InheritRot()
{
    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;

        if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
            InheritPos();
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::InheritPos()
{
    // 부모의 회전값이 존재한다면 부모의 회전으로부터 영향을 받아야 할 경우 해당 회전을
    // 적용받아서 공전을 할 수 있게 해야한다.
    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

CTransform* CTransform::Clone()
{
    return new CTransform(*this);
}

void CTransform::ClearState()
{
    m_VelocityScale = Vector3::Zero;
    m_VelocityRot = Vector3::Zero;
    m_Velocity = Vector3::Zero;
}

void CTransform::SetRelativeScale(const Vector3& Scale)
{
    m_VelocityScale = Scale - m_RelativeScale;

    m_RelativeScale = Scale;

    m_WorldScale = m_RelativeScale;

    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
    SetRelativeScale(Vector3(x, y, z));
}

void CTransform::SetRelativeRotation(const Vector3& Rot)
{
    m_VelocityRot = Rot - m_RelativeRot;

    m_RelativeRot = Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot = Rot - m_RelativeRot;

    m_RelativeRot = Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotationX(float x)
{
    m_VelocityRot.x = x - m_RelativeRot.x;

    m_RelativeRot.x = x;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotationY(float y)
{
    m_VelocityRot.y = y - m_RelativeRot.y;

    m_RelativeRot.y = y;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotationZ(float z)
{
    m_VelocityRot.z = z - m_RelativeRot.z;

    m_RelativeRot.z = z;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativePos(const Vector3& Pos)
{
    m_Velocity = Pos - m_RelativePos;

    m_RelativePos = Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetRelativePos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity = Pos - m_RelativePos;

    m_RelativePos = Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetRelativePosX(float x)
{
    float Pos = x;

    m_Velocity.x = Pos - m_RelativePos.x;

    m_RelativePos.x = Pos;

    //m_WorldPos.x = m_RelativePos.x;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetRelativePosY(float y)
{
    float Pos = y;

    m_Velocity.y = Pos - m_RelativePos.y;

    m_RelativePos.y = Pos;

    //m_WorldPos.y = m_RelativePos.y;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetRelativePosZ(float z)
{
    float Pos = z;

    m_Velocity.z = Pos - m_RelativePos.z;

    m_RelativePos.z = Pos;

    //m_WorldPos.z = m_RelativePos.z;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddRelativeScale(const Vector3& Scale)
{
    m_VelocityScale += Scale;

    m_RelativeScale += Scale;

    m_WorldScale = m_RelativeScale;

    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
    Vector3 Scale(x, y, z);

    m_VelocityScale += Scale;

    m_RelativeScale += Scale;

    m_WorldScale = m_RelativeScale;

    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddRelativeRotation(const Vector3& Rot)
{
    m_VelocityRot += Rot;

    m_RelativeRot += Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot += Rot;

    m_RelativeRot += Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotationX(float x)
{
    m_VelocityRot.x += x;

    m_RelativeRot.x += x;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotationY(float y)
{
    m_VelocityRot.y += y;

    m_RelativeRot.y += y;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotationZ(float z)
{
    m_VelocityRot.z += z;

    m_RelativeRot.z += z;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativePos(const Vector3& Pos)
{
    m_Velocity += Pos;

    m_RelativePos += Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddRelativePos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity += Pos;

    m_RelativePos += Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldScale(const Vector3& Scale)
{
    m_VelocityScale = Scale - m_WorldScale;

    m_WorldScale = Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::SetWorldScale(float x, float y, float z)
{
    Vector3 Scale(x, y, z);

    m_VelocityScale = Scale - m_WorldScale;

    m_WorldScale = Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::SetWorldRotation(const Vector3& Rot)
{
    m_VelocityRot = Rot - m_WorldRot;

    m_WorldRot = Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot = Rot - m_WorldRot;

    m_WorldRot = Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotationX(float x)
{
    m_VelocityRot.x = x - m_WorldRot.x;

    m_WorldRot.x = x;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotationY(float y)
{
    m_VelocityRot.y = y - m_WorldRot.y;

    m_WorldRot.y = y;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotationZ(float z)
{
    m_VelocityRot.z = z - m_WorldRot.z;

    m_WorldRot.z = z;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldPos2D(const Vector2& Pos)
{
    Vector3 Pos3D(Pos.x, Pos.y, GetWorldPos().z);

    m_Velocity = Pos3D - m_WorldPos;

    m_WorldPos.x = Pos3D.x;
    m_WorldPos.y = Pos3D.y;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldPos2D(float x, float y)
{
    Vector3 Pos(x, y, m_WorldPos.z);

    m_Velocity = Pos - m_WorldPos;

    m_WorldPos.x = Pos.x;
    m_WorldPos.y = Pos.y;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldPos(const Vector3& Pos)
{
    m_Velocity = Pos - m_WorldPos;

    m_WorldPos.x = Pos.x;
    m_WorldPos.y = Pos.y;
    m_WorldPos.z = Pos.z;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldPos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity = Vector3(x, y, z) - m_WorldPos;

    m_WorldPos.x = Pos.x;
    m_WorldPos.y = Pos.y;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldPosX(float x)
{
    float Pos = x;

    m_Velocity.x = Pos - m_WorldPos.x;

    m_WorldPos.x = Pos;

    m_RelativePos.x = m_WorldPos.x;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldPosY(float y)
{
    float Pos = y;

    m_Velocity.y = Pos - m_WorldPos.y;

    m_WorldPos.y = Pos;

    m_RelativePos.y = m_WorldPos.y;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldPosZ(float z)
{
    float Pos = z;

    m_Velocity.z = Pos - m_WorldPos.z;

    m_WorldPos.z = Pos;

    m_RelativePos.z = m_WorldPos.z;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddWorldScale(const Vector3& Scale)
{
    m_VelocityScale += Scale;

    m_WorldScale += Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddWorldScale(float x, float y, float z)
{
    Vector3 Scale(x, y, z);

    m_VelocityScale += Scale;

    m_WorldScale += Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddWorldRotation(const Vector3& Rot)
{
    m_VelocityRot += Rot;

    m_WorldRot += Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot += Rot;

    m_WorldRot += Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotationX(float x)
{
    m_VelocityRot.x += x;

    m_WorldRot.x += x;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotationY(float y)
{
    m_VelocityRot.y += y;

    m_WorldRot.y += y;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotationZ(float z)
{
    m_VelocityRot.z += z;

    m_WorldRot.z += z;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldPos(const Vector3& Pos)
{
    m_Velocity += Pos;

    m_WorldPos += Pos;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddWorldPos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity += Pos;

    m_WorldPos += Pos;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::Start()
{
    m_VelocityScale = Vector3::Zero;
    m_VelocityRot = Vector3::Zero;
    m_Velocity = Vector3::Zero;

    InheritScale();
    InheritRot();
    InheritPos();
    
    m_FallStartY = m_WorldPos.y;
}

bool CTransform::Init()
{
    m_pCBuffer = new CTransformConstantBuffer;

    m_pCBuffer->Init();
    
    return true;
}

void CTransform::Update(float DeltaTime)
{
}

void CTransform::PostUpdate(float DeltaTime)
{
    // 중력을 적용한다.

    if (m_pOwnerObject && m_pOwner == m_pOwnerObject->GetRootComponent())
    {
        if (!m_IsGround && m_IsInAir && m_PhysicsSimulate)
        {
            float	SaveY = 0.f;

            m_FallTime += DeltaTime * m_GravityAccel;

            float Velocity = 0.f;

            // t초 후 y값	
            // V : 속도     A : 가속도    G : 중력
            // y = V * A - 0.5f * G * A * A
            // 
            // 0 = -0.5GA^2 + VA - y
            // (-b +- 루트(b^2 - 4ac) / 2a

            if (m_Jump)
                Velocity = (m_JumpVelocity * m_FallTime);

            if (m_IsBounce && m_BounceGravity)
            {
                Velocity = m_BounceForce * m_FallTime;
            }

            m_VelocityY = m_FallStartY + Velocity - (0.5f * GRAVITY * m_FallTime * m_FallTime) - m_WorldPos.y;

            m_VelocityY /= DeltaTime; // 초속 구하기.

            m_VelocityY = m_VelocityY < m_VelocityMax ? m_VelocityMax : m_VelocityY;
            m_vMove.y += m_VelocityY * DeltaTime;


            if (m_vMove.y < 0.f)
                m_IsFalling = true;
            else
                m_IsFalling = false;

        }
        else if (m_IsGround)
        {
            if (m_BounceGravity)
            {
                m_BounceForce = -0.1f * m_VelocityY * m_Elasticity;

                if (m_BounceForce > 2.f)
                {
                    m_IsBounce = true;
                    m_IsGround = false;
                }
                else
                {
                    m_IsBounce = false;
                }
            }

            m_FallTime = 0.f;
            m_VelocityY = 0.f;
            m_IsFalling = false;
            m_FallStartY = m_WorldPos.y;
        }
    }


    if (m_IsGround && m_Friction)
        m_Force.x -= m_Force.x * m_Friction * DeltaTime;

    m_vMove += m_Force * DeltaTime;


    AddWorldPos(m_vMove);

    m_vMoveDir.x = 0.f;

    if (m_vMove.x > 0)
        m_vMoveDir.x = 1.f;
    else if (m_vMove.x < 0)
        m_vMoveDir.x = -1.f;


    /*UpdateSRT();*/
}

void CTransform::PrevRender(float DeltaTime)
{
    if (m_UpdatePosZ)
    {
        float z = 1.f / (float)Render_Type_2D::RT2D_End;
        m_WorldPos.z = m_DefaultZ * z + m_AddedZ;
    }
}

void CTransform::SetTransform()
{   
    m_pCBuffer->SetAnimation2DEnable(m_Animation2DEnable);

    m_pCBuffer->SetWorldMatrix(m_matWorld);

    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();
    
    Matrix  matView = Camera->GetViewMatrix();

    m_pCBuffer->SetViewMatrix(matView);

    Matrix  matProj = Camera->GetProjMatrix();

    m_pCBuffer->SetProjMatrix(matProj);

    m_pCBuffer->SetPivot(m_Pivot);
    m_pCBuffer->SetMeshSize(m_MeshSize);

    m_pCBuffer->UpdateCBuffer();
}

void CTransform::ComputeWorld()
{
    m_matWorld = m_matScale * m_matRot * m_matPos;
}
