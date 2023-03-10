
#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "Camera.h"
#include "../Resource/StructuredBuffer.h"
#include "../Resource/Material.h"
#include "Transform.h"
#include "../PathManager.h"
#include "../Resource/ResourceManager.h"
#include "../NavigationManager.h"
#include "../Navigation2D.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderLine.h"
#include "../Scene/SceneMode.h"

CTileMapComponent::CTileMapComponent()	:
	m_WorldBuffer(nullptr),
	m_CBuffer(nullptr),
	m_FrameMaxX(1),
	m_FrameMaxY(1),
	m_RenderCount(0),
	m_EditorMode(true)
{
	m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
	m_2DType = RT2D_MAP;
	m_3DType = RT3D_Default;
	
	m_listColliderBox2DObj.clear();
	m_listColliderLineObj.clear();
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& com)	:
	CPrimitiveComponent(com)
{
}

CTileMapComponent::~CTileMapComponent()
{
	//CNavigation2D* Navigation = (CNavigation2D*)CNavigationManager::GetInst()->GetNavigation();

	//Navigation->DeleteTileMapData(this);

	SAFE_DELETE(m_WorldBuffer);
	SAFE_DELETE(m_CBuffer);

	auto	iter = m_vecTile.begin();
	auto	iterEnd = m_vecTile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_vecTile.clear();
}

CMesh* CTileMapComponent::GetMesh() const
{
	return (CMesh*)m_Mesh.Get();
}

void CTileMapComponent::SetMesh(CMesh* pMesh)
{
	m_Mesh = (CSpriteMesh*)pMesh;

	Vector3 Min = m_Mesh->GetMin();
	Vector3 Max = m_Mesh->GetMax();

	m_pTransform->SetMeshSize(Max - Min);

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* pMaterialSlots =
		m_Mesh->GetMaterialSlots();

	auto    iter = pMaterialSlots->begin();
	auto    iterEnd = pMaterialSlots->end();

	for (; iter != iterEnd; ++iter)
	{
		CMaterial* pClone = (*iter)->Clone();

		pClone->SetScene(m_pScene);
		m_vecMaterialSlot.push_back(pClone);

		SAFE_RELEASE(pClone);
	}
}

void CTileMapComponent::SetMesh(const std::string& Name)
{
	m_Mesh = (CSpriteMesh*)m_pScene->GetResource()->FindMesh(Name);	// ?????? ???????? ?????????? ?????? ?????? ??

	Vector3 Min = m_Mesh->GetMin();	// ?????? ???? ?????? ???? ???? ?????? ??????.
	Vector3 Max = m_Mesh->GetMax();

	m_pTransform->SetMeshSize(Max - Min);	// Min, Max?? ???????? ?????? ???????? ????????.

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* pMaterialSlots =	// ?????? ???????????? ????????.
		m_Mesh->GetMaterialSlots();

	auto    iter = pMaterialSlots->begin();
	auto    iterEnd = pMaterialSlots->end();

	for (; iter != iterEnd; ++iter)
	{
		CMaterial* pClone = (*iter)->Clone();

		pClone->SetScene(m_pScene);	// Scene?? ???? ??????????.
		m_vecMaterialSlot.push_back(pClone);

		SAFE_RELEASE(pClone);
	}
}

void CTileMapComponent::DeleteAllBox2DColliders()
{
	m_listColliderBox2DObj.clear();
}

void CTileMapComponent::DeleteAllLineColliders()
{
	m_listColliderLineObj.clear();
}

void CTileMapComponent::DeleteColliderBox2D(CGameObject* Box2DObj)
{
	auto iter = m_listColliderBox2DObj.begin();
	auto iterEnd = m_listColliderBox2DObj.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Box2DObj)
		{
			(*iter)->Active(false);
			m_listColliderBox2DObj.erase(iter);

			return;
		}
	}
}

void CTileMapComponent::DeleteColliderLine(CGameObject* LineObj)
{
	auto iter = m_listColliderLineObj.begin();
	auto iterEnd = m_listColliderLineObj.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == LineObj)
		{
			(*iter)->Active(false);
			m_listColliderLineObj.erase(iter);

			return;
		}
	}
}

