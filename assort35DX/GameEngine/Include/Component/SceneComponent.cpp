
#include "SceneComponent.h"
#include "Transform.h"
#include "../GameObject.h"

CSceneComponent::CSceneComponent()
{
    m_pTransform = new CTransform;

    m_pTransform->m_pOwner = this;
    m_pParent = nullptr;

    m_SceneComponentType = SceneComponent_Type::Scene;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)    :
    CComponent(com)
{
    *this = com;

    m_pTransform = com.m_pTransform->Clone();

    m_pTransform->m_pParent = nullptr;
    m_pTransform->m_vecChild.clear();

    m_pTransform->m_pOwner = this;

    m_pParent = nullptr;

    m_vecChild.clear();
    size_t  Size = com.m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        CSceneComponent* CloneCom = m_vecChild[i]->Clone();

        CloneCom->m_pParent = this;

        m_vecChild.push_back(CloneCom);

        CloneCom->m_pTransform->m_pParent = m_pTransform;

        m_pTransform->m_vecChild.push_back(CloneCom->m_pTransform);
    }
}

CSceneComponent::~CSceneComponent()
{
    SAFE_DELETE(m_pTransform);
}

void CSceneComponent::Active(bool bActive)
{
    CComponent::Active(bActive);

    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->Active(bActive);
    }
}

void CSceneComponent::AddChild(CSceneComponent* Child, const std::string& SocketName)
{
    Child->m_pParent = this;
    m_vecChild.push_back(Child);

    Child->m_pTransform->m_pParent = m_pTransform;

    m_pTransform->m_vecChild.push_back(Child->m_pTransform);

    Child->m_pTransform->InheritScale();
    Child->m_pTransform->InheritRot();
    Child->m_pTransform->InheritPos();
}

void CSceneComponent::DeleteChild(CSceneComponent* Child)
{
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        if (m_vecChild[i] == Child)
        {
            auto    iter = m_vecChild.begin() + i;

            m_vecChild.erase(iter);

            auto    iterTr = m_pTransform->m_vecChild.begin() + i;

            m_pTransform->m_vecChild.erase(iterTr);

            break;
        }
    }
}

void CSceneComponent::GetAllComponentName(std::vector<HierarchyName>& vecName)
{
    HierarchyName   NameInfo;
    NameInfo.Name = m_Name;
    
    if (m_pParent)
        NameInfo.ParentName = m_pParent->m_Name;

    vecName.push_back(NameInfo);

    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->GetAllComponentName(vecName);
    }
}

void CSceneComponent::GetAllComponent(std::vector<CSceneComponent*>& vecComponent)
{
    vecComponent.push_back(this);

    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->GetAllComponent(vecComponent);
    }
}

void CSceneComponent::DetatchChild(CSceneComponent* Child)
{
    Child->m_pParent = nullptr;

    Child->m_pTransform->m_pParent = nullptr;

    Child->m_pTransform->SetRelativePos(Child->m_pTransform->GetWorldPos());
    Child->m_pTransform->SetRelativeRotation(Child->m_pTransform->GetWorldRotation());
    Child->m_pTransform->SetRelativeScale(Child->m_pTransform->GetWorldScale());
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& Name)
{
    if (m_Name == Name)
        return this;

    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        CSceneComponent* pFind = m_vecChild[i]->FindComponent(Name);

        if (pFind)
            return pFind;
    }

    return nullptr;
}

void CSceneComponent::Start()
{
    CComponent::Start();

    m_pTransform->Start();

    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->Start();
    }
}

bool CSceneComponent::Init()
{
    m_pTransform->m_pScene = m_pScene;
    m_pTransform->m_pOwnerObject = m_pOwner;

    m_pTransform->Init();

    return true;
}

void CSceneComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);
    m_pTransform->Update(DeltaTime);

    auto    iter = m_vecChild.begin();
    auto    iterEnd = m_vecChild.end();
    auto    iterTr = m_pTransform->m_vecChild.begin();

    for (; iter != iterEnd; )
    {
        if (!(*iter)->IsActive())
        {
            DetatchChild(*iter);
            iter = m_vecChild.erase(iter);
            iterEnd = m_vecChild.end();

            iterTr = m_pTransform->m_vecChild.erase(iterTr);

            continue;
        }

        if ((*iter)->IsEnable())
            (*iter)->Update(DeltaTime);

        ++iter;
        ++iterTr;
    }
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);

    m_pTransform->PostUpdate(DeltaTime);

    auto    iter = m_vecChild.begin();
    auto    iterEnd = m_vecChild.end();
    auto    iterTr = m_pTransform->m_vecChild.begin();

    for (; iter != iterEnd; )
    {
        if (!(*iter)->IsActive())
        {
            DetatchChild(*iter);
            iter = m_vecChild.erase(iter);
            iterEnd = m_vecChild.end();

            iterTr = m_pTransform->m_vecChild.erase(iterTr);

            continue;
        }


        if ((*iter)->IsEnable())
            (*iter)->PostUpdate(DeltaTime);

        ++iter;
        ++iterTr;
    }
}

void CSceneComponent::Collision(float DeltaTime)
{
    CComponent::Collision(DeltaTime);

}

void CSceneComponent::PrevRender(float DeltaTime)
{
    m_pTransform->PrevRender(DeltaTime);

    UpdateSRT();

    CComponent::PrevRender(DeltaTime);

    auto    iter = m_vecChild.begin();
    auto    iterEnd = m_vecChild.end();
    auto    iterTr = m_pTransform->m_vecChild.begin();

    for (; iter != iterEnd; )
    {
        if (!(*iter)->IsActive())
        {
            DetatchChild(*iter);
            iter = m_vecChild.erase(iter);
            iterEnd = m_vecChild.end();

            iterTr = m_pTransform->m_vecChild.erase(iterTr);

            continue;
        }

        if ((*iter)->IsEnable())
            (*iter)->PrevRender(DeltaTime);

        ++iter;
        ++iterTr;
    }

    SetMove(0.f, 0.f, 0.f);
}

void CSceneComponent::Render(float DeltaTime)
{
    // 출력 직전에 자신의 Transform 정보를 Shader로 보내준다.
    m_pTransform->SetTransform();
}

CSceneComponent* CSceneComponent::Clone()
{
    return new CSceneComponent(*this);
}

CTransform* CSceneComponent::GetTransform() const
{
    return m_pTransform;
}

float CSceneComponent::GetMoveSpeed() const
{
    return m_pTransform->GetMoveSpeed();
}

bool CSceneComponent::IsJump() const
{
    return m_pTransform->IsJump();
}

int CSceneComponent::GetCurrentJumpCount() const
{
    return m_pTransform->GetCurrentJumpCount();
}

bool CSceneComponent::IsFalling() const
{
    return m_pTransform->IsFalling();
}

bool CSceneComponent::IsInAir() const
{
    return m_pTransform->IsInAir();
}

Vector3 CSceneComponent::GetMoveDir() const
{
    return m_pTransform->GetMoveDir();
}

void CSceneComponent::Jump()
{
    m_pTransform->Jump();
}

void CSceneComponent::OnFloor()
{
    m_pTransform->JumpEnd();
}

void CSceneComponent::Falling()
{
    m_pTransform->Falling();
}

void CSceneComponent::InAir()
{
    m_pTransform->InAir();
}

void CSceneComponent::UpdateSRT()
{
    m_pTransform->UpdateSRT();
}

void CSceneComponent::SetGravityAccel(float Accel)
{
    m_pTransform->SetGravityAccel(Accel);
}

void CSceneComponent::RelativePosFlip(bool x, bool y, bool z)
{
    m_pTransform->RelativePosFlip(x, y, z);
}

void CSceneComponent::SetJumpMaxCount(int JumpCount)
{
    m_pTransform->SetJumpMaxCount(JumpCount);
}

void CSceneComponent::SetJumpVelocity(float Velocity)
{
    m_pTransform->SetJumpVelocity(Velocity);
}

