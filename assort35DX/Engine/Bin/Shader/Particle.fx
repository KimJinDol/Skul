
#include "Share.fx"

// c++코드에서 가져와야 하는 정보는 상수버퍼로 가져올 것이다.
cbuffer ParticleCBuffer : register(b11)
{
	float3	g_ParticlePos;
	uint	g_ParticleSpawnCount;
	float3	g_ParticleStartScale;
	uint	g_ParticleSpawnCountMax;
	float3	g_ParticleEndScale;
	float	g_ParticleLifeTimeMin;
	float3	g_ParticleRange;
	float	g_ParticleLifeTimeMax;
	float4	g_ParticleStartColor;
	float4	g_ParticleEndColor;
	float	g_MinSpeed;
	float	g_MaxSpeed;
	int		g_ParticleMove;
	int		g_Particle2D;
	float3	g_ParticleMoveDir;
	float	g_ParticleDefaultZ;
	float3	g_ParticleMoveAngle;
	int		g_ParticleGravity;
};

#define	GRAVITY	9.8f

struct ParticleInfo
{
	float3	WorldPos;
	float3	Dir;
	float	Speed;
	float	AddSpeed;
	float	LifeTime;
	float	MaxLifeTime;
	int		Alive;
	float	FallTime;
	float	FallStartY;
};

struct ParticleInfoShared
{
	uint	SpawnCount;
	uint	CurrentCount;

	float3	StartScale;
	float3	EndScale;
	
	float4	StartColor;
	float4	EndColor;

	int	GravityEnable;
};

// RW : 읽고 쓸수 있다는 뜻.
// unordered access view형식으로 사용할 것이기 때문에 레퍼런스 번호 앞에 u라는 접두어가 붙는다.

// 구조적 버퍼를 사용하려면 그에 대응되는 구조체를 셰이더 프로그램 안에서 정의해야 한다.
// 파티클 시스템에서 구조화 버퍼는 출력용으로 만들어 사용된다.
RWStructuredBuffer<ParticleInfo>		g_ParticleArray	: register(u0);
RWStructuredBuffer<ParticleInfoShared>	g_ParticleShare	: register(u1);