void CTileMapComponent::PushColliderBox2D(CGameObject* Box2DObj)
{
	m_listColliderBox2DObj.push_back(Box2DObj);
}

void CTileMapComponent::PushColliderLine(CGameObject* LineObj)
{
	m_listColliderLineObj.push_back(LineObj);
}

void CTileMapComponent::SetFrameMax(int x, int y)
{
	m_FrameMaxX = x;
	m_FrameMaxY = y;
	// ???? ?????? ???????? ???? ???? ?????? ???? ?????? ????????, ???? ?????? ???? ?? ???? ?????? ????????.
	//m_TileImageFrameSize.x = m_TileImageSize.x / m_FrameMaxX;
	//m_TileImageFrameSize.y = m_TileImageSize.y / m_FrameMaxY;
}

void CTileMapComponent::SetTileImageFrameSize(int x, int y)
{
	m_TileImageFrameSize.x = x;
	m_TileImageFrameSize.y = y;
}

void CTileMapComponent::SetTileDefaultFrame(int x, int y)
{
	auto	iter = m_vecTile.begin();
	auto	iterEnd = m_vecTile.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetFrameStart(m_TileImageFrameSize * Vector2((float)x, (float)y));
		(*iter)->SetFrameEnd(m_TileImageFrameSize * Vector2((float)(x + 1), (float)(y + 1)));
		(*iter)->SetFrame(x, y);
	}
}

void CTileMapComponent::SetTileFrame(int TileIndex, int x, int y)
{
	m_vecTile[TileIndex]->SetFrameStart(m_TileImageFrameSize * Vector2((float)x, (float)y));
	m_vecTile[TileIndex]->SetFrameEnd(m_TileImageFrameSize * Vector2((float)(x + 1), (float)(y + 1)));
}

void CTileMapComponent::SetTileFrame(const Vector2& Pos, int x, int y)
{
	int	TileIndex = GetTileIndex(Vector3(Pos.x, Pos.y, 0.f));

	if (TileIndex == -1)
		return;
	
	SetTileFrame(TileIndex, x, y);

}

void CTileMapComponent::TileRemoveRender(const Vector3& Pos)
{
	int	Index = GetTileIndex(Pos);

	if (Index == -1)
		return;

	m_vecTile[Index]->SetFrameStart(0.f, 0.f);
	m_vecTile[Index]->SetFrameEnd(0.f, 0.f);
}

void CTileMapComponent::TileRemoveRender(int TileIndexX, int TileIndexY)
{
	int	Index = TileIndexY * m_CountX + TileIndexX;

	m_vecTile[Index]->SetFrameStart(0.f, 0.f);
	m_vecTile[Index]->SetFrameEnd(0.f, 0.f);
}

void CTileMapComponent::TileRemoveRender(int TileIndex)
{
	m_vecTile[TileIndex]->SetFrameStart(0.f, 0.f);
	m_vecTile[TileIndex]->SetFrameEnd(0.f, 0.f);
}

void CTileMapComponent::SetTileType(const Vector3& Pos, Tile_Type Type)
{
	int	Index = GetTileIndex(Pos);

	if (Index == -1)
		return;

	m_vecTile[Index]->SetTileType(Type);

}

void CTileMapComponent::SetTileType(int TileIndexX, int TileIndexY, Tile_Type Type)
{
	int	Index = TileIndexY * m_CountX + TileIndexX;

	m_vecTile[Index]->SetTileType(Type);
}

void CTileMapComponent::SetTileType(int TileIndex, Tile_Type Type)
{
	m_vecTile[TileIndex]->SetTileType(Type);
}

void CTileMapComponent::SetMaterial(int SlotIndex, CMaterial* pMaterial)
{
	CPrimitiveComponent::SetMaterial(SlotIndex, pMaterial);

	const MaterialTextureInfo* pInfo = pMaterial->GetMaterialTextureInfo();

	if (pInfo)
	{
		m_TileImageSize.x = (float)pInfo->pTexture->GetWidth();
		m_TileImageSize.y = (float)pInfo->pTexture->GetHeight();

		m_CBuffer->SetTileImageSize(m_TileImageSize);

		//m_TileImageFrameSize.x = m_TileImageSize.x / m_FrameMaxX;
		//m_TileImageFrameSize.y = m_TileImageSize.y / m_FrameMaxY;
	}
}

