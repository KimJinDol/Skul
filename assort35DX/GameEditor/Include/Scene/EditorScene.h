#pragma once
#include "Scene\SceneMode.h"
#include "../Editor.h"
class CEditorScene :
    public CSceneMode
{
    friend class CScene;

protected:
    CEditorScene();
    virtual ~CEditorScene();

private:
    class CTileMapWindow* m_TileMapWindow;
    class CTileSelectWindow* m_TileSelectWindow;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);

private:
    void MoveUp(float DeltaTime);
    void MoveDown(float DeltaTime);
    void MoveLeft(float DeltaTime);
    void MoveRight(float DeltaTime);
    void MouseLButton(float DeltaTime);
    void MouseLButtonUp(float DeltaTime);

private:
    void EditTileType();
    void EditTileImage();
    void CreateTileCollision();
};

