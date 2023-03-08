
#include "Mesh.h"
#include "../Device.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CMesh::CMesh() :
    m_pScene(nullptr)
{
    m_Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    m_Max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

CMesh::~CMesh()
{
    auto    iter = m_vecMeshContainer.begin();
    auto    iterEnd = m_vecMeshContainer.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_RELEASE((*iter)->VB.pBuffer);
        SAFE_DELETE_ARRAY((*iter)->VB.pData);

        // 인덱스 버퍼 정보를 날려준다.
        auto    iter1 = (*iter)->vecIB.begin();
        auto    iter1End = (*iter)->vecIB.end();

        for (; iter1 != iter1End; ++iter1)
        {
            SAFE_DELETE_ARRAY((*iter1)->pData);
            SAFE_RELEASE((*iter1)->pBuffer);
            SAFE_DELETE((*iter1));
        }

        SAFE_DELETE((*iter));
    }

    auto    iterSlot = m_vecMeshSlot.begin();
    auto    iterSlotEnd = m_vecMeshSlot.end();

    for (; iterSlot != iterSlotEnd; ++iterSlot)
    {
        SAFE_DELETE((*iterSlot));
    }

    m_vecMeshSlot.clear();
}

void CMesh::AddMaterialSlot(const std::string& Name)
{
    CMaterial* pMaterial = nullptr;

    if (m_pScene)
    {
        pMaterial = m_pScene->GetResource()->FindMaterial(Name);
    }

    else
    {
        pMaterial = CResourceManager::GetInst()->FindMaterial(Name);
    }

    if (pMaterial)
        m_vecMaterialSlot.push_back(pMaterial);
}

