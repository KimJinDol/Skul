#include "SpriteDetailWindow.h"
#include "IMGUILabel.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "GameObject.h"
#include "SceneObjectWindow.h"
#include "Component/SpriteComponent.h"
#include "Resource/Animation2D.h"

CSpriteDetailWindow::CSpriteDetailWindow()  :
    m_AnimationText(nullptr)
{
}

CSpriteDetailWindow::~CSpriteDetailWindow()
{
}

bool CSpriteDetailWindow::Init()
{
    CObjectDetailWindow::Init();

    CIMGUILabel* Label = AddWidget<CIMGUILabel>("Animation", 100.f, 20.f);
    Label->SetText("Animation");

    CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

    m_AnimationText = AddWidget<CIMGUIText>("AnimationName");
    m_AnimationText->SetFont("DefaultFont");
    return true;
}

void CSpriteDetailWindow::Update(float DeltaTime)
{
    CObjectDetailWindow::Update(DeltaTime);

    if (m_ObjectWindow)
    {
        CGameObject* SelectObject = m_ObjectWindow->GetSelectObject();
        
        if (SelectObject)
        {
            m_SpriteComponent = (CSpriteComponent*)SelectObject->FindSceneComponent("Sprite");
            m_AnimationText->SetText(m_SpriteComponent->GetAnimation2D()->GetName().c_str());
        }
    }
}