void CTileMapComponent::SetMaterial(int SlotIndex, const std::string& Name)
{
	CPrimitiveComponent::SetMaterial(SlotIndex, Name);

	const MaterialTextureInfo* pInfo = m_vecMaterialSlot[SlotIndex]->GetMaterialTextureInfo();

	if (pInfo)
	{
		m_TileImageSize.x = (float)pInfo->pTexture->GetWidth();
		m_TileImageSize.y = (float)pInfo->pTexture->GetHeight();

		m_CBuffer->SetTileImageSize(m_TileImageSize);

		//m_TileImageFrameSize.x = m_TileImageSize.x / m_FrameMaxX;
		//m_TileImageFrameSize.y = m_TileImageSize.y / m_FrameMaxY;
	}
}

void CTileMapComponent::AddMaterial(CMaterial* pMaterial)
{
	CPrimitiveComponent::AddMaterial(pMaterial);

	const MaterialTextureInfo* pInfo = pMaterial->GetMaterialTextureInfo();

	if (pInfo)
	{
		m_TileImageSize.x = (float)pInfo->pTexture->GetWidth();
		m_TileImageSize.y = (float)pInfo->pTexture->GetHeight();

		m_CBuffer->SetTileImageSize(m_TileImageSize);

		//m_TileImageFrameSize.x = m_TileImageSize.x / m_FrameMaxX;
		//m_TileImageFrameSize.y = m_TileImageSize.y / m_FrameMaxY;
	}
}

void CTileMapComponent::AddMaterial(const std::string& Name)
{
	CPrimitiveComponent::AddMaterial(Name);

	const MaterialTextureInfo* pInfo = m_vecMaterialSlot[0]->GetMaterialTextureInfo();

	if (pInfo)
	{
		m_TileImageSize.x = (float)pInfo->pTexture->GetWidth();
		m_TileImageSize.y = (float)pInfo->pTexture->GetHeight();

		m_CBuffer->SetTileImageSize(m_TileImageSize);

		//m_TileImageFrameSize.x = m_TileImageSize.x / m_FrameMaxX;
		//m_TileImageFrameSize.y = m_TileImageSize.y / m_FrameMaxY;
	}
}

Vector2 CTileMapComponent::GetTileMapSize() const
{
	return Vector2(m_CountX * m_TileSize.x, m_CountY * m_TileSize.y);
}

int CTileMapComponent::GetTileIndexX(const Vector3& Pos)
{
	if (m_Shape == Tile_Shape::Rect)
	{
		float	ConvertX = Pos.x - GetWorldPos().x; // ?????? ???????? ?????? ???? ???? (0, 0)???? ???????? ??????.

		int	IndexX = (int)(ConvertX / m_TileSize.x);

		if (IndexX < 0 || IndexX >= m_CountX)
			return -1;

		return IndexX;
	}

	int	IndexY = GetTileIndexY(Pos);

	if (IndexY < 0 || IndexY >= m_CountY)
		return -1;

	int	IndexX = -1;

	float	ConvertX = Pos.x - GetWorldPos().x;

	if (IndexY % 2 == 0)
		IndexX = (int)(ConvertX / m_TileSize.x);

	else
		IndexX = (int)((ConvertX - m_TileSize.x * 0.5f) / m_TileSize.x);

	if (IndexX < 0 || IndexX >= m_CountX)
		return -1;

	return IndexX;
}