bool CMesh::CreateMesh(void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
    void* pIndices, int IdxCount, int IdxSize, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
    MeshContainer* pContainer = new MeshContainer;

    m_vecMeshContainer.push_back(pContainer);

    pContainer->Primitive = Primitive;

    pContainer->VB.Count = VtxCount;
    pContainer->VB.Size = VtxSize;
    pContainer->VB.Usage = VtxUsage;
    pContainer->VB.pData = new char[VtxCount * VtxSize];    // 메시의 정점에 대한 정보를 저장할 데이터를 할당. 정점 개수 * 정점의 사이즈로 해당 메시에 필요한 정점 데이터 크기를 알 수 있다.
    memcpy(pContainer->VB.pData, pVertices, VtxCount * VtxSize);    // 위에서 할당한 메모리에 정점 데이터를 집어넣는다.

    // D3D11_BUFFER_DESC : 생성할 버퍼를 서술하는 구조체.
    D3D11_BUFFER_DESC   VtxDesc = {};

    VtxDesc.Usage = VtxUsage;                       // 버퍼가 쓰일 용도.
                                                    // D3D11_USAGE_DEFAULT : GPU가 버퍼의 자원을 읽고 써야 한다면 이 용도를 지정한다.
                                                    // D3D11_USAGE_IMMUTABLE : 버퍼 생성 후에 전혀 변경하지 않을 거라면 이 용도를 지정한다. GPU의 읽기 전용 용도.
                                                    // D3D11_USAGE_DYNAMIC : 응용 프로그램이 자원의 자료 내용을 자주 갱신해야 한다면 이 용도를 지정한다.
                                                    // D3D11_USAGE_STAGING : 응용 프로그램이 자원의 복사본을 읽어야 한다면 이 용도를 지정. (느린 연산임)
    VtxDesc.ByteWidth = VtxSize * VtxCount;         // 생성할 정점 버퍼의 크기.
    VtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;   // D3D11_BIND_VERTEX_BUFFER : 버텍스 버퍼 용도로 만들어서 바인딩 해 사용할 것이라는 뜻.


    // D3D11_SUBRESOURCE_DATA : 버퍼의 초기화에 사용할 자료를 서술하는 구조체.
    D3D11_SUBRESOURCE_DATA  VtxData = {};
    VtxData.pSysMem = pVertices;    // pSysMem : 정점 버퍼를 초기화할 자료를 담고있는 메모리 배열을 가리키는 포인터.

    if (FAILED(DEVICE->CreateBuffer(&VtxDesc, &VtxData, &pContainer->VB.pBuffer)))
        return false;

    char* pVertexData = (char*)pVertices;

    // Posiiton 값(12바이트), Color 값(16바이트)로 구성된 버텍스 정보에서 제일 앞 메모리부터 += VtxSize를 해주면서 탐색한다.
    // 가장 작은 위치값을 가진 정점의 위치를 m_Min에, 
    // 가장 큰 위치갓을 가진 정점의 위치를 m_Max에 넣어준다.
    for (int i = 0; i < VtxCount; ++i)
    {
        Vector3* pPos = (Vector3*)pVertexData;
        pVertexData += VtxSize;

        if (m_Min.x > pPos->x)
            m_Min.x = pPos->x;

        if (m_Min.y > pPos->y)
            m_Min.y = pPos->y;

        if (m_Min.z > pPos->z)
            m_Min.z = pPos->z;

        if (m_Max.x < pPos->x)
            m_Max.x = pPos->x;

        if (m_Max.y < pPos->y)
            m_Max.y = pPos->y;

        if (m_Max.z < pPos->z)
            m_Max.z = pPos->z;
    }

    // 인덱스 버퍼가 있다면 만들어 넣어준다.
    // 인덱스 버퍼 : 정점을 어떻게 나열할 것인지에 대한 순서 정보를 저장한다.
    if (pIndices)
    {
        IndexBuffer* IB = new IndexBuffer;
        pContainer->vecIB.push_back(IB);

        IB->Count = IdxCount;
        IB->Size = IdxSize;
        IB->Usage = IdxUsage;
        IB->Fmt = Fmt;
        IB->pData = new char[IdxCount * IdxSize];
        memcpy(IB->pData, pIndices, IdxCount * IdxSize);

        D3D11_BUFFER_DESC   IdxBufferDesc = {};

        IdxBufferDesc.Usage = IdxUsage;
        IdxBufferDesc.ByteWidth = IdxCount * IdxSize;
        IdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  IdxData = {};

        IdxData.pSysMem = pIndices;

        if (FAILED(DEVICE->CreateBuffer(&IdxBufferDesc, &IdxData, &IB->pBuffer)))
            return false;
    }

    MeshSlot* pSlot = new MeshSlot;

    pSlot->VB = pContainer->VB;

    if (!pContainer->vecIB.empty())
        pSlot->IB = pContainer->vecIB[0];

    pSlot->Primitive = pContainer->Primitive;

    m_vecMeshSlot.push_back(pSlot);


    return true;
}

void CMesh::Render()
{
    size_t  Size = m_vecMeshContainer.size();

    for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
    {
        unsigned int    Stride = m_vecMeshContainer[i]->VB.Size;
        unsigned int    Offset = 0;

        // IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);
        // 기본 도형을 형성하는 방식을 Direct3D에게 알려 주는 데에 쓰이는 메서드.
        // 이 메서드를 호출해서 한 번 기본도형 위상구조를 설정하면 다음 호출이 있을 때까지 모든 그리기 호출에서
        // 그 위상구조가 적용된다. 그래서 매번 렌더 시마다 Primivive를 설정해주는 것임.
        CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->Primitive);
        CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->VB.pBuffer, &Stride, &Offset);

        size_t  Size1 = m_vecMeshContainer[i]->vecIB.size();

        if (Size1 > 0)
        {
            for (size_t j = 0; j < Size1; ++j)
            {
                if (m_vecMeshContainer[i]->vecIB[j]->pBuffer)
                {
                    CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIB[j]->pBuffer,
                        m_vecMeshContainer[i]->vecIB[j]->Fmt, 0);
                    CONTEXT->DrawIndexed(m_vecMeshContainer[i]->vecIB[j]->Count, 0, 0);
                }
            }
        }

        else
        {
            CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
            CONTEXT->Draw(m_vecMeshContainer[i]->VB.Count, 0);
        }
    }
}

