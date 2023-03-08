#pragma once
#include "ObjectDetailWindow.h"
class CSpriteDetailWindow :
    public CObjectDetailWindow
{
public:
	CSpriteDetailWindow();
	virtual ~CSpriteDetailWindow();

private:
	class CIMGUIText* m_AnimationText;
	class CSpriteComponent* m_SpriteComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

