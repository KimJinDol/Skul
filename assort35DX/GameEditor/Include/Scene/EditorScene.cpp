
#include "pch.h"
#include "EditorScene.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"
#include "../TileMapWindow.h"
#include "IMGUIManager.h"
#include "Component/TileMapComponent.h"
#include "../TileSelectWindow.h"
#include "Device.h"

CEditorScene::CEditorScene()
{
}

CEditorScene::~CEditorScene()
{
}

bool CEditorScene::Init()
{
    CInput::GetInst()->CreateKey("MoveUp", 'W');
    CInput::GetInst()->CreateKey("MoveDown", 'S');
    CInput::GetInst()->CreateKey("MoveLeft", 'A');
    CInput::GetInst()->CreateKey("MoveRight", 'D');
    CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);

    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveUp", KT_Push, this, &CEditorScene::MoveUp);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveDown", KT_Push, this, &CEditorScene::MoveDown);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveLeft", KT_Push, this, &CEditorScene::MoveLeft);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveRight", KT_Push, this, &CEditorScene::MoveRight);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MouseLButton", KT_Push, this, &CEditorScene::MouseLButton);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MouseLButton", KT_Up, this, &CEditorScene::MouseLButtonUp);

    m_TileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");
    m_TileMapWindow->SetScene(m_pScene);
    m_TileSelectWindow = (CTileSelectWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileSelectWindow");

	return true;
}

void CEditorScene::Update(float DeltaTime)
{
    CSceneMode::Update(DeltaTime);
}

void CEditorScene::PostUpdate(float DeltaTime)
{
    CSceneMode::PostUpdate(DeltaTime);
}

void CEditorScene::MoveUp(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CEditorScene::MoveDown(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_Y) * -300.f * DeltaTime);
}

void CEditorScene::MoveLeft(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_X) * -300.f * DeltaTime);
}

void CEditorScene::MoveRight(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_X) * 300.f * DeltaTime);
}

void CEditorScene::MouseLButton(float DeltaTime)
{
    if (!m_TileMapWindow->IsTileMap())
        return;

    // Ŀ���� ȭ����� ����� return;

    CCamera* pCamera = m_pScene->GetCameraManager()->GetCurrentCamera();
    // ���� ����󿡼��� Ŀ�� ��ġ
    Vector2 CameraWorldPos2D = Vector2(pCamera->GetWorldPos().x, pCamera->GetWorldPos().y);
    Vector2	MousePos = CInput::GetInst()->GetMousePos() + CameraWorldPos2D;

    Vector2 DisplayLimitPos = Vector2(CameraWorldPos2D.x + GET_RESOLUTION.Width, CameraWorldPos2D.y + GET_RESOLUTION.Height);
    

    // Ŀ���� Ÿ�� ����â�� ����� return;
    Vector2 CursorPos = m_TileSelectWindow->GetCursorPos();
    Vector2 ImageWindowPos = m_TileSelectWindow->GetImageWindowPos();
    Vector2 TextureSize = m_TileSelectWindow->GetTextureSize();

    ImageWindowPos.y -= TextureSize.y;
    CursorPos -= ImageWindowPos;


    if (MousePos.x < pCamera->GetWorldPos().x || MousePos.x > DisplayLimitPos.x ||
        MousePos.y < pCamera->GetWorldPos().y || MousePos.y > DisplayLimitPos.y)
    {
        if (CursorPos.x < 0 || CursorPos.y < 0 || CursorPos.x > TextureSize.x || CursorPos.y > TextureSize.y)
            return;

        // Ÿ�� �̹��� ����â�� Ŀ���� �ö������� ���� ����.
        m_TileMapWindow->SelectTileImage(CursorPos);
    }

    else
    {
        Tile_Modify_Type    ModifyType = m_TileMapWindow->GetTileModifyType();

        switch (ModifyType)
        {
        case Tile_Modify_Type::Type:
            EditTileType();
            break;
        case Tile_Modify_Type::Image:
            EditTileImage();
            break;
        case Tile_Modify_Type::Collider:
            CreateTileCollision();
        }
    }
}

void CEditorScene::MouseLButtonUp(float DeltaTime)
{
    Tile_Modify_Type    ModifyType = m_TileMapWindow->GetTileModifyType();

    if (ModifyType == Tile_Modify_Type::Collider)
    {
        TileCollider_Type ColliderType = m_TileMapWindow->GetTileColliderType();

        switch (ColliderType)
        {
        case TileCollider_Type::Box2D:
            m_TileMapWindow->MouseOnColliderBox();
            break;
        case TileCollider_Type::Line:
            m_TileMapWindow->MouseOnColliderLine();
            break;
        }

    }
}

void CEditorScene::EditTileType()
{
    Tile_Type   Type = m_TileMapWindow->GetTileType();

    Vector2 MousePos = CInput::GetInst()->GetMouse2DWorldPos();

    CTileMapComponent* TileMap = m_TileMapWindow->GetTileMap();

    // ���콺 ��ġ�� �̿��ؼ� ���� ���콺�� ��ġ�� ���� Ÿ���� ���´�.
    CTile* Tile = TileMap->GetTile(MousePos);

    if (Tile)
        Tile->SetTileType(Type);
}

void CEditorScene::EditTileImage()
{
    int ImageFrameX = m_TileMapWindow->GetImageFrameX();
    int ImageFrameY = m_TileMapWindow->GetImageFrameY();

    Vector2 MousePos = CInput::GetInst()->GetMouse2DWorldPos();

    CTileMapComponent* TileMap = m_TileMapWindow->GetTileMap();

    if (m_TileMapWindow->GetTileImageType() == Tile_Image_Type::Remove)
    {
        TileMap->TileRemoveRender(Vector3(MousePos.x, MousePos.y, 0.f));

        CTile* Tile = TileMap->GetTile(MousePos);

    }

    else
    {
        TileMap->SetTileFrame(MousePos, ImageFrameX, ImageFrameY);
    }
}

void CEditorScene::CreateTileCollision()
{
    Vector2 MousePos = CInput::GetInst()->GetMouse2DWorldPos();

    TileCollider_Type ColliderType = m_TileMapWindow->GetTileColliderType();

    switch (ColliderType)
    {
    case TileCollider_Type::Box2D:
        m_TileMapWindow->CreateColliderBox(MousePos);
        break;
    case TileCollider_Type::Line:
        m_TileMapWindow->CreateColliderLine(MousePos);
        break;
    case TileCollider_Type::Delete:
        m_TileMapWindow->DeleteCollider(MousePos);
        break;
    }

}