void CMesh::Render(int MaterialSlotIndex)
{
    MeshSlot* pSlot = m_vecMeshSlot[MaterialSlotIndex];

    unsigned int    Stride = pSlot->VB.Size;
    unsigned int    Offset = 0;

    // IA : Input Assembler (입력 조립기의 줄임말)
    // IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) : 기본 도형 위상 구조를 지정하는 함수. topology 타입에 따라 그려지는 방식이 달라지는 것?
    CONTEXT->IASetPrimitiveTopology(pSlot->Primitive);
    // IASetVertexBuffers(...) : 버텍스 버퍼를 지정하는 함수.
    CONTEXT->IASetVertexBuffers(0, 1, &pSlot->VB.pBuffer, &Stride, &Offset);

    if (pSlot->IB)
    {
        // IASetIndexBuffer(...) : 인덱스 버퍼를 지정하는 함수.
        CONTEXT->IASetIndexBuffer(pSlot->IB->pBuffer,
            pSlot->IB->Fmt, 0);
        //
        CONTEXT->DrawIndexed(pSlot->IB->Count, 0, 0);
    }

    else
    {
        CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
        CONTEXT->Draw(pSlot->VB.Count, 0);
    }
}

void CMesh::RenderInstancing(unsigned int Count)
{
    size_t  Size = m_vecMeshContainer.size();
    // 해당 메시를 구성하는 파츠(메시의 일부분들)들만큼 반복문을 돌려준다.
    for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
    {
        unsigned int    Stride = m_vecMeshContainer[i]->VB.Size;    // 해당 일부 메시의 버텍스 버퍼의 사이즈를 구한다.
        unsigned int    Offset = 0;

        CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->Primitive);
        CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->VB.pBuffer, &Stride, &Offset);

        size_t  Size1 = m_vecMeshContainer[i]->vecIB.size();

        if (Size1 > 0)
        {
            for (size_t j = 0; j < Size1; ++j)
            {
                if (m_vecMeshContainer[i]->vecIB[j]->pBuffer)
                {
                    CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIB[j]->pBuffer,
                        m_vecMeshContainer[i]->vecIB[j]->Fmt, 0);
                    //CONTEXT->DrawIndexed(m_vecMeshContainer[i]->vecIB[j]->Count, 0, 0);
                    CONTEXT->DrawIndexedInstanced(m_vecMeshContainer[i]->vecIB[j]->Count,
                        Count, 0, 0, 0);
                }
            }
        }

        else
        {
            CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
            //CONTEXT->Draw(m_vecMeshContainer[i]->VB.Count, 0);
            CONTEXT->DrawInstanced(m_vecMeshContainer[i]->VB.Count, Count, 0, 0);
        }
    }
}

void CMesh::RenderInstancing(int MaterialSlotIndex, unsigned int Count)
{
    MeshSlot* pSlot = m_vecMeshSlot[MaterialSlotIndex];

    unsigned int    Stride = pSlot->VB.Size;
    unsigned int    Offset = 0;

    CONTEXT->IASetPrimitiveTopology(pSlot->Primitive);
    CONTEXT->IASetVertexBuffers(0, 1, &pSlot->VB.pBuffer, &Stride, &Offset);

    if (pSlot->IB)
    {
        CONTEXT->IASetIndexBuffer(pSlot->IB->pBuffer,
            pSlot->IB->Fmt, 0);
        //CONTEXT->DrawIndexed(pSlot->IB->Count, 0, 0);
        CONTEXT->DrawIndexedInstanced(pSlot->IB->Count,
            Count, 0, 0, 0);
    }

    else
    {
        CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
        //CONTEXT->Draw(pSlot->VB.Count, 0);
        CONTEXT->DrawInstanced(pSlot->VB.Count, Count, 0, 0);
    }
}
