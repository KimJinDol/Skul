#pragma once
#include "Widget.h"
#include "../Resource/Texture.h"

class CCoolTime :
    public CWidget
{
    friend class CViewport;
    friend class CScene;
    friend class CWidgetWindow;

protected:
    CCoolTime();
    CCoolTime(const CCoolTime& widget);
    virtual ~CCoolTime();

protected:
    CSharedPtr<CTexture>    m_Texture;
    class CCoolTimeCBuffer* m_CoolTimeCBuffer;
    float   m_Ratio;
    int     m_Dir;
    int     m_Circle;

public:
    CTexture* GetTexture()  const
    {
        return m_Texture;
    }

public:
    void SetCircle(int Circle)
    {
        m_Circle = Circle;
    }

    void SetDir(int Dir = 1) // 1 : 시계방향, 2 : 반시계방향
    {
        m_Dir = Dir;
    }

    void SetRatio(float Ratio)
    {
        m_Ratio = Ratio;
    }
    void SetStartEndUV(unsigned int x, unsigned int y, const Vector2& FrameSize);
    void SetTexture(CTexture* Texture)
    {
        m_Texture = Texture;
    }

    void SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);

    void SetShader(const std::string& Name);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CCoolTime* Clone();
};
