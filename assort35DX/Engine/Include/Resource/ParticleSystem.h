#pragma once

#include "../Ref.h"
#include "Material.h"
#include "ParticleUpdateShader.h"

class CParticleSystem :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CParticleSystem();
	CParticleSystem(const CParticleSystem& particle);
	~CParticleSystem();

private:
	class CScene* m_Scene;
	std::vector<class CStructuredBuffer*>   m_vecBuffer; // Particle.fx 에서 계산되는 모든 데이터들은 이 버퍼에 저장될 것이다.
	CSharedPtr<CMaterial>					m_Material;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader; // 파티클 정보를 업데이트 할 때 이 쉐이더를 쓸 것임.
	ParticleInfo							m_ParticleInfo;			// ParticleInfo, ParticleInfoShared : 구조화 버퍼를 만들어낼 목적으로 c++코드 상에서 만든 구조체
	ParticleInfoShared						m_ParticleInfoShare;	// 아직까지 쓰인 부분은 없음!!!
	class CParticleConstantBuffer* m_CBuffer;	// 파티클 생성시 파티클을 초기화 시켜주기 위한 정보
	float									m_SpawnTime;	// 이 시간에 한 번씩 파티클 입자를 생성하라는 뜻
	bool									m_2D;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

public:
	bool Init();

protected:
	void AddStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int ConstantBufferType = CBT_COMPUTE);

public:
	void SetMaterial(CMaterial* Material)
	{
		m_Material = Material;
	}
	void SetMaxParticleCount(unsigned int Count);

	void SetStartColor(const Vector4& Color);
	void SetStartColor(float r, float g, float b, float a);
	void SetEndColor(const Vector4& Color);
	void SetEndColor(float r, float g, float b, float a);

	void SetStartScale(const Vector3& Scale);
	void SetStartScale(float x, float y, float z);
	void SetEndScale(const Vector3& Scale);
	void SetEndScale(float x, float y, float z);

	void SetLifeTimeMin(float Min);
	void SetLifeTimeMax(float Max);
	void SetRange(const Vector3& Range);
	void SetRange(float x, float y, float z);
	void SetMinSpeed(float Min);
	void SetMaxSpeed(float Max);
	void SetMoveEnable(bool Move);
	void SetParticle2D(bool b2D);
	void SetMoveDir(const Vector3& Dir);
	void SetMoveDir(float x, float y, float z);
	void SetMoveAngle(const Vector3& Angle);
	void SetMoveAngle(float x, float y, float z);
	void SetGravityEnable(bool Enable);

protected:
	bool ResizeBuffer(const std::string& Name, unsigned int Size,
		unsigned int Count, unsigned int Register, bool Dynamic = false,
		int ConstantBufferType = CBT_COMPUTE);

public:
	class CParticleConstantBuffer* CloneConstantBuffer()	const;
	CMaterial* CloneMaterial()	const;
	CParticleUpdateShader* GetUpdateShader()	const;
	void CloneStructuredBuffer(std::vector<class CStructuredBuffer*>& vecBuffer);
	float GetSpawnTime()	const
	{
		return m_SpawnTime;
	}

	void SetSpawnTime(float Time)
	{
		m_SpawnTime = Time;
	}
};

