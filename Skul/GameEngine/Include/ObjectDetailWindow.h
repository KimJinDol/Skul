#pragma once
#include "IMGUIWindow.h"
class CObjectDetailWindow :
	public CIMGUIWindow
{
public:
	CObjectDetailWindow();
	virtual ~CObjectDetailWindow();

protected:
	class CIMGUITextInput* m_InputPosX;
	class CIMGUITextInput* m_InputPosY;
	class CIMGUITextInput* m_InputPosZ;

	class CIMGUITextInput* m_InputRotX;
	class CIMGUITextInput* m_InputRotY;
	class CIMGUITextInput* m_InputRotZ;

	class CIMGUITextInput* m_InputScaleX;
	class CIMGUITextInput* m_InputScaleY;
	class CIMGUITextInput* m_InputScaleZ;

    class CIMGUITextInput* m_InputPivotX;
    class CIMGUITextInput* m_InputPivotY;
    class CIMGUITextInput* m_InputPivotZ;

	class CIMGUITextInput* m_InputMoveX;
	class CIMGUITextInput* m_InputMoveY;

	class CIMGUITextInput* m_BounceForce;

	class CIMGUIText* m_Falling;
	class CIMGUIText* m_Jump;


	class CSceneObjectWindow* m_ObjectWindow;

public:
	void SetPosition(const Vector3& Pos);
	void SetRotation(const Vector3& Rot);
	void SetScale(const Vector3& Scale);
	void SetPivot(const Vector3& Pivot);
	void SetMove(const Vector3& Move);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void InputPosX();
	void InputPosY();
	void InputPosZ();

	void InputRotX();
	void InputRotY();
	void InputRotZ();

	void InputScaleX();
	void InputScaleY();
	void InputScaleZ();

    void InputPivotX();
    void InputPivotY();
    void InputPivotZ();

	void InputMoveX();
	void InputMoveY();
};