int CTileMapComponent::GetTileIndexY(const Vector3& Pos)
{
	Vector3	TileMapPos = GetWorldPos();

	if (m_Shape == Tile_Shape::Rect)
	{
		float	ConvertY = Pos.y - TileMapPos.y;

		int	IndexY = (int)(ConvertY / m_TileSize.y);

		if (IndexY < 0 || IndexY >= m_CountY)
			return -1;

		return IndexY;
	}

	Vector2	ConvertPos = Vector2(Pos.x, Pos.y) - Vector2(TileMapPos.x, TileMapPos.y);

	float	RatioX = ConvertPos.x / m_TileSize.x;
	float	RatioY = ConvertPos.y / m_TileSize.y;

	// ???? ?????? ?????? ???? ???????????? ???????? ??????.
	int	IndexX = (int)RatioX;
	int	IndexY = (int)RatioY;

	if (IndexX < 0 || IndexX > m_CountX)
		return -1;

	// ?????????? ???????? ?????? ???????? ?????? ????????.
	RatioX -= (float)IndexX;
	RatioY -= (float)IndexY;

	// ???????? ?????? ?????? ????
	if (RatioY < 0.5f)
	{
		// ?????? ?????? ???????? RatioX ?? 0.5???? ???? ?? ???? ???????? ???? ??????
		// ?? ???? ???????? ????. ???? ???????? ???? ?????? ???? ?????? ??????????
		// ?? ???? ?????????? ?? ???? ???????? ?????? ????????.
		if (RatioY < 0.5f - RatioX)
		{
			// ?? ???? ???? ???? ???????? ?? ???? ???????? ?? ???? ???????? ?????? ????????
			// ???????????? -1?? ????????.
			if (IndexX == 0)
				return -1;

			// ?? ???? ???? ?????? ???????? ?? ???? ???????? ?? ???? ???????? ?????? ????????
			// ???????????? -1?? ????????.
			else if (IndexY == 0)
				return -1;

			else if (IndexY == 1)
				return 1;

			else
				return IndexY * 2 - 1;
		}

		// ?? ???? ???????? ?? ???? ???????? ???????? ???? ?????? ???????? ????.
		else if (RatioY < RatioX - 0.5f)
		{
			if (IndexY == 0)
				return -1;

			else if (IndexY == 1)
				return 1;

			else
				return IndexY * 2 - 1;
		}

		else
			return IndexY * 2;
	}

	// ???????? ???? ?????? ????
	else if (RatioY > 0.5f)
	{
		// ?? ???? ???????? ?? ???? ???????? ???? ???? ?????? ???????? ????.
		if (RatioY - 0.5f > RatioX)
		{
			if (IndexX == 0)
				return -1;

			int	Index = IndexY * 2 + 1;

			if (Index >= m_CountY)
				return -1;

			return Index;
		}

		// ?? ???? ???????? ?? ???? ???????? ???? ???? ?????? ???????? ????.
		else if (RatioY - 0.5f > 1.f - RatioX)
		{
			if (IndexX == m_CountX)
				return -1;

			int	Index = IndexY * 2 + 1;

			if (Index >= m_CountY)
				return -1;

			return Index;
		}

		else
			return IndexY * 2;
	}

	// ???????? ?????? ?????? ????
	else
		return IndexY * 2;

	return -1;
}

int CTileMapComponent::GetTileIndex(const Vector3& Pos)
{
	int	IndexX = GetTileIndexX(Pos);
	int	IndexY = GetTileIndexY(Pos);

	if (IndexX == -1 || IndexY == -1)
		return -1;

	return IndexY * m_CountX + IndexX;
}

int CTileMapComponent::GetTileUpdateIndexX(const Vector3& Pos)
{
	if (m_Shape == Tile_Shape::Rect)
	{
		float	ConvertX = Pos.x - GetWorldPos().x;

		int	IndexX = (int)(ConvertX / m_TileSize.x);

		if (IndexX < 0)
			return 0;

		else if (IndexX >= m_CountX)
			return m_CountX - 1;

		return IndexX;
	}

	int	IndexY = GetTileIndexY(Pos);

	if (IndexY < 0)
		IndexY = 0;

	else if (IndexY >= m_CountY)
		IndexY = m_CountY - 1;

	int	IndexX = -1;

	float	ConvertX = Pos.x - GetWorldPos().x;

	if (IndexY % 2 == 0)
		IndexX = (int)(ConvertX / m_TileSize.x);

	else
		IndexX = (int)((ConvertX - m_TileSize.x * 0.5f) / m_TileSize.x);

	if (IndexX < 0)
		return 0;

	else if (IndexX >= m_CountX)
		return m_CountX - 1;

	return IndexX;
}

