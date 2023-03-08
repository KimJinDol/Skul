#pragma once

#include "PrimitiveComponent.h"
#include "../Resource/SpriteMesh.h"
#include "../Resource/TileMapCBuffer.h"
#include "Tile.h"
#include "../GameObject.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderLine.h"

class CTileMapComponent :
	public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CTileMapComponent();
	CTileMapComponent(const CTileMapComponent& com);
	virtual ~CTileMapComponent();

protected:
	CSharedPtr<CSpriteMesh>     m_Mesh;
	Tile_Shape  m_Shape;
	int         m_CountX;	// ���� Ÿ�� ����
	int         m_CountY;	// ���� Ÿ�� ����
	Vector2     m_TileSize;	// Ÿ�� �ϳ��� ���� ���� ũ��
	Vector2     m_TileImageSize;	// Ÿ�� �ؽ�ó �̹����� ��ü ũ��
	Vector2		m_TileImageFrameSize;	// Ÿ�� �ؽ�ó �̹������� Ÿ�� �� ĭ�� ũ��
	int			m_FrameMaxX;	// Ÿ�� �̹��� �ؽ�ó�� Ÿ�� ���� ���� (��) / �ش� �̹����� Ÿ���� ��ĭ�� �����ϴ��� 
	int			m_FrameMaxY;	// Ÿ�� �̹��� �ؽ�ó�� Ÿ�� ���� ���� (��)
	std::vector<CTile*>   m_vecTile;
	std::vector<TileInfo>	m_vecTileInfo;	// Ÿ�� ������ŭ Tile Info�� �ʿ���. �� TileInfo�� ������ ����ȭ ���۸� ������Ʈ ���ش�.
	class CStructuredBuffer* m_WorldBuffer;
	CTileMapCBuffer* m_CBuffer;
	int		m_RenderCount;
	bool	m_EditorMode;

	std::list<CGameObject*>	m_listColliderBox2DObj;
	std::list<CGameObject*>	m_listColliderLineObj;

public:
	void DeleteAllBox2DColliders();
	void DeleteAllLineColliders();

	void DeleteColliderBox2D(class CGameObject* Box2DObj);
	void DeleteColliderLine(class CGameObject* LineObj);

	void PushColliderBox2D(class CGameObject* Box2DObj);
	void PushColliderLine(class CGameObject* LineObj);

	std::list<CGameObject*> GetAllBox2DColliders()	const
	{
		return m_listColliderBox2DObj;
	}

	std::list<CGameObject*> GetAllLineColliders()	const
	{
		return m_listColliderLineObj;
	}

public:
	void SetFrameMax(int x, int y);
	void SetTileImageFrameSize(int x, int y);
	void SetTileDefaultFrame(int x, int y);
	void SetTileFrame(int TileIndex, int x, int y);
	void SetTileFrame(const Vector2& Pos, int x, int y);
	void TileRemoveRender(const Vector3& Pos);
	void TileRemoveRender(int TileIndexX, int TileIndexY);
	void TileRemoveRender(int TileIndex);
	void SetTileType(const Vector3& Pos, Tile_Type Type);
	void SetTileType(int TileIndexX, int TileIndexY, Tile_Type Type);
	void SetTileType(int TileIndex, Tile_Type Type);
	void SetEditorMode(bool Mode)
	{
		m_EditorMode = Mode;
	}

public:
	virtual class CMesh* GetMesh()  const;
	virtual void SetMesh(class CMesh* pMesh);
	virtual void SetMesh(const std::string& Name);
	virtual void SetMaterial(int SlotIndex, CMaterial* pMaterial);
	virtual void SetMaterial(int SlotIndex, const std::string& Name);
	virtual void AddMaterial(CMaterial* pMaterial);
	virtual void AddMaterial(const std::string& Name);

public:
	Vector2 GetTileMapSize()	const;

	const Vector2& GetTileSize()	const
	{
		return m_TileSize;
	}

	int GetTileCount()	const
	{
		return m_CountX * m_CountY;
	}

	int GetTileCountX()	const
	{
		return m_CountX;
	}

	int GetTileCountY()	const
	{
		return m_CountY;
	}

	int GetImageFrameMaxX()	const
	{
		return m_FrameMaxX;
	}

	int GetImageFrameMaxY()	const
	{
		return m_FrameMaxY;
	}

	Vector2 GetTileImageFrameSize()	const
	{
		return m_TileImageFrameSize;
	}

	int GetTileIndexX(const Vector3& Pos);
	int GetTileIndexY(const Vector3& Pos);
	int GetTileIndex(const Vector3& Pos);
	int GetTileUpdateIndexX(const Vector3& Pos);
	int GetTileUpdateIndexY(const Vector3& Pos);
	CTile* GetTile(const Vector3& Pos);
	CTile* GetTile(const Vector2& Pos);
	CTile* GetTile(int x, int y);
	CTile* GetTile(int Index);
	void UpdateInfo(const Vector2& TileImageFrameSize = Vector2(64, 64));

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CTileMapComponent* Clone();

public:
	void Save(const TCHAR* FileName, const std::string& PathName = MAP_PATH);
	void SaveFullPath(const TCHAR* FullPath);
	void Load(const TCHAR* FileName, const std::string& PathName = MAP_PATH);
	void LoadFullPath(const TCHAR* FullPath);

private:
	void SetWorldInfo();

