#pragma once
#include "UI\WidgetWindow.h"
#include "UI\Image.h"

class CCinemaWidget :
    public CWidgetWindow
{
public:
    CCinemaWidget();
    CCinemaWidget(const CCinemaWidget& widget);
    virtual ~CCinemaWidget();

private:
    CSharedPtr<CImage>   m_BackgroundImage;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();

};