void CSceneComponent::AddForce(const Vector3& Force)
{
    m_pTransform->AddForce(Force);
}

void CSceneComponent::AddForce(float x, float y, float z)
{
    m_pTransform->AddForce(Vector3(x, y, z));
}

void CSceneComponent::SetMoveX(float x)
{
    m_pTransform->SetMoveX(x);
}

void CSceneComponent::SetMoveY(float y)
{
    m_pTransform->SetMoveY(y);
}

void CSceneComponent::SetMoveZ(float z)
{
    m_pTransform->SetMoveZ(z);
}

void CSceneComponent::SetMove(float x, float y, float z)
{
    m_pTransform->SetMove(x, y, z);
}

void CSceneComponent::SetMove(const Vector3& Move)
{
    m_pTransform->SetMove(Move);
}

void CSceneComponent::AddMove(float x, float y, float z)
{
    m_pTransform->AddMove(x, y, z);
}

void CSceneComponent::AddMove(const Vector3& Move)
{
    m_pTransform->AddMove(Move);
}

void CSceneComponent::SetMoveDir(const Vector3& Dir)
{
    m_pTransform->SetMoveDir(Dir);
}

void CSceneComponent::SetMoveDir(float x, float y, float z)
{
    m_pTransform->SetMoveDir(x, y, z);
}

void CSceneComponent::SetMoveSpeed(float Speed)
{
    m_pTransform->SetMoveSpeed(Speed);
}

void CSceneComponent::SetAnimation2DEnable(bool Enable)
{
    m_pTransform->SetAnimation2DEnable(Enable);
}

float CSceneComponent::GetFriction() const
{
    return m_pTransform->GetFriction();
}

float CSceneComponent::GetBounceForce() const
{
    return m_pTransform->GetBounceForce();
}

bool CSceneComponent::GetPhysicsSimulate() const
{
    return m_pTransform->GetPhysicsSimulate();
}

bool CSceneComponent::IsGround() const
{
    return m_pTransform->IsGround();
}

Vector3 CSceneComponent::GetVelocityScale() const
{
    return m_pTransform->GetVelocityScale();
}

Vector3 CSceneComponent::GetVelocityRot() const
{
    return m_pTransform->GetVelocityRot();
}

Vector3 CSceneComponent::GetVelocity() const
{
    return m_pTransform->GetVelocity();
}

Vector3 CSceneComponent::GetAxis(AXIS Axis) const
{
    return m_pTransform->GetAxis(Axis);
}

void CSceneComponent::SetFriction(float Friction)
{
    m_pTransform->SetFriction(Friction);
}

void CSceneComponent::SetElasticity(float Elasticity)
{
    m_pTransform->SetElasticity(Elasticity);
}

void CSceneComponent::SetBounceGravity(bool Bounce)
{
    m_pTransform->SetBounceGravity(Bounce);
}

void CSceneComponent::SetPhysicsSimulate(bool Simulate)
{
    m_pTransform->SetPhysicsSimulate(Simulate);
}

void CSceneComponent::SetInheritScale(bool Inherit)
{
    m_pTransform->SetInheritScale(Inherit);
}

void CSceneComponent::SetInheritRotX(bool Inherit)
{
    m_pTransform->SetInheritRotX(Inherit);
}

void CSceneComponent::SetInheritRotY(bool Inherit)
{
    m_pTransform->SetInheritRotY(Inherit);
}

void CSceneComponent::SetInheritRotZ(bool Inherit)
{
    m_pTransform->SetInheritRotZ(Inherit);
}

void CSceneComponent::SetInheritPosZ(bool Inherit)
{
    m_pTransform->SetInheritPosZ(Inherit);
}

void CSceneComponent::SetUpdatePosZ(bool UpdatePosZ)
{
    m_pTransform->SetUpdatePosZ(UpdatePosZ);
}

void CSceneComponent::InheritScale()
{
    m_pTransform->InheritScale();
}