[numthreads(64, 1, 1)] // Thread ID가 곧 버퍼의 인덱스가 되는 셈.
void ParticleUpdate(uint3 ThreadID : SV_DispatchThreadID)
{
	// 위에서 g_ParticleShare라는 객체를 RW타입으로 정의했기 때문에 쓰기가 가능하다.
	g_ParticleShare[0].SpawnCount = g_ParticleSpawnCount;
	g_ParticleShare[0].StartScale = g_ParticleStartScale;
	g_ParticleShare[0].EndScale = g_ParticleEndScale;
	g_ParticleShare[0].StartColor = g_ParticleStartColor;
	g_ParticleShare[0].EndColor = g_ParticleEndColor;
	g_ParticleShare[0].GravityEnable = g_ParticleGravity;

	// 스레드 개수는 Spawn Count Max 이상일 수 없으므로 리턴.
	if (g_ParticleSpawnCountMax <= ThreadID.x)
		return;

	// 해당 인덱스의 파티클이 비활성화 상태라면 아래 실행
	// 파티클의 방향과 위치등을 새로 정해주는 과정이다.
	if (g_ParticleArray[ThreadID.x].Alive == 0)
	{
		int	SpawnCount = g_ParticleShare[0].SpawnCount;
		int	Exchange = 0;

		if (g_ParticleShare[0].SpawnCount == 1)
		{
			int	InputValue = SpawnCount - 1;

			// 함수 인자에 in, out, inout 을 붙일 수 있다.
			// in은 일반적인 함수 인자로 함수 외부에서 값을 넘겨줄때 사용한다.
			// out은 반쪽짜리 레퍼런스이다. 이 함수가 결과값을 return 말고
			// out으로 되어 있는 인자에 값을 넣어주면 그 값을 이 함수를 호출한
			// 부분에서 가져다가 사용할 수 있다.
			// 단, out은 함수 내에서 연산할때 사용하면 책임지지 않는다.
			// inout은 레퍼런스와 동일.
			// 아래 함수는 3번 인자에 1번의 원본값을 넣어준다.
			// 그러면서 1번인자의 값을 2번인자의 값으로 채워준다.

			// InterlockedExchange : 임계 영역에 대해 다른 스레드에서 접근을 불가하게 만들어 안전하게 값을 변경하게 해주는 함수이다.
			// InterlockedExchange(변경할 변수,	이 값으로 변경될 것임, 1번 인자의 바뀌기 전의 값(오리지널값))
			InterlockedExchange(g_ParticleShare[0].SpawnCount, InputValue, Exchange);
			// 다른 스레드에서 이미 파티클을 살려버렸다면 지금 스레드에서 살릴 필요가 없으니 Exchange값과 SpawnCount값을 비교해보는 것이다.
			if (Exchange == SpawnCount)
				g_ParticleArray[ThreadID.x].Alive = 1;
		}

		if (g_ParticleArray[ThreadID.x].Alive == 0)
			return;

		// 파티클이 비규칙적이게 흩날리게 하기 위해 난수로 포지션을 구한다.
		// 살려야 하는 파티클이므로 파티클 정보를 계산한다.
		float	key = ThreadID.x / (float)g_ParticleSpawnCountMax;

		// Rand는 무조건 0 ~ 1 사이의 값을 얻어온다.
		float3	RandomPos = float3(Rand(key), Rand(key * 2.f), Rand(key * 3.f));

		// -0.5를 해주어서 -0.5 ~ 0.5 사이로 만들어주고 크기를 곱한다.
		float3	PosRange = (RandomPos - 0.5f) * g_ParticleRange;

		g_ParticleArray[ThreadID.x].WorldPos = g_ParticlePos + PosRange;
		// 카메라 기준으로 Y값을 변환해 깊이값을 비교하기 위해 변환하는 것임.
		float	ConvertY = g_ParticleArray[ThreadID.x].WorldPos.y - g_CameraBottom;

		// 생성할 파티클을 2D기준으로 아래일수록 앞에 나와있다 가정하고 깊이값을 설정해주는 것임.
		float	RatioZ = ConvertY / (g_Resolution.y * 2.f);

		if (g_ParticleDefaultZ >= 0.7f)
		{
			// Min : 0.7f   Max : 0.99999f 
			g_ParticleArray[ThreadID.x].WorldPos.z = 
				(0.99999f - 0.7f) * RatioZ + 0.7f;
		}

		// Default
		else if (g_ParticleDefaultZ >= 0.3f)
		{
			// Min : 0.3f   Max : 0.69999f 
			g_ParticleArray[ThreadID.x].WorldPos.z = 
				(0.69999f - 0.3f) * RatioZ + 0.3f;
		}

		// Particle
		else
		{
			// Min : 0.f   Max : 0.29999f 
			g_ParticleArray[ThreadID.x].WorldPos.z = 
				0.29999f * RatioZ;
		}

		g_ParticleArray[ThreadID.x].FallTime = 0.f;
		g_ParticleArray[ThreadID.x].FallStartY = 
			g_ParticleArray[ThreadID.x].WorldPos.y;

		g_ParticleArray[ThreadID.x].MaxLifeTime = RandomPos.x * (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;
		g_ParticleArray[ThreadID.x].LifeTime = 0.f;
		// 만약 움직이는 파티클이라면 이하 실행
		if (g_ParticleMove == 1)
		{	// 위에서 구한 PosRange는 원점을 기준으로 한 Pos이기 때문에 정규화(Normalize)해서 방향을 구한다.
			g_ParticleArray[ThreadID.x].Speed = RandomPos.x * (g_MaxSpeed - g_MinSpeed) + g_MinSpeed;

			float2	RandomDir = RandomPos.xy * 2.f - 1.f;
			g_ParticleArray[ThreadID.x].Dir = normalize(normalize(float3(RandomDir, 0.f)) + g_ParticleMoveDir);
			//g_ParticleArray[ThreadID.x].Dir = normalize(PosRange);
		}
	}

	// 현재 파티클이 살아있는 파티클이라면 파티클 정보를 갱신한다.
	else
	{
		g_ParticleArray[ThreadID.x].LifeTime += g_DeltaTime;

		if (g_ParticleMove == 1)
		{	// DeltaTime에 비례하여 방향만큼 위치를 갱신해준다.
			g_ParticleArray[ThreadID.x].WorldPos.xy +=
				g_ParticleArray[ThreadID.x].Dir.xy * 
				g_ParticleArray[ThreadID.x].Speed * g_DeltaTime;
			// 깊이 값 갱신
			if (g_ParticleShare[0].GravityEnable)
			{
				g_ParticleArray[ThreadID.x].FallTime += g_DeltaTime * 10.f;

				float	Velocity = 0.1f * g_ParticleArray[ThreadID.x].Speed *
					g_ParticleArray[ThreadID.x].FallTime;

				g_ParticleArray[ThreadID.x].WorldPos.y =
					g_ParticleArray[ThreadID.x].FallStartY +
					(Velocity - 0.5f * GRAVITY * g_ParticleArray[ThreadID.x].FallTime * g_ParticleArray[ThreadID.x].FallTime);
			}

			float	ConvertY = g_ParticleArray[ThreadID.x].WorldPos.y - g_CameraBottom;

			float	RatioZ = ConvertY / (g_Resolution.y * 2.f);

			if (g_ParticleDefaultZ >= 0.7f)
			{
				// Min : 0.7f   Max : 0.99999f 
				g_ParticleArray[ThreadID.x].WorldPos.z =
					(0.99999f - 0.7f) * RatioZ + 0.7f;
			}

			// Default
			else if (g_ParticleDefaultZ >= 0.3f)
			{
				// Min : 0.3f   Max : 0.69999f 
				g_ParticleArray[ThreadID.x].WorldPos.z =
					(0.69999f - 0.3f) * RatioZ + 0.3f;
			}

			// Particle
			else
			{
				// Min : 0.f   Max : 0.29999f 
				g_ParticleArray[ThreadID.x].WorldPos.z =
					0.29999f * RatioZ;
			}
		}

		if (g_ParticleArray[ThreadID.x].LifeTime >=
			g_ParticleArray[ThreadID.x].MaxLifeTime)
		{
			g_ParticleArray[ThreadID.x].Alive = 0;
		}
	}
}

// 인스턴싱 ---------------------------
StructuredBuffer<ParticleInfo>		g_ParticleArrayInput	: register(t30);
StructuredBuffer<ParticleInfoShared>	g_ParticleShareInput	: register(t31);

struct VS_INPUT_PARTICLE
{
	float3	Pos : POSITION; // 위치 정보(정점 정보 하나)만으로 지오메트리 쉐이더에서 사각형(도형)으로 확장할 것임. 그러므로 위치 정보가 필요.
	uint	InstanceID : SV_InstanceID;	// 인스턴스 ID. 위의 g_ParticleArrayInput의 인덱스 번호를 의미.
										// (결국 g_ParticleArrayInput도 똑같은 파티클을 여러개 찍어내야 하는 것이기 때문에 인스턴싱으로 하는 것)
										// SV_InstanceID : 인스턴싱 전용 System Value 시멘틱.
};

struct VS_OUTPUT_PARTICLE
{
	uint	InstanceID : TEXCOORD;
};

VS_OUTPUT_PARTICLE ParticleVS(VS_INPUT_PARTICLE input)
{
	VS_OUTPUT_PARTICLE	output = (VS_OUTPUT_PARTICLE)0;

	output.InstanceID = input.InstanceID;

	return output;
}

struct GS_PARTICLE_OUTPUT
{
	float4	Pos	: SV_Position;
	float4	Color : COLOR;
	float2	UV	: TEXCOORD;
	float4	ProjPos : POSITION;
};

static float4	g_ParticleLocalPos[4] =
{
	{-0.5f, 0.5f, 0.f, 0.f},
	{0.5f, 0.5f, 0.f, 0.f},
	{0.5f, -0.5f, 0.f, 0.f},
	{-0.5f, -0.5f, 0.f, 0.f}
};

[maxvertexcount(6)] // maxvertexcount : 지오메트리 쉐이더에서 확장할 정점의 개수(꼭짓점 개수). 6이니까 위치 정보만으로 정점 6개를 사용해 사각형으로 확장하겠다는 뜻.
void ParticleGS(point VS_OUTPUT_PARTICLE input[1],
	inout TriangleStream<GS_PARTICLE_OUTPUT> OutputStream)	// inout은 래퍼런스 같은 놈이기 때문에 지금 현재 ParticleGS함수 결과 값이 이 변수에 저장될 것임.
															// TriangleStream<GS_PARTICLE_OUTPUTL> 벡터이 역할
{
	uint	InstanceID = input[0].InstanceID;

	if (g_ParticleArrayInput[InstanceID].Alive == 0)
		return;

	GS_PARTICLE_OUTPUT	OutputArray[4] =
	{
		(GS_PARTICLE_OUTPUT)0.f,
		(GS_PARTICLE_OUTPUT)0.f,
		(GS_PARTICLE_OUTPUT)0.f,
		(GS_PARTICLE_OUTPUT)0.f
	};
	// UV 값을 넣어놓는다.
	OutputArray[0].UV = float2(0.f, 0.f);	// UV 좌상단
	OutputArray[1].UV = float2(1.f, 0.f);	// UV 우상단
	OutputArray[2].UV = float2(1.f, 1.f);	// UV 우하단
	OutputArray[3].UV = float2(0.f, 1.f);	// UV 좌하단

	// UpdateShader에서 파티클의 월드좌표를 만들어놨다.
	// 그렇기 때문에 이 월드좌표를 뷰행렬 * 투영행렬을 해서 투영공간으로 변환한다.
	float	Ratio = g_ParticleArrayInput[InstanceID].LifeTime /
		g_ParticleArrayInput[InstanceID].MaxLifeTime;

	// 위에서 구한 비율(Ratio)값을 사용해서 스케일과 컬러값을 조절해준다.
	float3	Scale = lerp(g_ParticleShareInput[0].StartScale,
		g_ParticleShareInput[0].EndScale,
		float4(Ratio, Ratio, Ratio, Ratio)).xyz;

	float4	Color = lerp(g_ParticleShareInput[0].StartColor,
		g_ParticleShareInput[0].EndColor,
		float4(Ratio, Ratio, Ratio, Ratio));

	// 4개의 정점정보를 만들어준다.
	for (int i = 0; i < 4; ++i)
	{
		// 이 과정을 통해서 원점을 기준으로 사각형이 만들어지는 것!!!!!!!
		float3	WorldPos = (float3)0.f;
		
		WorldPos.xy = g_ParticleArrayInput[InstanceID].WorldPos.xy +
			g_ParticleLocalPos[i].xy * Scale.xy;
		WorldPos.z = g_ParticleArrayInput[InstanceID].WorldPos.z;

		// 위에서 구해준 WorldPos 값에 뷰 * 투영 행렬을 mul함수를 통해 곱해버린다!!!
		OutputArray[i].ProjPos = mul(float4(WorldPos, 1.f), g_matVP);
		OutputArray[i].Pos = OutputArray[i].ProjPos;
		OutputArray[i].Color = Color;
	}
	// Append는 인자의 값을 OutputStream에 넣어주는 함수이다. 최종 결과값을 inout (래퍼런스 비슷) 인자인 OutputStream에 넣어서 결과를 전달해주는 것.
	OutputStream.Append(OutputArray[0]);
	OutputStream.Append(OutputArray[1]);
	OutputStream.Append(OutputArray[2]);
	OutputStream.RestartStrip(); // 삼각형 정보 셋업을 다시 시작하는 함수

	OutputStream.Append(OutputArray[0]);
	OutputStream.Append(OutputArray[2]);
	OutputStream.Append(OutputArray[3]);
	OutputStream.RestartStrip();
}

float4 ParticlePS(GS_PARTICLE_OUTPUT input) : SV_Target // 함수 이름 옆에 SV_Target시멘틱이 들어가는 이유는, 리턴값이 SV_Target 시멘틱 0번에 들어가라는 의미이다.
{
	float4	Color = (float4)0.f;

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);
	float4	EmissiveColor = g_EmissiveTexture.Sample(g_AnisotropicSmp, input.UV);

	Color.rgb = (BaseColor.rgb * g_vMtrlBaseColor.rgb * input.Color.rgb + EmissiveColor.rgb * g_vMtrlEmissiveColor.rgb) * (float3)g_Contrast;
	Color.a = BaseColor.a * g_MtrlOpacity * input.Color.a;

	Color = Distortion(Color, input.UV, input.ProjPos);

	return Color;
}

