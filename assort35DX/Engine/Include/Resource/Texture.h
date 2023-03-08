#pragma once

#include "../Ref.h"

/*
텍스처 : 이미지의 픽셀 정보를 저장하고 있는 객체
*/
struct TextureResourceInfo
{
    // ScratchImage : 이미지의 픽셀정보를 저장하기 위한 기능.
    ScratchImage* Image;
    ID3D11ShaderResourceView* SRV;  // 텍스처를 쉐이더에 넘겨주려면 이 ShaderResoureView가 필요하다. 셰이더가 텍스처에 엑세스할 수 있게끔 텍스처를 래핑해준다.
    unsigned int    Width;
    unsigned int    Height;
    TCHAR* FileName;
    char* PathName;
    TCHAR* FullPath;
    TCHAR* FilePath;

    TextureResourceInfo() :
        Image(nullptr),
        SRV(nullptr),
        FileName(nullptr),
        PathName(nullptr),
        FullPath(nullptr),
        FilePath(nullptr),
        Width(0),
        Height(0)
    {
    }
};

class CTexture :
    public CRef
{
    friend class CResourceManager;
    friend class CSceneResource;
    friend class CMaterial;

protected:
    CTexture();
    virtual ~CTexture();

protected:
    class CScene* m_pScene;
    std::vector<TextureResourceInfo*>   m_vecResourceInfo;
    Image_Type                          m_ImageType;
    ID3D11ShaderResourceView* m_ArraySRV;

public:
    TextureResourceInfo* GetTextureResourceInfo(int Index = 0)   const
    {
        return m_vecResourceInfo[Index];
    }

    ID3D11ShaderResourceView* GetResource(int Index = 0) const
    {
        return m_vecResourceInfo[Index]->SRV;
    }

    Image_Type GetImageType()   const
    {
        return m_ImageType;
    }

    unsigned int GetWidth(int Index = 0)    const
    {
        return m_vecResourceInfo[Index]->Width;
    }

    unsigned int GetHeight(int Index = 0)    const
    {
        return m_vecResourceInfo[Index]->Height;
    }

    size_t GetImageCount()  const
    {
        return m_vecResourceInfo.size();
    }

public:
    bool LoadTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFilePath(const std::string& Name, const TCHAR* FilePath,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    bool LoadTexture(const std::string& Name,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(const std::string& Name,
        const std::vector<const TCHAR*>& vecFullPath);
    bool LoadTextureArray(const std::string& Name,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureArrayFullPath(const std::string& Name,
        const std::vector<const TCHAR*>& vecFullPath);

protected:
    bool CreateResource(int Index);
    bool CreateResourceArray();

public:
    virtual void SetShader(int Register, int ShaderType, int Index = 0);    // 쉐이더에서 해당 텍스처를 사용하기 위해 레지스터에 등록해주는 함수.
    virtual void ResetShader(int Register, int ShaderType, int Index = 0);
    virtual void Save(FILE* pFile);
};

