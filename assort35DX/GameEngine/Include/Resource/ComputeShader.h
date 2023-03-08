#pragma once

#include "Shader.h"

// Compute Shader는 출력용이 아닌 GPU성능을 이용해 연산만 하는 작업이기 때문에 Input Layout이 필요가 없다.
class CComputeShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CComputeShader();
	virtual ~CComputeShader();

private:
	ID3D11ComputeShader* m_CS;
	ID3DBlob* m_CSBlob;

public:
	bool LoadComputeShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);

public:
	virtual bool Init();
	virtual void SetShader();
	void Excute(unsigned int x, unsigned int y, unsigned int z);
};

