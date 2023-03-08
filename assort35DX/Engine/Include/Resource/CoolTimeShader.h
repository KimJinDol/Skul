#pragma once
#include "GraphicShader.h"

class CCoolTimeShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CCoolTimeShader();
    virtual ~CCoolTimeShader();

public:
    virtual bool Init();
};