void CSceneComponent::InheritRot()
{
    m_pTransform->InheritRot();
}

void CSceneComponent::InheritPos()
{
    m_pTransform->InheritPos();
}

bool CSceneComponent::GetInheritPosZ() const
{
    return m_pTransform->GetInheritPosZ();
}

Vector3 CSceneComponent::GetRelativeScale() const
{
    return m_pTransform->GetRelativeScale();
}

Vector3 CSceneComponent::GetRelativeRotation() const
{
    return m_pTransform->GetRelativeRotation();
}

Vector3 CSceneComponent::GetRelativePos() const
{
    return m_pTransform->GetRelativePos();
}

void CSceneComponent::SetRelativeScale(const Vector3& Scale)
{
    m_pTransform->SetRelativeScale(Scale);
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
    m_pTransform->SetRelativeScale(x, y, z);
}

void CSceneComponent::SetRelativeRotation(const Vector3& Rot)
{
    m_pTransform->SetRelativeRotation(Rot);
}

void CSceneComponent::SetRelativeRotation(float x, float y, float z)
{
    m_pTransform->SetRelativeRotation(x, y, z);
}

void CSceneComponent::SetRelativeRotationX(float x)
{
    m_pTransform->SetRelativeRotationX(x);
}

void CSceneComponent::SetRelativeRotationY(float y)
{
    m_pTransform->SetRelativeRotationY(y);
}

void CSceneComponent::SetRelativeRotationZ(float z)
{
    m_pTransform->SetRelativeRotationZ(z);
}

void CSceneComponent::SetRelativePos(const Vector3& Pos)
{
    m_pTransform->SetRelativePos(Pos);
}

void CSceneComponent::SetRelativePos(float x, float y, float z)
{
    m_pTransform->SetRelativePos(x, y, z);
}

void CSceneComponent::SetRelativePosX(float x)
{
    m_pTransform->SetRelativePosX(x);
}

void CSceneComponent::SetRelativePosY(float y)
{
    m_pTransform->SetRelativePosY(y);
}

void CSceneComponent::SetRelativePosZ(float z)
{
    m_pTransform->SetRelativePosZ(z);
}

void CSceneComponent::AddRelativeScale(const Vector3& Scale)
{
    m_pTransform->AddRelativeScale(Scale);
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
    m_pTransform->AddRelativeScale(x, y, z);
}

void CSceneComponent::AddRelativeRotation(const Vector3& Rot)
{
    m_pTransform->AddRelativeRotation(Rot);
}

void CSceneComponent::AddRelativeRotation(float x, float y, float z)
{
    m_pTransform->AddRelativeRotation(x, y, z);
}

void CSceneComponent::AddRelativeRotationX(float x)
{
    m_pTransform->AddRelativeRotationX(x);
}

void CSceneComponent::AddRelativeRotationY(float y)
{
    m_pTransform->AddRelativeRotationY(y);
}

void CSceneComponent::AddRelativeRotationZ(float z)
{
    m_pTransform->AddRelativeRotationZ(z);
}

void CSceneComponent::AddRelativePos(const Vector3& Pos)
{
    m_pTransform->AddRelativePos(Pos);
}

void CSceneComponent::AddRelativePos(float x, float y, float z)
{
    m_pTransform->AddRelativePos(x, y, z);
}

Vector3 CSceneComponent::GetWorldScale() const
{
    return m_pTransform->GetWorldScale();
}

Vector3 CSceneComponent::GetWorldRotation() const
{
    return m_pTransform->GetWorldRotation();
}

Vector3 CSceneComponent::GetWorldPos() const
{
    return m_pTransform->GetWorldPos();
}

Vector2 CSceneComponent::GetWorldPos2D() const
{
    return m_pTransform->GetWorldPos2D();
}

Vector3 CSceneComponent::GetMove() const
{
    return m_pTransform->GetMove();
}

Vector3 CSceneComponent::GetPivot() const
{
    return m_pTransform->GetPivot();
}

Vector3 CSceneComponent::GetMeshSize() const
{
    return m_pTransform->GetMeshSize();
}

