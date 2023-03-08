#include "Effect.h"
#include "Resource/Animation2D.h"

CEffect::CEffect()  :
    m_bIsLoop(false),
    m_bUseLocalSpace(false)
{
}

CEffect::CEffect(const CEffect& obj)
{
}

CEffect::~CEffect()
{
}

void CEffect::SetOpacity(float Opacity)
{
    m_Sprite->GetMaterial(0)->SetOpacity(Opacity);
}

void CEffect::SetAutoDestroy()
{
    m_Anim->SetFrameEndFunction<CEffect>(m_Anim->GetCurrentSequenceName(), this, &CEffect::EffectEnd);
}

void CEffect::SetAutoDestroy(const std::string& Name)
{
    Sequence2DInfo* Info = m_Anim->FindSequence(Name);
    
    if (!Info) return;

    m_Anim->SetFrameEndFunction<CEffect>(Info->Sequence->GetName(), this, &CEffect::EffectEnd);
}

void CEffect::EffectEnd()
{
    Active(false);
}

void CEffect::Start()
{
    CGameObject::Start();
}

bool CEffect::Init()
{
    CGameObject::Init();

    m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
    m_Sprite->SetRender2DType(Render_Type_2D::RT2D_Particle);

    SetRootComponent(m_Sprite);
    m_Sprite->CreateAnimation2D<CAnimation2D>();
    m_Anim = m_Sprite->GetAnimation2D();
    
    return true;
}

void CEffect::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (m_bUseLocalSpace && m_pOwner)
    {
        SetWorldPos(m_pOwner->GetWorldPos() + m_Offset);

        if (m_View == Object_View::Left)
            SetWorldPosX(m_pOwner->GetWorldPos().x - m_Offset.x);
    }
}

void CEffect::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CEffect::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}
