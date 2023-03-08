#pragma once
#include "IMGUIWindow.h"
#include "GameObject.h"
#include "Component/SceneComponent.h"

class CSceneObjectWindow :
	public CIMGUIWindow
{
public:
	CSceneObjectWindow();
	virtual ~CSceneObjectWindow();

private:
	unsigned int	m_CreateObjectCount;
	class CIMGUIListBox* m_ObjListBox;
	class CIMGUIListBox* m_ComponentListBox;
	CSharedPtr<CGameObject> m_SelectObject;
	int		m_SelectObjectIndex;
	CSharedPtr<CSceneComponent> m_SelectComponent;
	int		m_SelectComponentIndex;
	int		m_CreateComponentIndex;

	class CIMGUITextInput* m_NameInput;

public:
	class CGameObject* GetSelectObject() const
	{
		return m_SelectObject.Get();
	}

	class CComponent* GetSelectComponent() const
	{
		return m_SelectComponent.Get();
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void AddItem(const char* Item);
	void DeleteItem(const char* Item);

	void ListCallback(int SelectIndex, const char* Item);
	void ComponentListCallback(int SelectIndex, const char* Item);
	void ComponentComboCallback(int SelectIndex, const char* Item);
	void CreateObjectButtonClick();
	void InputNamePopupButton();

public:
	void SetPosX(float x);
	void SetPosY(float y);
	void SetPosZ(float z);

	void SetRotX(float x);
	void SetRotY(float y);
	void SetRotZ(float z);

	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);

    void SetPivotX(float x);
    void SetPivotY(float y);
    void SetPivotZ(float z);

	void SetMoveX(float x);
	void SetMoveY(float y);
	//void SaveButton();
	//void LoadButton();
};