Matrix CSceneComponent::GetScaleMatrix() const
{
    return m_pTransform->GetScaleMatrix();
}

Matrix CSceneComponent::GetRotatinMatrix() const
{
    return m_pTransform->GetRotatinMatrix();
}

Matrix CSceneComponent::GetTranslationMatrix() const
{
    return m_pTransform->GetTranslationMatrix();
}

Matrix CSceneComponent::GetWorldMatrix() const
{
    return m_pTransform->GetWorldMatrix();
}

void CSceneComponent::SetPivot(const Vector3& Pivot)
{
    m_pTransform->SetPivot(Pivot);
}

void CSceneComponent::SetPivot(float x, float y, float z)
{
    m_pTransform->SetPivot(x, y, z);
}

void CSceneComponent::SetMeshSize(const Vector3& Size)
{
    m_pTransform->SetMeshSize(Size);
}

void CSceneComponent::SetMeshSize(float x, float y, float z)
{
    m_pTransform->SetMeshSize(x, y, z);
}

void CSceneComponent::SetWorldScale(const Vector3& Scale)
{
    m_pTransform->SetWorldScale(Scale);
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
    m_pTransform->SetWorldScale(x, y, z);
}

void CSceneComponent::SetWorldRotation(const Vector3& Rot)
{
    m_pTransform->SetWorldRotation(Rot);
}

void CSceneComponent::SetWorldRotation(float x, float y, float z)
{
    m_pTransform->SetWorldRotation(x, y, z);
}

void CSceneComponent::SetWorldRotationX(float x)
{
    m_pTransform->SetWorldRotationX(x);
}

void CSceneComponent::SetWorldRotationY(float y)
{
    m_pTransform->SetWorldRotationY(y);
}

void CSceneComponent::SetWorldRotationZ(float z)
{
    m_pTransform->SetWorldRotationZ(z);
}

void CSceneComponent::SetWorldPos2D(const Vector2& Pos)
{
    m_pTransform->SetWorldPos2D(Pos);
}

void CSceneComponent::SetWorldPos2D(float x, float y)
{
    m_pTransform->SetWorldPos2D(x, y);
}

void CSceneComponent::SetWorldPos(const Vector3& Pos)
{
    m_pTransform->SetWorldPos(Pos);
}

void CSceneComponent::SetWorldPos(float x, float y, float z)
{
    m_pTransform->SetWorldPos(x, y, z);
}

void CSceneComponent::SetWorldPosX(float x)
{
    m_pTransform->SetWorldPosX(x);
}

void CSceneComponent::SetWorldPosY(float y)
{
    m_pTransform->SetWorldPosY(y);
}

void CSceneComponent::SetWorldPosZ(float z)
{
    m_pTransform->SetWorldPosZ(z);
}

void CSceneComponent::AddWorldScale(const Vector3& Scale)
{
    m_pTransform->AddWorldScale(Scale);
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
    m_pTransform->AddWorldScale(x, y, z);
}

void CSceneComponent::AddWorldRotation(const Vector3& Rot)
{
    m_pTransform->AddWorldRotation(Rot);
}

void CSceneComponent::AddWorldRotation(float x, float y, float z)
{
    m_pTransform->AddWorldRotation(x, y, z);
}

void CSceneComponent::AddWorldRotationX(float x)
{
    m_pTransform->AddWorldRotationX(x);
}

void CSceneComponent::AddWorldRotationY(float y)
{
    m_pTransform->AddWorldRotationY(y);
}

void CSceneComponent::AddWorldRotationZ(float z)
{
    m_pTransform->AddWorldRotationZ(z);
}

void CSceneComponent::AddWorldPos(const Vector3& Pos)
{
    m_pTransform->AddWorldPos(Pos);
}

void CSceneComponent::AddWorldPos(float x, float y, float z)
{
    m_pTransform->AddWorldPos(x, y, z);
}

void CSceneComponent::ClearTransformState()
{
    m_pTransform->ClearState();
}
