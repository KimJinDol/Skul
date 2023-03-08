
#include "Share.fx"

/*
int : ����
float : �Ǽ�
float2 ~ 4 : 2, 3, 4���� ����(floatŸ��)
int2 ~ 4 : 2, 3, 4���� ����(intŸ��)
matrix : 4x4 ���
float3x3 : 3x3 ��� floatŸ��
*/


struct VS_INPUT_COLOR
{
	// : �ڿ� Semantic �� �ۼ��Ѵ�.
	// POSITION �������Ϳ� �ִ� ���� �����ڴٴ� �ǹ��̴�.
	// �ڿ��� �������� ��ȣ�� ���� �� �ִ�. POSITION1 �̷������� �ۼ��� �����ϴ�.
	// �Ⱥٿ��� ��쿡�� 0���� �ڵ����� ������ �ȴ�.
	float3  Pos : POSITION;
	float4  Color : COLOR;
};

struct VS_OUTPUT_COLOR
{
	// SV �� ������ System Value �̴�.
	// �� �������� Shader �ڵ忡�� ���Ͽ����θ� ����� �ǰ� ��������� ����� �Ұ����ϴ�.
	// ���� �������� ��ȯ�� ������ SV_POSITION �������Ϳ� �־������ PixelShader�� ���۵Ǳ� ����
	// ȭ����������� ��ȯ�� �Ͼ�� �ȴ�.
	float4  Pos : SV_POSITION;
	float4  Color : COLOR;
	float2	UV	: TEXCOORD;
};

float2 ComputeMaterial2DUV(float2 UV)
{
	float2	ResultUV = UV;

	if (g_MtrlStartUV.x != 0 || g_MtrlStartUV.y != 0 || g_MtrlEndUV.x != 0 || g_MtrlEndUV.y != 0)
	{
		if (UV.x <= 0.f)
			ResultUV.x = g_MtrlStartUV.x;

		else
			ResultUV.x = g_MtrlEndUV.x;

		if (UV.y <= 0.f)
			ResultUV.y = g_MtrlStartUV.y;

		else
			ResultUV.y = g_MtrlEndUV.y;
	}

	ResultUV.x += g_MtrlAddUV.x;
	ResultUV.y += g_MtrlAddUV.y;

	return ResultUV;
}

float2 ComputeAnimation2DUV(float2 UV)
{
	if (g_Animation2DEnable == 0)
		return ComputeMaterial2DUV(UV);

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

VS_OUTPUT_COLOR Standard2DColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;

	float3  Pos = input.Pos - g_vPivot * g_vMeshSize;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.Pos = mul(float4(Pos, 1.f), g_matWVP);
	output.Color = input.Color;

	return output;
}


PS_OUTPUT_SINGLE Standard2DColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	float4	Color = (input.Color * g_vMtrlBaseColor + g_vMtrlEmissiveColor) * (float4)g_Contrast;

	output.Color = Color;

	return output;
}

struct VS_INPUT_UV
{
	// : �ڿ� Semantic �� �ۼ��Ѵ�.
	// POSITION �������Ϳ� �ִ� ���� �����ڴٴ� �ǹ��̴�.
	// �ڿ��� �������� ��ȣ�� ���� �� �ִ�. POSITION1 �̷������� �ۼ��� �����ϴ�.
	// �Ⱥٿ��� ��쿡�� 0���� �ڵ����� ������ �ȴ�.
	float3  Pos : POSITION;
	float2  UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
	// SV �� ������ System Value �̴�.
	// �� �������� Shader �ڵ忡�� ���Ͽ����θ� ����� �ǰ� ��������� ����� �Ұ����ϴ�.
	// ���� �������� ��ȯ�� ������ SV_POSITION �������Ϳ� �־������ PixelShader�� ���۵Ǳ� ����
	// ȭ����������� ��ȯ�� �Ͼ�� �ȴ�.
	float4  Pos : SV_POSITION;
	float2  UV : TEXCOORD;
	float4	ProjPos : POSITION;
};

VS_OUTPUT_UV Standard2DTextureVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

	float3  Pos = input.Pos - g_vPivot * g_vMeshSize;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.UV = ComputeAnimation2DUV(input.UV);

	return output;
}


PS_OUTPUT_SINGLE Standard2DTexturePS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	if (ComputeCameraAreaRatio(input.Pos) == 0)
	{
		output.Color.rgb = 0.f;
		output.Color.a = 1.f;
		return output;
	}

	float4  BaseColor = g_BaseTexture.Sample(g_AnisotropicSmp, input.UV);
	float4	EmissiveColor = g_EmissiveTexture.Sample(g_AnisotropicSmp, input.UV);
	
	float4	result = (float4)0.f;

	result.rgb = ((BaseColor.rgb * g_vMtrlBaseColor.rgb + EmissiveColor.rgb * g_vMtrlEmissiveColor.rgb) + g_MtrlTint) * (float3)g_Contrast;
	result.a = BaseColor.a * g_MtrlOpacity;

	result = PaperBurn2D(result, input.UV);

	result = Distortion(result, input.UV, input.ProjPos);

	output.Color = result;

	return output;
}

VS_OUTPUT_UV BackgroundTextureVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

	float3  Pos = input.Pos - g_vPivot * g_vMeshSize;

	// output.Pos �� x, y, z ���� input.Pos�� x, y, z �� ���� w���� 1.f �� ���� �ȴ�.
	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.UV = ComputeMaterial2DUV(input.UV);

	return output;
}

PS_OUTPUT_SINGLE BackgroundTexturePS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE    output = (PS_OUTPUT_SINGLE)0;

	if (ComputeCameraAreaRatio(input.Pos) == 0)
	{
		output.Color.rgb = 0.f;
		output.Color.a = 1.f;
		return output;
	}

	float4  BaseColor = g_BaseTexture.Sample(g_BackgroundSmp, input.UV);
	float4	EmissiveColor = g_EmissiveTexture.Sample(g_BackgroundSmp, input.UV);

	float4	result = (float4)0.f;

	result.rgb = ((BaseColor.rgb * g_vMtrlBaseColor.rgb + EmissiveColor.rgb * g_vMtrlEmissiveColor.rgb) *(float3)g_Contrast + g_MtrlTint);
	result.a = BaseColor.a * g_MtrlOpacity;

	result = PaperBurn2D(result, input.UV);

	result = Distortion(result, input.UV, input.ProjPos);

	output.Color = result;

	return output;
}