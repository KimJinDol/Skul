#pragma once

#include "../Ref.h"
#include "Shader.h"
#include "Texture.h"

struct MaterialTextureInfo
{
	std::string		Name;
	Texture_Link    Link;
	CSharedPtr<CTexture> pTexture;
	int             Register;
	int             ShaderType;

	MaterialTextureInfo() :
		Link(Texture_Link::BaseTexture),
		Register(0),
		ShaderType(TST_PIXEL)
	{
	}
};

struct MaterialCallback
{
	std::string	Name;
	std::function<void()>	Func;
};

class CMaterial :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;
	friend class CMesh;

protected:
	CMaterial();
	CMaterial(const CMaterial& material);
	virtual ~CMaterial();

protected:
	class CScene* m_pScene;
	class CMaterialConstantBuffer* m_pCBuffer;
	Vector4     m_BaseColor;
	Vector4     m_EmissiveColor;
	std::vector<MaterialTextureInfo*>	m_vecTexture;
	CSharedPtr<CShader> m_pShader;
	bool        m_bTransparency;
	class CRenderState* m_RenderState[RST_End];
	float		m_Opacity;
	Vector2		m_AddUV;
	Vector4		m_Tint;
	bool		m_PaperBurnEnable;
	bool		m_DistortionEnable;
	Vector2		m_StartUV;
	Vector2		m_EndUV;

	std::list<MaterialCallback*>	m_SetMaterialList;
	std::list<MaterialCallback*>	m_ResetMaterialList;

public:
	const MaterialTextureInfo* GetMaterialTextureInfo(int Index = 0)	const
	{
		return m_vecTexture[Index];
	}

public:
	void SetScene(class CScene* pScene)
	{
		m_pScene = pScene;
	}

public:
	void SetStartEndUV(unsigned int x, unsigned int y, const Vector2& FrameSize);
	void SetStartUV(const Vector2& UV);
	void SetStartUV(float u, float v);
	void SetEndUV(const Vector2& UV);
	void SetEndUV(float u, float v);
	void SetRenderState();
	void SetTransparency(bool bTransparency);
	void SetOpacity(float Opacity)
	{
		m_Opacity = Opacity;
	}

public:
	void SetTintColor(const Vector4& Color);
	void SetTintColor(float r, float g, float b, float a);
	void SetTintColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a);

	void SetBaseColor(const Vector4& Color);
	void SetBaseColor(float r, float g, float b, float a);
	void SetBaseColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a);

	void SetEmissiveColor(const Vector4& Color);
	void SetEmissiveColor(float r, float g, float b, float a);
	void SetEmissiveColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a);

	void SetAddUV(const Vector2& UV);

public:
	void AddUV(const Vector2& UV);

	bool AddTexture(const std::string& Name, class CTexture* Texture);
	bool AddTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool AddTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool AddTexture(const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool AddTextureFullPath(const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);
	bool AddTextureArray(const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool AddTextureArrayFullPath(const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);

	// 기존 Texture를 교체한다.
	bool SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTexture(const std::string& Name, class CTexture* Texture);
	bool SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool SetTexture(const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);
	bool SetTextureArray(const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureArrayFullPath(const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);

	bool SetTextureLink(const std::string& Name, Texture_Link Link);
	bool SetTextureRegister(const std::string& Name, int Register);
	bool SetTextureShaderType(const std::string& Name, int ShaderType);

	void PaperBurnEnable(bool Enable);
	void DistortionEnable(bool Enable);


public:
	virtual bool Init();
	virtual void SetShader(const std::string& Name);	// 어떤 쉐이더를 써서 출력할 지.
	virtual void SetMaterial();		// 쉐이더에 머터리얼 정보를 보내주기 위한 함수. (Set Transform처럼)
	virtual void ResetMaterial();	// 상수 버퍼에 저장된 머터리얼 정보 초기화해주는 함수.
	virtual void SetMaterialEmptyShader();
	virtual void ResetMaterialEmptyShader();
	virtual CMaterial* Clone();
	void Save(const TCHAR* FileName, const std::string& PathName = MAP_PATH);
	virtual void Save(FILE* pFile);
	void SaveFullPath(const TCHAR* FullPath);
	void Load(const TCHAR* FileName, const std::string& PathName = MAP_PATH);
	virtual void Load(FILE* pFile);
	void LoadFullPath(const TCHAR* FullPath);

public:
	template <typename T>
	void AddSetMaterialCallback(const std::string& Name, T* Obj, void(T::* Func)())
	{
		MaterialCallback* Callback = new MaterialCallback;

		Callback->Name = Name;
		Callback->Func = std::bind(Func, Obj);

		m_SetMaterialList.push_back(Callback);
	}

	template <typename T>
	void AddResetMaterialCallback(const std::string& Name, T* Obj, void(T::* Func)())
	{
		MaterialCallback* Callback = new MaterialCallback;

		Callback->Name = Name;
		Callback->Func = std::bind(Func, Obj);

		m_ResetMaterialList.push_back(Callback);
	}

	void DeleteSetMaterialCallback(const std::string& Name)
	{
		auto	iter = m_SetMaterialList.begin();
		auto	iterEnd = m_SetMaterialList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->Name == Name)
			{
				SAFE_DELETE((*iter));
				m_SetMaterialList.erase(iter);
				return;
			}
		}
	}

	void DeleteResetMaterialCallback(const std::string& Name)
	{
		auto	iter = m_ResetMaterialList.begin();
		auto	iterEnd = m_ResetMaterialList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->Name == Name)
			{
				SAFE_DELETE((*iter));
				m_ResetMaterialList.erase(iter);
				return;
			}
		}
	}
};

