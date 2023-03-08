#pragma once

#include "../Ref.h"
#include "Material.h"

// �� �����̳ʴ� 2D���� �� �ǹ� ������ 3D���� ������ ���� ����
struct MeshContainer
{
    VertexBuffer    VB;
    D3D11_PRIMITIVE_TOPOLOGY    Primitive;  // �޽��� �⺻ ���� ������
    std::vector<IndexBuffer*>    vecIB;     // �ش� �޽ø� ������ ������ ������ �ڷᱸ��. �ٵ� �� ������? ������߰ڴ�.
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

    // �ϳ��� ���� �޽ô� �Ӹ�, ����, �ٸ� ���� ���� �������� �޽� �����̳ʷ� ������ �� �����Ƿ� ���ͷ� �޽õ��� �����Ѵ�.
    std::vector<MeshContainer*>     m_vecMeshContainer;
    std::vector<MeshSlot*>          m_vecMeshSlot;
    std::vector<CSharedPtr<CMaterial>>   m_vecMaterialSlot;

    Vector3     m_Min;      // �ش� ���� �޽� �� ��ġ���� ���� ���� ������ ��
    Vector3     m_Max;      // �ش� ���� �޽� �� ��ġ���� ���� ū ������ ��
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
    // �޽ø� �����ϱ� ���� ����� �ΰ����� �ִµ� 1. ������ �̿��Ͽ� ���� ����� ��, 2. 3D�ƽ��� ���߰��� ������� �޽ø� �ε��ϴ� ���.
    // ���� �ܰ迡�� 1�� ������� ����ؼ� ���θ� ���� �� ����.
    // 1�� ���� : ���ؽ� ����. ���ؽ� ������ Ư������ �ʰ� ����� ���� �������� �������·� ������ �� �ֱ� ������ void �����͸� ����.
    //           �׷��� �ش� ���ڸ� ������ �� ���� ���ϴ� Ÿ������ ����ȯ�� �� �Ŀ� �����ؾ� ��!
    // 6��° ���ں��ʹ� �ε��� ���ۿ� ���� �����ε�, �ε��� ���۴� �����ϸ� ������ ���� ��쵵 ����.. �׷��� Default ���ڷ� ä���ش�!
    // �ε��� ���� : ���� ���۴� ������ ���� ������ �����ϴ� �����̴�. �ε��� ���۴� �� ������ ��� ������ �������� ���� ���� ���� ������ ������ ������.
    virtual bool CreateMesh(void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
        void* pIndices = nullptr, int IdxCount = 0, int IdxSize = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
        DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
    virtual void Render();
    virtual void Render(int MaterialSlotIndex);
    virtual void RenderInstancing(unsigned int Count);
    virtual void RenderInstancing(int MaterialSlotIndex, unsigned int Count);
};

