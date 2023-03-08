//#include "pch.h"
#include "SceneObjectWindow.h"
#include "IMGUIButton.h"
#include "IMGUIConsole.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"
#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/Camera.h"
#include "Component/SpringArm.h"
#include "Component/SpringArm2D.h"
#include "ObjectDetailWindow.h"
#include "SpriteDetailWindow.h"
#include "IMGUIManager.h"

CSceneObjectWindow::CSceneObjectWindow() :
	m_CreateObjectCount(0),
	m_SelectObjectIndex(-1),
	m_SelectComponentIndex(-1),
	m_CreateComponentIndex(-1)
{
}

CSceneObjectWindow::~CSceneObjectWindow()
{
}

bool CSceneObjectWindow::Init()
{
	m_ObjListBox = AddWidget<CIMGUIListBox>("ListBox", 300.f, 100.f);

	m_ObjListBox->SetSelectCallback<CSceneObjectWindow>(this, &CSceneObjectWindow::ListCallback);

	m_ComponentListBox = AddWidget<CIMGUIListBox>("ComponentListBox", 300.f, 100.f);

	m_ComponentListBox->SetSelectCallback<CSceneObjectWindow>(this, &CSceneObjectWindow::ComponentListCallback);


	CIMGUIButton* CreateObjButton = AddWidget<CIMGUIButton>("오브젝트생성");

	CreateObjButton->SetFont("DefaultFont");

	CreateObjButton->SetClickCallback<CSceneObjectWindow>(this, &CSceneObjectWindow::CreateObjectButtonClick);

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

	CIMGUIComboBox* ComponentCombo = AddWidget<CIMGUIComboBox>("ComponentCombo", 300.f, 100.f);

	ComponentCombo->SetSelectCallback<CSceneObjectWindow>(this, &CSceneObjectWindow::ComponentComboCallback);

	ComponentCombo->AddItem("Scene");
	ComponentCombo->AddItem("Sprite");
	ComponentCombo->AddItem("Circle");
	ComponentCombo->AddItem("Box2D");
	ComponentCombo->AddItem("Circle");
	ComponentCombo->AddItem("Pixel");
	ComponentCombo->AddItem("Camera");
	ComponentCombo->AddItem("SpringArm");
	ComponentCombo->AddItem("SpringArm2D");

	return true;
}

void CSceneObjectWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}


void CSceneObjectWindow::AddItem(const char* Item)
{
	m_ObjListBox->AddItem(Item);
}

void CSceneObjectWindow::DeleteItem(const char* Item)
{
	m_ObjListBox->DeleteItem(Item);
}

void CSceneObjectWindow::ListCallback(int SelectIndex, const char* Item)
{
	m_SelectObjectIndex = SelectIndex;

	std::string SelectName = Item;

	CScene* Scene = CSceneManager::GetInst()->GetScene();

	m_SelectObject = Scene->FindObject(SelectName);

	m_SelectComponent = nullptr;
	m_SelectComponentIndex = -1;

	// 가지고 있는 컴포넌트의 이름을 얻어온다.
	std::vector<std::string>	vecName;

	m_SelectObject->GetComponentName(vecName);

	m_ComponentListBox->Clear();

	size_t	Size = vecName.size();

	for (size_t i = 0; i < vecName.size(); ++i)
	{
		m_ComponentListBox->AddItem(vecName[i].c_str());
	}
}

void CSceneObjectWindow::ComponentListCallback(int SelectIndex, const char* Item)
{
	m_SelectComponentIndex = SelectIndex;

	m_SelectComponent = m_SelectObject->FindSceneComponent(Item);

	//if (Cast<CSpriteComponent*>(m_SelectComponent.Get()))
	//{
	//	CIMGUIManager::GetInst()->OnDetailWindow("SpriteDetailWindow");
	//}
	//else
	//	CIMGUIManager::GetInst()->OnDetailWindow("DetailWindow");

	////if (DetailWindow)
	////	DetailWindow->SetPosition(Pos);
}

void CSceneObjectWindow::ComponentComboCallback(int SelectIndex, const char* Item)
{
	if (!m_SelectObject || !m_SelectComponent)
		return;

	m_CreateComponentIndex = SelectIndex;

	// 이름을 지정하기 위한 Popup 창을 열어준다.
	m_NameInput = AddPopupWidget<CIMGUITextInput>("NameInput");

	CIMGUIButton* NameButton = AddPopupWidget<CIMGUIButton>("NameButton");

	NameButton->SetClickCallback<CSceneObjectWindow>(this, &CSceneObjectWindow::InputNamePopupButton);

	SetPopupTitle("Component Name");
	EnableModalPopup();
}

void CSceneObjectWindow::CreateObjectButtonClick()
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	char	ObjName[256] = {};

	sprintf_s(ObjName, "GameObject_%d", m_CreateObjectCount);

	++m_CreateObjectCount;

	CGameObject* Obj = Scene->SpawnObject<CGameObject>(ObjName);

	m_ObjListBox->AddItem(ObjName);
}