int CTileMapComponent::GetTileUpdateIndexY(const Vector3& Pos)
{
	Vector3	TileMapPos = GetWorldPos();

	if (m_Shape == Tile_Shape::Rect)
	{
		float	ConvertY = Pos.y - TileMapPos.y;

		int	IndexY = (int)(ConvertY / m_TileSize.y);

		if (IndexY < 0)
			return 0;

		else if (IndexY >= m_CountY)
			return m_CountY - 1;

		return IndexY;
	}

	Vector2	ConvertPos = Vector2(Pos.x, Pos.y) - Vector2(TileMapPos.x, TileMapPos.y);

	float	RatioX = ConvertPos.x / m_TileSize.x;
	float	RatioY = ConvertPos.y / m_TileSize.y;

	// ???? ?????? ?????? ???? ???????????? ???????? ??????.
	int	IndexX = (int)RatioX;
	int	IndexY = (int)RatioY;

	if (IndexX < 0)
		IndexX = 0;

	else if (IndexX > m_CountX)
		IndexX = m_CountX - 1;

	// ?????????? ???????? ?????? ???????? ?????? ????????.
	RatioX -= (float)IndexX;
	RatioY -= (float)IndexY;

	// ???????? ?????? ?????? ????
	if (RatioY < 0.5f)
	{
		// ?????? ?????? ???????? RatioX ?? 0.5???? ???? ?? ???? ???????? ???? ??????
		// ?? ???? ???????? ????. ???? ???????? ???? ?????? ???? ?????? ??????????
		// ?? ???? ?????????? ?? ???? ???????? ?????? ????????.
		if (RatioY < 0.5f - RatioX)
		{
			// ?? ???? ???? ???? ???????? ?? ???? ???????? ?? ???? ???????? ?????? ????????
			// ???????????? -1?? ????????.
			if (IndexX == 0)
			{
				if (IndexY < 0)
					return 0;

				else if (IndexY >= m_CountY)
					return m_CountY - 1;

				else
					return IndexY * 2;
			}

			// ?? ???? ???? ?????? ???????? ?? ???? ???????? ?? ???? ???????? ?????? ????????
			// ???????????? -1?? ????????.
			else if (IndexY == 0)
			{
				return 0;
			}

			else if (IndexY == 1)
				return 1;

			else
				return IndexY * 2 - 1;
		}

		// ?? ???? ???????? ?? ???? ???????? ???????? ???? ?????? ???????? ????.
		else if (RatioY < RatioX - 0.5f)
		{
			if (IndexY == 0)
				return 0;

			else if (IndexY == 1)
				return 1;

			else
				return IndexY * 2 - 1;
		}

		else
			return IndexY * 2;
	}

	// ???????? ???? ?????? ????
	else if (RatioY > 0.5f)
	{
		// ?? ???? ???????? ?? ???? ???????? ???? ???? ?????? ???????? ????.
		if (RatioY - 0.5f > RatioX)
		{
			if (IndexX == 0)
			{
				if (IndexY < 0)
					return 0;

				else if (IndexY >= m_CountY)
					return m_CountY - 1;
			}

			int	Index = IndexY * 2 + 1;

			if (Index >= m_CountY)
				return m_CountY - 1;

			return Index;
		}

		// ?? ???? ???????? ?? ???? ???????? ???? ???? ?????? ???????? ????.
		else if (RatioY - 0.5f > 1.f - RatioX)
		{
			if (IndexX == m_CountX)
			{
				if (IndexY < 0)
					return 0;

				else if (IndexY >= m_CountY)
					return m_CountY - 1;
			}

			int	Index = IndexY * 2 + 1;

			if (Index >= m_CountY)
				return m_CountY  - 1;

			return Index;
		}

		else
			return IndexY * 2;
	}

	// ???????? ?????? ?????? ????
	else
		return IndexY * 2;

	return -1;
}

CTile* CTileMapComponent::GetTile(const Vector3& Pos)
{
	int	Index = GetTileIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTile[Index];
}

CTile* CTileMapComponent::GetTile(const Vector2& Pos)
{
	int	Index = GetTileIndex(Vector3(Pos.x, Pos.y, 0.f));

	if (Index == -1)
		return nullptr;

	return m_vecTile[Index];
}

CTile* CTileMapComponent::GetTile(int x, int y)
{
	Vector3	Pos((float)x, (float)y, 0.f);

	int	Index = GetTileIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTile[Index];
}

CTile* CTileMapComponent::GetTile(int Index)
{
	return m_vecTile[Index];
}

