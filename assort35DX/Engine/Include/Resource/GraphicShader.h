#pragma once

#include "Shader.h"

/*
Vertex Shader, Pixel Shader, Domain Shader, Hull Shader, Geometry Shader
*/

class CGraphicShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CGraphicShader();
	virtual ~CGraphicShader();

protected:
	// ID3DBlob 은 Shader 코드를 저장해두는 역할을 한다.
    // Blob에 쉐이더 파일에 포함된 코드 내용을 저장하고 저장된 Shader 코드 내용을 이용하여 Device에서 쉐이더를 만드는 것임.
	ID3D11VertexShader* m_pVS;
	ID3DBlob* m_pVSBlob;

	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pPSBlob;

	ID3D11HullShader* m_pHS;
	ID3DBlob* m_pHSBlob;

	ID3D11DomainShader* m_pDS;
	ID3DBlob* m_pDSBlob;

	ID3D11GeometryShader* m_pGS;
	ID3DBlob* m_pGSBlob;

public:
	// 각각의 쉐이더들을 내가 원하는 엔트리 포인터를 사용하여 로딩하게 해주는 함수.
	// 쉐이더는 미리 컴파일 후에 사용하는 것이 아니라 이미지 처럼 필요에 따라 로딩해서 사용하는 방식이기 때문에
	// 엔트리 포인터가 main이 아니다.
	bool LoadVertexShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadPixelShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadDomainShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadHullShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadGeometryShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);

public:
	virtual bool Init();
	virtual bool CreateInputLayout();
	virtual void SetShader();
};

