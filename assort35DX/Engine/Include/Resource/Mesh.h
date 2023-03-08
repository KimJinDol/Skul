#pragma once

#include "../Ref.h"
#include "Material.h"

// 이 컨테이너는 2D에선 별 의미 없지만 3D에서 유용히 쓰일 예정
struct MeshContainer
{
    VertexBuffer    VB;
    D3D11_PRIMITIVE_TOPOLOGY    Primitive;  // 메시의 기본 도형 위상구조
    std::vector<IndexBuffer*>    vecIB;     // 해당 메시를 구성을 정점을 보관할 자료구조. 근데 왜 벡터지? 여쭤봐야겠다.
};

struct MeshSlot
{
    VertexBuffer    VB;
    D3D11_PRIMITIVE_TOPOLOGY    Primitive;
    IndexBuffer* IB;

    MeshSlot() :
        IB(nullptr)
    {
    }
};

class CMesh :
    public CRef
{
    friend class CResourceManager;
    friend class CSceneResource;

protected:
    CMesh();
    virtual ~CMesh() = 0;

protected:
    class CScene* m_pScene;

    // 하나의 최종 메시는 머리, 몸통, 다리 등등과 같이 여러개의 메시 컨테이너로 구성될 수 있으므로 벡터로 메시들을 저장한다.
    std::vector<MeshContainer*>     m_vecMeshContainer;
    std::vector<MeshSlot*>          m_vecMeshSlot;
    std::vector<CSharedPtr<CMaterial>>   m_vecMaterialSlot;

    Vector3     m_Min;      // 해당 최종 메시 중 위치값이 가장 작은 정점의 값
    Vector3     m_Max;      // 해당 최종 메시 중 위치값이 가장 큰 정점의 값
    Mesh_Type   m_MeshType; // Sprite, StaticMesh, AnimationMesh

public:
    const std::vector<CSharedPtr<CMaterial>>* GetMaterialSlots()    const
    {
        return &m_vecMaterialSlot;
    }

    const CMaterial* GetMaterialSlot(int SlotIndex) const
    {
        return m_vecMaterialSlot[SlotIndex].Get();
    }

    Mesh_Type GetMeshType() const
    {
        return m_MeshType;
    }

    Vector3 GetMin()    const
    {
        return m_Min;
    }

    Vector3 GetMax()    const
    {
        return m_Max;
    }

public:
    void AddMaterialSlot(CMaterial* pMaterial)
    {
        m_vecMaterialSlot.push_back(pMaterial);
    }

    void AddMaterialSlot(const std::string& Name);

public:
    // 메시를 구축하기 위한 방법은 두가지가 있는데 1. 정점을 이용하여 직접 만드는 것, 2. 3D맥스나 마야같이 만들어진 메시를 로딩하는 방법.
    // 현재 단계에선 1번 방법만을 사용해서 공부를 진행 할 것임.
    // 1번 인자 : 버텍스 정보. 버텍스 정보는 특정되지 않고 사용자 정의 형식으로 여러형태로 존재할 수 있기 떄문에 void 포인터를 쓴다.
    //           그래서 해당 인자를 역참조 할 때엔 원하는 타입으로 형변환을 한 후에 접근해야 함!
    // 6번째 인자부터는 인덱스 버퍼에 대한 내용인데, 인덱스 버퍼는 웬만하면 있지만 없는 경우도 있음.. 그래서 Default 인자로 채워준다!
    // 인덱스 버퍼 : 정점 버퍼는 정점에 대한 정보를 저장하는 버퍼이다. 인덱스 버퍼는 이 정점을 어떻게 나열할 것인지에 대한 나열 순서 정보를 가지는 버퍼임.
    virtual bool CreateMesh(void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
        void* pIndices = nullptr, int IdxCount = 0, int IdxSize = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
        DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
    virtual void Render();
    virtual void Render(int MaterialSlotIndex);
    virtual void RenderInstancing(unsigned int Count);
    virtual void RenderInstancing(int MaterialSlotIndex, unsigned int Count);
};

