#include "Engine_Shader_Defines.hlsl"

/* ������̺�. */
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

matrix			g_BoneMatrices[256];

Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;


/* ���ؽ� ���̴� */
struct VS_IN
{	
	float3		vPosition : POSITION;
    float3		vNormal	  : NORMAL;
	float2		vTexcoord : TEXCOORD0;
    float3      vTangent : TANGENT;
	
    uint4		vBlendIndices : BLENDINDEX;
    float4		vBlendWeights : BLENDWEIGHT;
};


struct VS_OUT
{
    float4		vPosition : SV_POSITION;
    float4		vNormal : NORMAL;
    float3      vTangent : TANGENT;
	float2		vTexcoord : TEXCOORD0;
    float4		vWorldPos : TEXCOORD1;
    float4      vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

    float fW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);
    
    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fW;
	
    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vTangent = In.vTangent;
    Out.vNormal = mul(vNormal, g_WorldMatrix);
    Out.vWorldPos = mul(vPosition, g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
  
	return Out;	
}

/* �ȼ� ���̴� */
struct PS_IN
{
	float4		vPosition : SV_POSITION;
    float4		vNormal : NORMAL;
    float3      vTangent : TANGENT;
	float2		vTexcoord : TEXCOORD0;
    float4		vWorldPos : TEXCOORD1;
    float4      vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT	PS_MAIN_ALPHA(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a < 0.3f)
        discard;

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.0f, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_NONALPHA(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.0f, 0.f, 0.f);

    return Out;
}

technique11 DefaultTechnique
{
    pass Mesh_Alpha
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHA();
    }

    pass Mesh_NonAlpha
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONALPHA();
    }
}




