
#include "Share.fx"

cbuffer UITransformCBuffer : register(b10)
{
	matrix	g_matUIWVP;
	int		g_UITextureEnable;
	int		g_UIAnimation2DEnable;
	float2	g_UISize;
	float2	g_UIStartUV;
	float2	g_UIEndUV;
};

cbuffer UIColorTintCBuffer : register(b11)
{
	float4	g_UIColorTint;
};

cbuffer ProgressBarCBuffer : register(b12)
{
	float	g_ProgressPercent;
	int		g_ProgressDir;
	float2	g_ProgressEmpty;
};

cbuffer UINumberCBuffer : register(b13)
{
	float2	g_NumberStartUV;
	float2	g_NumberEndUV;
};

cbuffer UICoolTimeCBuffer : register(b12)
{
	float	g_CoolRatio;
	int		g_CoolDir; // 시계 방향, 반시계 방향 선택 : 1은 시계 방항, -1은 반시계 방향
	int		g_CoolCircle;
	float	g_CoolEmpty;
};


struct VS_INPUT_UI
{
	float3	Pos : POSITION;
	float2	UV	: TEXCOORD;
};

struct VS_OUTPUT_UI
{
	float4	Pos : SV_POSITION;
	float2	UV : TEXCOORD;
};

float2 ComputeUIImage2DUV(float2 UV)
{
	float2	ResultUV = UV;

	if (g_UIStartUV.x != 0 || g_UIStartUV.y != 0 || g_UIEndUV.x != 0 || g_UIEndUV.y != 0)
	{
		if (UV.x <= 0.f)
			ResultUV.x = g_UIStartUV.x;

		else
			ResultUV.x = g_UIEndUV.x;

		if (UV.y <= 0.f)
			ResultUV.y = g_UIStartUV.y;

		else
			ResultUV.y = g_UIEndUV.y;
	}

	return ResultUV;
}

float2 ComputeUIAnimation2DUV(float2 UV)
{
	if (g_UIAnimation2DEnable == 0)
		return ComputeUIImage2DUV(UV);

	if (g_Animation2DType == Animation2DFrame ||
		g_Animation2DType == Animation2DArray)
		return UV;

	float2	ResultUV;

	if (UV.x <= 0.f)
		ResultUV.x = g_vAnimation2DStartUV.x;

	else
		ResultUV.x = g_vAnimation2DEndUV.x;

	if (UV.y <= 0.f)
		ResultUV.y = g_vAnimation2DStartUV.y;

	else
		ResultUV.y = g_vAnimation2DEndUV.y;

	return ResultUV;
}

VS_OUTPUT_UI UIMainVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	output.Pos = mul(float4(input.Pos, 1.f), g_matUIWVP);
	output.UV = ComputeUIAnimation2DUV(input.UV);

	return output;
}

VS_OUTPUT_UI UIMouseVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	float3	Pos = input.Pos;
	
	Pos.y = input.Pos.y - 1.f;

	output.Pos = mul(float4(Pos, 1.f), g_matUIWVP);
	output.UV = ComputeUIAnimation2DUV(input.UV);

	return output;
}

PS_OUTPUT_SINGLE UICoolTimePS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	float2 UV = input.UV - 0.5f; // 센터를 0.5 0.5 기준으로 해야하기때문에 뺴줌
	float Radian = atan2(UV.y * g_CoolDir * -1.f, UV.x);
	float Degree = Radian * 180.f / 3.14159f - 90.f;

	if (Degree < 0)
		Degree = 360.f + Degree;

	float CoolDegree = 360.f * g_CoolRatio;

	if (g_UITextureEnable == 1 || g_UIAnimation2DEnable == 1)
		output.Color = (g_BaseTexture.Sample(g_AnisotropicSmp, input.UV) * g_UIColorTint) * (float4)g_Contrast;

	else
		output.Color = g_UIColorTint;

	if (g_CoolCircle == 1 && sqrt(UV.x * UV.x + UV.y * UV.y) > 0.5f)
		output.Color.a = 0.f;

	if (Degree > CoolDegree)
		output.Color.a = 0.f;
		
	return output;
}

PS_OUTPUT_SINGLE UIImagePS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_UITextureEnable == 1 || g_UIAnimation2DEnable == 1)
		output.Color = (g_BaseTexture.Sample(g_AnisotropicSmp, input.UV) * g_UIColorTint) * (float4)g_Contrast;

	else
		output.Color = g_UIColorTint;

	return output;
}

VS_OUTPUT_UI ProgressBarVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	float3	Pos = input.Pos;

	// 오른쪽 -> 왼쪽
	if (g_ProgressDir == 0)
	{
		if (Pos.x == 1.f)
		{
			Pos.x = g_ProgressPercent;
		}
	}

	// 왼쪽 -> 오른쪽
	if (g_ProgressDir == 1)
	{
		if (Pos.x == 0.f)
		{
			Pos.x = 1.f - g_ProgressPercent;
		}
	}

	// 위 -> 아래
	if (g_ProgressDir == 2)
	{
		if (Pos.y == 1.f)
		{
			Pos.y = g_ProgressPercent;
		}
	}

	// 아래 -> 위
	if (g_ProgressDir == 3)
	{
		if (Pos.y == 0.f)
		{
			Pos.y = 1.f - g_ProgressPercent;
		}
	}

	output.Pos = mul(float4(Pos, 1.f), g_matUIWVP);

	float2	UV = input.UV;

	// 오른쪽 -> 왼쪽
	if (g_ProgressDir == 0)
	{
		if (UV.x == 1.f)
		{
			UV.x = g_ProgressPercent;
		}
	}

	// 왼쪽 -> 오른쪽
	if (g_ProgressDir == 1)
	{
		if (UV.x == 0.f)
		{
			UV.x = 1.f - g_ProgressPercent;
		}
	}

	// 위 -> 아래
	if (g_ProgressDir == 2)
	{
		if (UV.y == 0.f)
		{
			UV.y = 1.f - g_ProgressPercent;
		}
	}

	// 아래 -> 위
	if (g_ProgressDir == 3)
	{
		if (UV.y == 1.f)
		{
			UV.y = g_ProgressPercent;
		}
	}

	output.UV = UV;

	return output;
}


PS_OUTPUT_SINGLE ProgressBarPS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_UITextureEnable == 1)
		output.Color = (g_BaseTexture.Sample(g_AnisotropicSmp, input.UV) * g_UIColorTint) * (float4)g_Contrast;

	else
		output.Color = g_UIColorTint;


	return output;
}

VS_OUTPUT_UI UINumberVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	output.Pos = mul(float4(input.Pos, 1.f), g_matUIWVP);

	if (input.Pos.x == 0.f)
		output.UV.x = g_NumberStartUV.x;

	else
		output.UV.x = g_NumberEndUV.x;

	if (input.Pos.y == 0.f)
		output.UV.y = g_NumberEndUV.y;

	else
		output.UV.y = g_NumberStartUV.y;

	return output;
}


PS_OUTPUT_SINGLE UINumberPS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_UITextureEnable == 1)
		output.Color = (g_BaseTexture.Sample(g_AnisotropicSmp, input.UV) * g_UIColorTint) * (float4)g_Contrast;

	else
		output.Color = g_UIColorTint;

	return output;
}
