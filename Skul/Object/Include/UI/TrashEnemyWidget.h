#pragma once
#include "UI\WidgetWindow.h"
#include "UI/Text.h"
#include "UI/ProgressBar.h"
#include "UI/Button.h"
#include "UI/NumberWidget.h"
#include "UI/Image.h"

class CTrashEnemyWidget :
    public CWidgetWindow
{
public:
    CTrashEnemyWidget();
    CTrashEnemyWidget(const CTrashEnemyWidget& widget);
    virtual ~CTrashEnemyWidget();

private:
    class CAICharacter* m_pOwnerAI;

    CSharedPtr<CProgressBar>    m_HPBar;
    CSharedPtr<CProgressBar>    m_DamageBar;

    bool    m_bOnChangeHP;
    float   m_DamageChangeWaitTerm;
    float   m_CurDamageChangeWaitTerm;
    float   m_DamageLerpSpeed;

public:
    void SetOwnerAI(CAICharacter* Owner);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();

public:
    void ClickCallback();
};