void CTileMapComponent::UpdateInfo(const Vector2& TileImageFrameSize)
{
	const MaterialTextureInfo* pInfo = m_vecMaterialSlot[0]->GetMaterialTextureInfo();

	if (pInfo)
	{
		m_TileImageSize.x = (float)pInfo->pTexture->GetWidth();
		m_TileImageSize.y = (float)pInfo->pTexture->GetHeight();

		m_CBuffer->SetTileImageSize(m_TileImageSize);

		m_TileImageFrameSize.x = TileImageFrameSize.x;
		m_TileImageFrameSize.y = TileImageFrameSize.y;
	}
}

void CTileMapComponent::Start()
{
	CPrimitiveComponent::Start();

	m_pScene->GetCameraManager()->GetCurrentCamera()->Set2DWorldSize(GetTileMapSize());
}

bool CTileMapComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	SetTileImageFrameSize(64, 64);

	SetMesh("TextureRect");

	// ???????? ????
	m_CBuffer = new CTileMapCBuffer;

	m_CBuffer->Init();

	CNavigation2D* Navigation = (CNavigation2D*)CNavigationManager::GetInst()->GetNavigation();

	// Navigation->DeleteTileMapData(this); 11.01 ?????? ???? ????????

	return true;
}

void CTileMapComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	/*size_t Size = m_vecTile.size();
	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTile[i]->Update(DeltaTime);
	}*/
}

void CTileMapComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);

}

void CTileMapComponent::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);
}

void CTileMapComponent::Render(float DeltaTime)
{
	CPrimitiveComponent::Render(DeltaTime);

	CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

	Resolution	RS = Camera->GetResolution();

	Vector3	LB = Camera->GetWorldPos(); // ?????? ?????? ?????? Left Bottom?? ???????? ????.
	Vector3	RT = LB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

	int	StartX, StartY, EndX, EndY;

	StartX = GetTileUpdateIndexX(LB);
	StartY = GetTileUpdateIndexY(LB);

	EndX = GetTileUpdateIndexX(RT);
	EndY = GetTileUpdateIndexY(RT);

	if (m_Shape == Tile_Shape::Rhombus)
	{
		--StartX;
		--StartY;

		++EndX;
		++EndY;
	}

	StartX = StartX < 0 ? 0 : StartX;
	StartY = StartY < 0 ? 0 : StartY;

	EndX = EndX >= m_CountX ? m_CountX - 1 : EndX;
	EndY = EndY >= m_CountY ? m_CountY - 1 : EndY;

	Matrix	matView, matProj;
	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	m_RenderCount = 0;

	for (int i = StartY; i <= EndY; ++i)
	{
		for (int j = StartX; j <= EndX; ++j)
		{
			int	Index = i * m_CountX + j;

			m_vecTile[Index]->PostUpdate(DeltaTime);

			if (m_vecTile[Index]->GetRender())
			{
				if (m_EditorMode)
				{
					switch (m_vecTile[Index]->GetTileType())
					{
					case Tile_Type::None:
						m_vecTileInfo[m_RenderCount].Color = Vector4(1.f, 1.f, 1.f, 1.f);
						break;
					case Tile_Type::Wall:
						m_vecTileInfo[m_RenderCount].Color = Vector4(1.f, 0.f, 0.f, 1.f);
						break;
					}
				}


				m_vecTileInfo[m_RenderCount].FrameStart = m_vecTile[Index]->GetFrameStart();
				m_vecTileInfo[m_RenderCount].FrameEnd = m_vecTile[Index]->GetFrameEnd();
				m_vecTileInfo[m_RenderCount].matWVP = m_vecTile[Index]->GetWorldMatrix() * matView * matProj;
				m_vecTileInfo[m_RenderCount].matWVP.Transpose();
				++m_RenderCount;
			}
		}
	}

	if (m_WorldBuffer)
		m_WorldBuffer->UpdateBuffer(&m_vecTileInfo[0], sizeof(TileInfo) * m_RenderCount);


	/*--------------------------???? PostUpdate?? ???? ????---------------------------*/


	m_WorldBuffer->SetShader(30, CBT_VERTEX); // 30?? ?????? ???? ?????????? ?????? ?????? ??????????.

	m_CBuffer->UpdateCBuffer(); // TileMapCBuffer ?? ?????? ???? ???????? ???? ???????? ????????.

	if (!m_vecMaterialSlot.empty())
		m_vecMaterialSlot[0]->SetMaterial();

	m_Mesh->RenderInstancing((unsigned int)m_RenderCount);

	m_WorldBuffer->ResetShader(30, CBT_VERTEX);

	if (!m_vecMaterialSlot.empty())
		m_vecMaterialSlot[0]->ResetMaterial();
}

