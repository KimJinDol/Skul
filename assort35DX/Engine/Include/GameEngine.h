#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <math.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <typeinfo>
#include <algorithm>
#include <dinput.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <d2d1.h>
#include <process.h>

#pragma comment(lib, "d3d11.lib")	
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "Macro.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "Resource/DirectXTex.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "fmod.hpp"

#pragma comment(lib, "fmod64_vc.lib")

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif // _DEBUG


#define BASE_PATH       "BasePath"
#define SHADER_PATH     "ShaderPath"
#define TEXTURE_PATH    "TexturePath"
#define	FONT_PATH		"FontPath"
#define	SOUND_PATH		"SoundPath"
#define	MAP_PATH		"MapPath"
#define	STRING_PATH		"StringPath"

struct Resolution
{
	int Width;
	int Height;
};

struct VertexBuffer
{
	ID3D11Buffer* pBuffer;
	int         Size;   // ���� 1���� ũ��
	int         Count;  // ���� ��
	D3D11_USAGE Usage;
	void* pData;

	VertexBuffer()
	{
		pBuffer = nullptr;
		pData = nullptr;
		Size = 0;
		Count = 0;
	}
};

struct IndexBuffer
{
	ID3D11Buffer* pBuffer;
	int         Size;   // ���� 1���� ũ��
	int         Count;  // ���� ��
	D3D11_USAGE Usage;
	DXGI_FORMAT Fmt;
	void* pData;

	IndexBuffer()
	{
		pBuffer = nullptr;
		pData = nullptr;
		Size = 0;
		Count = 0;
	}
};

struct VertexColor
{
	Vector3    Pos;
	Vector4    Color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _Pos, const Vector4& _Color)
	{
		Pos = _Pos;
		Color = _Color;
	}

	VertexColor(float x, float y, float z, float r, float g, float b, float a)
	{
		Pos = Vector3(x, y, z);
		Color = Vector4(r, g, b, a);
	}
};

struct VertexUV
{
	Vector3    Pos;
	Vector2    UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& _Pos, const Vector2& _UV)
	{
		Pos = _Pos;
		UV = _UV;
	}

	VertexUV(float x, float y, float z, float u, float v)
	{
		Pos = Vector3(x, y, z);
		UV = Vector2(u, v);
	}
};

struct HierarchyName
{
	std::string Name;
	std::string ParentName;
};

struct TransformCBuffer
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProj;
	Matrix  matWV;
	Matrix	matVP;
	Matrix  matWVP;
	Vector3 Pivot;
	int     Animation2DEnable;
	Vector3 MeshSize;
	float   Empty2;
};

struct MaterialCBuffer
{
	Vector4 BaseColor;
	Vector4 EmissiveColor;
	float   Opacity;
	int		PaperBurn;
	int		Distortion;
	int		Empty1;
	Vector2 AddUV;
	Vector2 Empty2;
	Vector4 Tint;
	Vector2 StartUV;
	Vector2 EndUV;
};

struct Animation2DCBuffer
{
	Vector2 StartUV;
	Vector2 EndUV;
	int Animation2DType;
	Vector3 Empty;
};

struct CollisionChannel
{
	std::string	Name;
	Collision_Channel	Channel;
	Collision_Interaction	Interaction;
};

struct CollisionProfile
{
	std::string	Name;
	Collision_Channel	Channel;
	std::vector<CollisionChannel>	vecChannel;
};


struct ColliderCBuffer
{
	Vector4	Color;
};



struct Box2DInfo
{
	Vector2 Center;
	Vector2 Axis[2];
	float   Length[2];
	Vector3	Min;
	Vector3	Max;
	Vector2 Point[4]; // �簢���� �� ��������. ������ Z ���縵 ���� ����.
};

struct LineInfo
{
	Vector2 Start;
	Vector2 End;
};

struct CircleInfo
{
	Vector2	Center;
	float	Radius;
	Vector3	Min;
	Vector3	Max;
};

struct PixelInfo
{
	unsigned char* Pixel;
	size_t		Width;
	size_t		Height;
	PixelCollision_Type	Type;
	unsigned char	Color[4];
	Box2DInfo	Box;
	Vector3	Min;
	Vector3	Max;

	PixelInfo()
	{
		Width = 0;
		Height = 0;
		Pixel = nullptr;
		memset(Color, 0, sizeof(unsigned char) * 4);
	}
};


struct HitResult
{
	class CGameObject* DestObject;
	class CCollider* DestCollider;
	Vector3		HitPoint;
};

struct ParticleCBuffer
{
	Vector3	Pos;
	unsigned int	SpawnCount;
	Vector3	StartScale;
	unsigned int	SpawnCountMax;
	Vector3	EndScale;
	float	LifeTimeMin;
	Vector3	Range;
	float	LifeTimeMax;
	Vector4	StartColor;
	Vector4	EndColor;
	float	MinSpeed;
	float	MaxSpeed;
	int		Move;
	int		Particle2D;
	Vector3	MoveDir;
	float	DefaultZ;
	Vector3	MoveAngle;
	int		Gravity;
};

struct ParticleInfo
{
	Vector3	WorldPos;
	Vector3	Dir;
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
	unsigned int	SpawnCount;
	unsigned int	CurrentCount;

	Vector3	StartScale;
	Vector3	EndScale;

	Vector4	StartColor;
	Vector4	EndColor;

	int	GravityEnable;
};

struct GlobalCBuffer
{
	float	CameraBottom;
	float	DeltaTime;
	float	AccTime;
	float	Empty;
	Vector2	Resolution;
	Vector2	NoiseResolution;
	float	CameraAreaRatio;
	Vector3	Empty2;
	float	Contrast;
	Vector3 Empty3;
};

struct PostProcessCBuffer
{
	int		Blur;
	int		Distortion;
	Vector2	Empty;
};

struct PaperBurnCBuffer
{
	float	Burn;
	Vector3	OutLineColor;
	Vector3	CenterColor;
	int		Inverse;
	Vector3	InLineColor;
	float	InFilter;
	float	CenterFilter;
	float	OutFilter;
	Vector2	Empty;
};

struct Distortion2DCBuffer
{
	float	Weight;
	int		Self;
	float	Speed;
	float	Empty;
};


struct TransformUICBuffer
{
	Matrix  matWVP;
	int	TextureEnable;
	int	Animation2D;
	Vector2	Size;
	Vector2 StartUV;
	Vector2 EndUV;
};

struct UIColorTintCBuffer
{
	Vector4	Tint;
};

struct ProgressBarCBuffer
{
	float	Percent;
	int		Dir;
	Vector2	Empty;
};

struct NumberCBuffer
{
	Vector2	StartUV;
	Vector2	EndUV;
};

struct TileMapCBuffer
{
	Vector2	TileImageSize;
	int		TileStartX;
	int		TileStartY;
	int		TileEndX;
	int		TileEndY;
	Vector2	TileSize;
};

struct CoolTimeCBuffer
{
	float	CoolRatio;
	int		CoolDir; // 1 : �ð����, -1 : �ݽð����
	int		CoolCircle;
	float	Empty;
};

#pragma pack(push, 1)

struct TileInfo
{
	Matrix	matWVP;
	Vector2	FrameStart;
	Vector2	FrameEnd;
	Vector4	Color;
	Vector4	EmvColor;
	float	Opacity;
	Vector3	Empty;
};

#pragma pack(pop)

