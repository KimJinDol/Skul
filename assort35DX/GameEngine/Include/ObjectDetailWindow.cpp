#include "ObjectDetailWindow.h"
//#include "TestWindow.h"
#include "IMGUIButton.h"
#include "IMGUIConsole.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"
#include "SceneObjectWindow.h"
#include "IMGUIManager.h"

CObjectDetailWindow::CObjectDetailWindow() :
	m_ObjectWindow(nullptr)
{
}

CObjectDetailWindow::~CObjectDetailWindow()
{
}

void CObjectDetailWindow::SetPosition(const Vector3& Pos)
{
	m_InputPosX->SetFloat(Pos.x);
	m_InputPosY->SetFloat(Pos.y);
	m_InputPosZ->SetFloat(Pos.z);
}

void CObjectDetailWindow::SetRotation(const Vector3& Rot)
{
	m_InputRotX->SetFloat(Rot.x);
	m_InputRotY->SetFloat(Rot.y);
	m_InputRotZ->SetFloat(Rot.z);
}

void CObjectDetailWindow::SetScale(const Vector3& Scale)
{
	m_InputScaleX->SetFloat(Scale.x);
	m_InputScaleY->SetFloat(Scale.y);
	m_InputScaleZ->SetFloat(Scale.z);
}

void CObjectDetailWindow::SetPivot(const Vector3& Pivot)
{
    m_InputPivotX->SetFloat(Pivot.x);
    m_InputPivotY->SetFloat(Pivot.y);
    m_InputPivotZ->SetFloat(Pivot.z);
}

void CObjectDetailWindow::SetMove(const Vector3& Move)
{
	m_InputMoveX->SetFloat(Move.x);
	m_InputMoveY->SetFloat(Move.y);
}

bool CObjectDetailWindow::Init()
{
	// 위치
	{
		CIMGUIText* Text = AddWidget<CIMGUIText>("위치");
		Text->SetFont("DefaultFont");
		Text->SetText("Pos");

		CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

		CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
		Label->SetText("X");

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputPosX = AddWidget<CIMGUITextInput>("##PosX", 50.f, 20.f);
		m_InputPosX->SetNumberFloat(true);
		m_InputPosX->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPosX);

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
		Label->SetText("Y");
		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputPosY = AddWidget<CIMGUITextInput>("##PosY", 50.f, 20.f);
		m_InputPosY->SetNumberFloat(true);
		m_InputPosY->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPosY);

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
		Label->SetText("Z");
		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputPosZ = AddWidget<CIMGUITextInput>("##PosZ", 50.f, 20.f);
		m_InputPosZ->SetNumberFloat(true);
		m_InputPosZ->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPosZ);

		//SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputPosX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
		m_InputPosY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
		m_InputPosZ->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	}

	// 회전
	{
		CIMGUIText* Text = AddWidget<CIMGUIText>("회전");
		Text->SetFont("DefaultFont");
		Text->SetText("Rotation");

		CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

		CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
		Label->SetText("X");

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputRotX = AddWidget<CIMGUITextInput>("##RotX", 50.f, 20.f);
		m_InputRotX->SetNumberFloat(true);
		m_InputRotX->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputRotX);

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
		Label->SetText("Y");
		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputRotY = AddWidget<CIMGUITextInput>("##RotY", 50.f, 20.f);
		m_InputRotY->SetNumberFloat(true);
		m_InputRotY->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputRotY);

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
		Label->SetText("Z");
		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputRotZ = AddWidget<CIMGUITextInput>("##RotZ", 50.f, 20.f);
		m_InputRotZ->SetNumberFloat(true);
		m_InputRotZ->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputRotZ);


		m_InputRotX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
		m_InputRotY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
		m_InputRotZ->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	}

	// 스케일
	{
		CIMGUIText* Text = AddWidget<CIMGUIText>("스케일");
		Text->SetFont("DefaultFont");
		Text->SetText("Scale");

		CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

		CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
		Label->SetText("X");

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputScaleX = AddWidget<CIMGUITextInput>("##ScaleX", 50.f, 20.f);
		m_InputScaleX->SetNumberFloat(true);
		m_InputScaleX->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputScaleX);

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
		Label->SetText("Y");
		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputScaleY = AddWidget<CIMGUITextInput>("##ScaleY", 50.f, 20.f);
		m_InputScaleY->SetNumberFloat(true);
		m_InputScaleY->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputScaleY);

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
		Label->SetText("Z");
		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputScaleZ = AddWidget<CIMGUITextInput>("##ScaleZ", 50.f, 20.f);
		m_InputScaleZ->SetNumberFloat(true);
		m_InputScaleZ->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputScaleZ);

		m_InputScaleX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
		m_InputScaleY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
		m_InputScaleZ->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	}

    // 피봇
    {
        CIMGUIText* Text = AddWidget<CIMGUIText>("피봇");
        Text->SetFont("DefaultFont");
		Text->SetText("Pivot");

		CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

        CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
        Label->SetText("X");

        SameLine = AddWidget<CIMGUISameLine>("SameLine");

        m_InputPivotX = AddWidget<CIMGUITextInput>("##PivotX", 50.f, 20.f);
        m_InputPivotX->SetNumberFloat(true);
        m_InputPivotX->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPivotX);

        SameLine = AddWidget<CIMGUISameLine>("SameLine");

        Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
        Label->SetText("Y");
        SameLine = AddWidget<CIMGUISameLine>("SameLine");

        m_InputPivotY = AddWidget<CIMGUITextInput>("##PivotY", 50.f, 20.f);
        m_InputPivotY->SetNumberFloat(true);
        m_InputPivotY->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPivotY);

        SameLine = AddWidget<CIMGUISameLine>("SameLine");

        Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
        Label->SetText("Z");

        SameLine = AddWidget<CIMGUISameLine>("SameLine");

        m_InputPivotZ = AddWidget<CIMGUITextInput>("##PivotZ", 50.f, 20.f);
        m_InputPivotZ->SetNumberFloat(true);
        m_InputPivotZ->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPivotZ);


        m_InputPivotX->AddFlag(ImGuiInputTextFlags_AlwaysOverwrite);
        m_InputPivotY->AddFlag(ImGuiInputTextFlags_AlwaysOverwrite);
        m_InputPivotZ->AddFlag(ImGuiInputTextFlags_AlwaysOverwrite);
    }

	// Move
	{
		CIMGUIText* Text = AddWidget<CIMGUIText>("Move");
		Text->SetFont("DefaultFont");
		Text->SetText("Move");

		CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

		CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
		Label->SetText("X");

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputMoveX = AddWidget<CIMGUITextInput>("##MoveX", 50.f, 20.f);
		m_InputMoveX->SetNumberFloat(true);
		m_InputMoveX->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPivotX);

		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
		Label->SetText("Y");
		SameLine = AddWidget<CIMGUISameLine>("SameLine");

		m_InputMoveY = AddWidget<CIMGUITextInput>("##MoveY", 50.f, 20.f);
		m_InputMoveY->SetNumberFloat(true);
		m_InputMoveY->SetInputCallback<CObjectDetailWindow>(this, &CObjectDetailWindow::InputPivotY);

		m_InputMoveX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
		m_InputMoveY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	}

	

	return true;
}


void CObjectDetailWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
	
	if (m_ObjectWindow)
	{
		CSceneComponent* SelectComponent = (CSceneComponent*)m_ObjectWindow->GetSelectComponent();

		if (SelectComponent)
		{
			m_InputPosX->SetFloat(SelectComponent->GetWorldPos().x);
			m_InputPosY->SetFloat(SelectComponent->GetWorldPos().y);
			m_InputPosZ->SetFloat(SelectComponent->GetWorldPos().z);
								  
			m_InputRotX->SetFloat(SelectComponent->GetWorldRotation().x);
			m_InputRotY->SetFloat(SelectComponent->GetWorldRotation().y);
			m_InputRotZ->SetFloat(SelectComponent->GetWorldRotation().z);

			m_InputScaleX->SetFloat(SelectComponent->GetWorldScale().x);
			m_InputScaleY->SetFloat(SelectComponent->GetWorldScale().y);
			m_InputScaleZ->SetFloat(SelectComponent->GetWorldScale().z);
									
            m_InputPivotX->SetFloat(SelectComponent->GetPivot().x);
            m_InputPivotY->SetFloat(SelectComponent->GetPivot().y);
            m_InputPivotZ->SetFloat(SelectComponent->GetPivot().z);

			m_InputMoveX->SetFloat(SelectComponent->GetMove().x);
			m_InputMoveY->SetFloat(SelectComponent->GetMove().y);

	/*		m_Falling->SetText(SelectComponent->IsFalling());
			m_Jump->SetText(SelectComponent->IsJump());*/
			m_BounceForce->SetFloat(SelectComponent->GetBounceForce());
		}
	}
}

void CObjectDetailWindow::InputPosX()
{
	float x = m_InputPosX->GetValueFloat();

	m_ObjectWindow->SetPosX(x);
}

void CObjectDetailWindow::InputPosY()
{
	float y = m_InputPosY->GetValueFloat();

	m_ObjectWindow->SetPosY(y);
}

void CObjectDetailWindow::InputPosZ()
{
	float z = m_InputPosZ->GetValueFloat();

	m_ObjectWindow->SetPosZ(z);
}

void CObjectDetailWindow::InputRotX()
{
	float x = m_InputRotX->GetValueFloat();

	m_ObjectWindow->SetRotX(x);
}

void CObjectDetailWindow::InputRotY()
{
	float y = m_InputRotY->GetValueFloat();

	m_ObjectWindow->SetRotY(y);
}

void CObjectDetailWindow::InputRotZ()
{
	float z = m_InputRotZ->GetValueFloat();

	m_ObjectWindow->SetRotZ(z);
}

void CObjectDetailWindow::InputScaleX()
{
	float x = m_InputScaleX->GetValueFloat();

	m_ObjectWindow->SetScaleX(x);
}

void CObjectDetailWindow::InputScaleY()
{
	float y = m_InputScaleY->GetValueFloat();

	m_ObjectWindow->SetScaleY(y);
}

void CObjectDetailWindow::InputScaleZ()
{
	float z = m_InputScaleZ->GetValueFloat();

	m_ObjectWindow->SetScaleZ(z);
}

void CObjectDetailWindow::InputPivotX()
{
    float x = m_InputPivotX->GetValueFloat();

    m_ObjectWindow->SetPivotX(x);
}

void CObjectDetailWindow::InputPivotY()
{
    float y = m_InputPivotY->GetValueFloat();

    m_ObjectWindow->SetPivotY(y);
}

void CObjectDetailWindow::InputPivotZ()
{
    float z = m_InputPivotZ->GetValueFloat();

    m_ObjectWindow->SetPivotZ(z);
}

void CObjectDetailWindow::InputMoveX()
{
	float x = m_InputMoveX->GetValueFloat();

	m_ObjectWindow->SetMoveX(x);
}

void CObjectDetailWindow::InputMoveY()
{
	float y = m_InputMoveY->GetValueFloat();

	m_ObjectWindow->SetMoveX(y);
}