CTileMapComponent* CTileMapComponent::Clone()
{
	return new CTileMapComponent(*this);
}

void CTileMapComponent::Save(const TCHAR* FileName, 
	const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->pPath);

	lstrcat(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CTileMapComponent::SaveFullPath(const TCHAR* FullPath)
{
	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, FullPath);

#endif // UNICODE

	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPathMultibyte, "wb");

	if (!pFile)
		return;

	Vector3	Pos, Rot, Scale;

	Pos = GetWorldPos();
	Rot = GetWorldRotation();
	Scale = GetWorldScale();

	fwrite(&Pos, sizeof(Vector3), 1, pFile);
	fwrite(&Rot, sizeof(Vector3), 1, pFile);
	fwrite(&Scale, sizeof(Vector3), 1, pFile);

	fwrite(&m_Shape, sizeof(Tile_Shape), 1, pFile);
	fwrite(&m_CountX, sizeof(int), 1, pFile);
	fwrite(&m_CountY, sizeof(int), 1, pFile);

	fwrite(&m_TileSize, sizeof(Vector2), 1, pFile);
	fwrite(&m_TileImageSize, sizeof(Vector2), 1, pFile);
	fwrite(&m_TileImageFrameSize, sizeof(Vector2), 1, pFile);

	fwrite(&m_FrameMaxX, sizeof(int), 1, pFile);
	fwrite(&m_FrameMaxY, sizeof(int), 1, pFile);

	// ???? ???? ????.
	m_vecMaterialSlot[0]->Save(pFile);

	// ???? ????
	size_t	Size = m_vecTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTile[i]->Save(pFile);
	}

	// ???? ???????? ???? ????
	int BoxCount = m_listColliderBox2DObj.size();
	fwrite(&BoxCount, sizeof(int), 1, pFile);

	auto iter = m_listColliderBox2DObj.begin();
	auto iterEnd = m_listColliderBox2DObj.end();

	for (int i = 0; i < BoxCount; i++)
	{
		Vector2 Pos = Vector2((*iter)->GetWorldPos().x, (*iter)->GetWorldPos().y);
		CColliderBox2D* Box2D = (CColliderBox2D*)(*iter)->FindSceneComponent("Box2D");
		Vector2 Extent = Vector2(Box2D->GetInfo().Length[0], Box2D->GetInfo().Length[1]);

		fwrite(&Pos, sizeof(Vector2), 1, pFile);
		fwrite(&Extent, sizeof(Vector2), 1, pFile);

		++iter;
	}

	// ???? ???????? ???? ????
	int LineCount = m_listColliderLineObj.size();
	fwrite(&LineCount, sizeof(int), 1, pFile);

	iter = m_listColliderLineObj.begin();
	iterEnd = m_listColliderLineObj.end();

	for (int i = 0; i < LineCount; i++)
	{
		Vector2 Pos = Vector2((*iter)->GetWorldPos().x, (*iter)->GetWorldPos().y);
		CColliderLine* Line = (CColliderLine*)(*iter)->FindSceneComponent("Line");
		LineInfo Info = Line->GetInfo();

		fwrite(&Pos, sizeof(Vector2), 1, pFile);
		fwrite(&Info, sizeof(LineInfo), 1, pFile);

		++iter;
	}


	fclose(pFile);
}