public:
	template <typename T>
	void CreateTile(Tile_Shape Shape, int CountX, int CountY,
		const Vector2& TileSize)
	{
		bool NewTileMap = m_vecTile.size() == 0 ? true : false;

		m_Shape = Shape;
		m_TileSize = TileSize;

		if (NewTileMap)
		{
			m_CountX = CountX;
			m_CountY = CountY;

			m_vecTile.resize(m_CountX * m_CountY);
			m_vecTileInfo.resize(m_CountX * m_CountY);

			for (int i = 0; i < m_CountY; ++i)
			{
				for (int j = 0; j < m_CountX; ++j)
				{
					T* Tile = new T;

					Tile->m_Owner = this;

					m_vecTile[i * m_CountX + j] = Tile;

					m_vecTileInfo[i * m_CountX + j].Color = Vector4(1.f, 1.f, 1.f, 1.f); // ���� ������� �ʱ�ȭ.
					m_vecTileInfo[i * m_CountX + j].Opacity = 1.f;
				}
			}
		}
		else
		{
			int TempTileCountX = CountX;
			int TempTileCountY = CountY;
			int TempTileCount = TempTileCountX * TempTileCountY;

			std::vector<CTile*>	vecTempTile;
			vecTempTile.resize(TempTileCount);
			std::vector<TileInfo>	vecTempTileInfo;
			vecTempTileInfo.resize(TempTileCount);

			// ���� �ִ� �ʺ��� ���� ���� ���� ���̶�� ���� ����
			if (TempTileCountX < m_CountX || TempTileCountY < m_CountY)
			{
				for (int i = 0; i < m_CountY; ++i)
				{
					for (int j = 0; j < m_CountX; ++j)
					{
						int Index = i * m_CountX + j;

						if (j >= TempTileCountX || i >= TempTileCountY)
						{
							SAFE_DELETE(m_vecTile[Index]);
							/*auto iter = m_vecTileInfo.find
							m_vecTileInfo.erase()*/
						}
						else
						{
							int index = i * TempTileCountX + j;
							vecTempTile[index] = m_vecTile[Index];
							vecTempTileInfo[index] = m_vecTileInfo[Index];
						}
					}
				}
			}
			// ���� �ִ� �ʺ��� ���ų� ū ���� ����Ŷ�� ���� ����
			else
			{
				for (int i = 0; i < TempTileCountY; ++i)
				{
					for (int j = 0; j < TempTileCountX; ++j)
					{
						int Index = i * TempTileCountX + j;

						if (j >= m_CountX || i >= m_CountY)
						{
							//m_vecTileInfo[i * m_CountX + j].Color = Vector4(1.f, 1.f, 1.f, 1.f); // ���� ������� �ʱ�ȭ.
							//m_vecTileInfo[i * m_CountX + j].Opacity = 1.f;

							T* pTile = new T;

							pTile->m_Owner = this;
							vecTempTileInfo[Index].Color = Vector4(1.f, 1.f, 1.f, 1.f);
							vecTempTileInfo[Index].Opacity = 1.f;

							vecTempTile[Index] = pTile;
						}
						else
						{
							int index = i * m_CountX + j;
							vecTempTile[Index] = m_vecTile[index];
							vecTempTileInfo[Index] = m_vecTileInfo[index];
						}
					}
				}
			}

			//auto iter = m_vecTile.begin();
			//auto iterEnd = m_vecTile.end();

			//for (; iter != iterEnd; iter++)
			//	SAFE_DELETE(*iter);

			m_CountX = TempTileCountX;
			m_CountY = TempTileCountY;

			m_vecTile.resize(m_CountX * m_CountY);
			m_vecTileInfo.resize(m_CountX * m_CountY);

			for (int i = 0; i < m_CountY; ++i)
			{
				for (int j = 0; j < m_CountX; ++j)
				{
					int Index = i * m_CountX + j;
					m_vecTile[Index] = vecTempTile[Index];
					m_vecTileInfo[Index] = vecTempTileInfo[Index];
				}
			}
		}


		switch (m_Shape)
		{
		case Tile_Shape::Rect:
		{
			Vector2 Pos;

			for (int i = 0; i < m_CountY; ++i)
			{
				Pos.x = 0.f;
				Pos.y = i * m_TileSize.y;
				for (int j = 0; j < m_CountX; ++j)
				{
					Pos.x = j * m_TileSize.x;

					int Index = i * m_CountX + j;

					m_vecTile[Index]->SetPos(Pos);
					m_vecTile[Index]->SetSize(m_TileSize);
					m_vecTile[Index]->SetShape(m_Shape);
					m_vecTile[Index]->SetIndex(j, i, Index);

					if (!m_vecTile[Index]->Init())
						return;
				}
			}
		}
		break;
		case Tile_Shape::Rhombus:
		{
			Vector2 Pos;

			for (int i = 0; i < m_CountY; ++i)
			{	// Ÿ�� �ε����� ¦�� �� ��
				if (i % 2 == 0)
					Pos.x = 0.f; // 0���� ����
				// Ȧ �� �� ��
				else
					Pos.x = m_TileSize.x * 0.5f;	// 0.5 ĭ�������� ����.

				Pos.y = i * m_TileSize.y * 0.5f;

				for (int j = 0; j < m_CountX; ++j)
				{
					if (j > 0.f)
						Pos.x += m_TileSize.x;

					int Index = i * m_CountX + j;

					m_vecTile[Index]->SetPos(Pos);
					m_vecTile[Index]->SetSize(m_TileSize);
					m_vecTile[Index]->SetShape(m_Shape);
					m_vecTile[Index]->SetIndex(j, i, Index);

					if (!m_vecTile[Index]->Init())
						return;
				}
			}
		}
		break;
		}

		SetWorldInfo();
	}
};