void CSceneObjectWindow::InputNamePopupButton()
{
	if (!m_SelectObject)
		return;

	// 이름을 얻어온다.
	const char* Name = m_NameInput->GetTextMultibyte();

	CSceneComponent* NewComponent = nullptr;

	switch ((Component_Class_Type)m_CreateComponentIndex)
	{
	case Component_Class_Type::Scene:
		NewComponent = m_SelectObject->CreateSceneComponent<CSceneComponent>(Name);
		break;
	case Component_Class_Type::Sprite:
		NewComponent = m_SelectObject->CreateSceneComponent<CSpriteComponent>(Name);
		break;
	case Component_Class_Type::Box2D:
		NewComponent = m_SelectObject->CreateSceneComponent<CColliderBox2D>(Name);
		break;
	case Component_Class_Type::Circle:
		NewComponent = m_SelectObject->CreateSceneComponent<CColliderCircle>(Name);
		break;
	case Component_Class_Type::Pixel:
		NewComponent = m_SelectObject->CreateSceneComponent<CColliderPixel>(Name);
		break;
	case Component_Class_Type::Camera:
		NewComponent = m_SelectObject->CreateSceneComponent<CCamera>(Name);
		break;
	case Component_Class_Type::SpringArm:
		NewComponent = m_SelectObject->CreateSceneComponent<CSpringArm>(Name);
		break;
	case Component_Class_Type::SpringArm2D:
		NewComponent = m_SelectObject->CreateSceneComponent<CSpringArm2D>(Name);
		break;
	}

	m_ComponentListBox->AddItem(Name);

	m_SelectComponent->AddChild(NewComponent);

	NewComponent->SetRelativePos(10.f, 20.f, 30.f);

	ClosePopup();
}

void CSceneObjectWindow::SetPosX(float x)
{
	if (!m_SelectObject)
		return;

	m_SelectObject->SetWorldPosX(x);
}

void CSceneObjectWindow::SetPosY(float y)
{
	if (!m_SelectComponent)
		return;

	m_SelectComponent->SetWorldPosY(y);
}

void CSceneObjectWindow::SetPosZ(float z)
{
	if (!m_SelectComponent)
		return;

	m_SelectComponent->SetWorldPosZ(z);
}

void CSceneObjectWindow::SetRotX(float x)
{
	if (!m_SelectComponent)
		return;

	Vector3	Rot = m_SelectComponent->GetWorldRotation();
	Rot.x = x;

	m_SelectComponent->SetWorldRotation(Rot);
}

void CSceneObjectWindow::SetRotY(float y)
{
	if (!m_SelectComponent)
		return;

	Vector3	Rot = m_SelectComponent->GetWorldRotation();
	Rot.y = y;

	m_SelectComponent->SetWorldRotation(Rot);
}

void CSceneObjectWindow::SetRotZ(float z)
{
	if (!m_SelectComponent)
		return;

	Vector3	Rot = m_SelectComponent->GetWorldRotation();
	Rot.z = z;

	m_SelectComponent->SetWorldRotation(Rot);
}

void CSceneObjectWindow::SetScaleX(float x)
{
	if (!m_SelectComponent)
		return;

	Vector3	Scale = m_SelectComponent->GetWorldScale();
	Scale.x = x;

	m_SelectComponent->SetWorldScale(Scale);
}

void CSceneObjectWindow::SetScaleY(float y)
{
	if (!m_SelectComponent)
		return;

	Vector3	Scale = m_SelectComponent->GetWorldScale();
	Scale.y = y;

	m_SelectComponent->SetWorldScale(Scale);
}

void CSceneObjectWindow::SetScaleZ(float z)
{
	if (!m_SelectComponent)
		return;

	Vector3	Scale = m_SelectComponent->GetWorldScale();
	Scale.z = z;

	m_SelectComponent->SetWorldScale(Scale);
}

void CSceneObjectWindow::SetPivotX(float x)
{
    if (!m_SelectComponent)
        return;

    Vector3	Pivot = m_SelectComponent->GetPivot();
    Pivot.x = x;

    m_SelectComponent->SetPivot(Pivot);
}

void CSceneObjectWindow::SetPivotY(float y)
{
    if (!m_SelectComponent)
        return;

    Vector3	Pivot = m_SelectComponent->GetPivot();
    Pivot.y = y;

    m_SelectComponent->SetPivot(Pivot);
}

void CSceneObjectWindow::SetPivotZ(float z)
{
    if (!m_SelectComponent)
        return;

    Vector3	Pivot = m_SelectComponent->GetPivot();
    Pivot.z = z;

    m_SelectComponent->SetPivot(Pivot);
}

void CSceneObjectWindow::SetMoveX(float x)
{
	if (!m_SelectObject)
		return;

	Vector3	Move = m_SelectObject->GetMove();
	Move.x = x;

	m_SelectObject->SetMove(Move);
}

void CSceneObjectWindow::SetMoveY(float y)
{
	if (!m_SelectObject)
		return;

	Vector3	Move = m_SelectObject->GetMove();
	Move.y = y;

	m_SelectObject->SetMove(Move);
}

//void CSceneObjectWindow::SaveButton()
//{
//	static TCHAR Filter[] = TEXT("텍스트 파일(*.txt)|*.txt|모든파일(*.*)|*.*||");
//	CFileDialog	dlg(FALSE, TEXT("*.txt"), TEXT(""), OFN_OVERWRITEPROMPT,
//		Filter);
//
//	if (dlg.DoModal() == IDOK)
//	{
//		CString	Path = dlg.GetPathName();
//
//		AfxMessageBox(Path);
//	}
//}
//
//void CSceneObjectWindow::LoadButton()
//{
//	static TCHAR Filter[] = TEXT("텍스트 파일(*.txt)|*.txt|모든파일(*.*)|*.*||");
//	CFileDialog	dlg(TRUE, TEXT("*.txt"), TEXT(""), OFN_HIDEREADONLY,
//		Filter);
//
//	if (dlg.DoModal() == IDOK)
//	{
//		CString	Path = dlg.GetPathName();
//
//		AfxMessageBox(Path);
//	}
//}