void CTileMapComponent::Load(const TCHAR* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->pPath);

	lstrcat(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CTileMapComponent::LoadFullPath(const TCHAR* FullPath)
{
	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, FullPath);

#endif // UNICODE

	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPathMultibyte, "rb");

	if (!pFile)
		return;

	Vector3	Pos, Rot, Scale;

	fread(&Pos, sizeof(Vector3), 1, pFile);
	fread(&Rot, sizeof(Vector3), 1, pFile);
	fread(&Scale, sizeof(Vector3), 1, pFile);

	SetWorldPos(Pos);
	SetWorldRotation(Rot);
	SetWorldScale(Scale);

	fread(&m_Shape, sizeof(Tile_Shape), 1, pFile);
	fread(&m_CountX, sizeof(int), 1, pFile);
	fread(&m_CountY, sizeof(int), 1, pFile);

	fread(&m_TileSize, sizeof(Vector2), 1, pFile);
	fread(&m_TileImageSize, sizeof(Vector2), 1, pFile);
	fread(&m_TileImageFrameSize, sizeof(Vector2), 1, pFile);

	fread(&m_FrameMaxX, sizeof(int), 1, pFile);
	fread(&m_FrameMaxY, sizeof(int), 1, pFile);

	// ???? ???? ????????.
	CMaterial* Material = CResourceManager::GetInst()->CreateMaterial();

	Material->Load(pFile);

	m_vecMaterialSlot[0] = Material;
	m_vecMaterialSlot[0]->Release(); // ReasourceManager?? ?????? ?????? ?????? Release ?????? ????????.. ???????? ??????

	auto	iter = m_vecTile.begin();
	auto	iterEnd = m_vecTile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_vecTile.clear();


	// ???? ????????
	int	Count = m_CountX * m_CountY;

	m_vecTile.resize(Count);
	m_vecTileInfo.resize(Count);

	for (int i = 0; i < Count; ++i)
	{
		m_vecTile[i] = new CTile;

		m_vecTile[i]->m_Owner = this;

		m_vecTile[i]->Load(pFile);

		m_vecTileInfo[i].Color = Vector4(1.f, 1.f, 1.f, 1.f);
		m_vecTileInfo[i].Opacity = 1.f;
	}

	m_CBuffer->SetTileImageSize(m_TileImageSize);

	int BoxCount = 0;
	fread(&BoxCount, sizeof(int), 1, pFile);

	for (int i = 0; i < BoxCount; i++)
	{
		Vector2 Pos;
		Vector2 Extent;
		fread(&Pos, sizeof(Vector2), 1, pFile);
		fread(&Extent, sizeof(Vector2), 1, pFile);

		CGameObject* Obj = m_pScene->SpawnObject<CGameObject>("MapColliderBox2D");
		CColliderBox2D* Box2D = Obj->CreateSceneComponent<CColliderBox2D>("Box2D");
		Obj->SetRootComponent(Box2D);
		Obj->SetWorldPos2D(Pos.x, Pos.y);
		Box2D->SetCollisionProfile("Static");
		Box2D->SetExtent(Extent.x, Extent.y);
		Box2D->SetPivot(0.f, 1.f, 0.f);

		m_listColliderBox2DObj.push_back(Obj);
	}

	// ???? ???????? ???? ????
	int LineCount = 0;
	fread(&LineCount, sizeof(int), 1, pFile);

	for (int i = 0; i < LineCount; i++)
	{
		Vector2 Pos;
		LineInfo Info;

		fread(&Pos, sizeof(Vector2), 1, pFile);
		fread(&Info, sizeof(LineInfo), 1, pFile);

		CGameObject* Obj = m_pScene->SpawnObject<CGameObject>("MapColliderLine");
		CColliderLine* Line = Obj->CreateSceneComponent<CColliderLine>("Line");
		Obj->SetRootComponent(Line);
		Obj->SetWorldPos2D(Pos.x, Pos.y);
		Line->SetCollisionProfile("StaticLine");
		Line->SetStart(Info.Start);
		Line->SetEnd(Info.End);
		Line->SetPivot(0.f, 1.f, 0.f);

		m_listColliderLineObj.push_back(Obj);
	}

	fclose(pFile);


	SetWorldInfo();
}

void CTileMapComponent::SetWorldInfo()
{
	SAFE_DELETE(m_WorldBuffer);

	m_WorldBuffer = new CStructuredBuffer;
	
	if (!m_WorldBuffer->Init("TileMapBuffer", sizeof(TileInfo),
		(unsigned int)m_vecTile.size(), 30, true, CBT_VERTEX))
	{
		SAFE_DELETE(m_WorldBuffer);
		return;
	}
}
