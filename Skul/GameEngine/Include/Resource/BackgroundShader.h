#pragma once
#include "GraphicShader.h"

class CBackgroundShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CBackgroundShader();
    virtual ~CBackgroundShader();

public:
    virtual bool Init();
};

