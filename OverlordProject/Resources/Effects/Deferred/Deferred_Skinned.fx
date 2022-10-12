//*************************//
// BASIC EFFECT [DEFERRED] //
//*************************//

/*
	- Diffuse Color/Texture
	- Specular Color
	- SpecularLevel Texture (Blinn)
	- Specular Intensity (Shininess)
	- NormalMap Texture
	- Ambient Color
	- Ambient Intensity [0-1]
	- Opacity Texture/Value [0-1]
*/

//GLOBAL MATRICES
//***************
// The World View Projection Matrix
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
// The ViewInverse Matrix - the third row contains the camera position!
float4x4 gViewInverse : VIEWINVERSE;
// The World Matrix
float4x4 gWorld : WORLD;
float4x4 gBones[70];

//Texture2D gDiffuseMap;
//STATES
//******
RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};

//SAMPLER STATES
//**************
SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	//Filter = ANISOTROPIC;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//LIGHT
//*****
float3 gLightDirection:DIRECTION
<
	string UIName = "Light Direction";
	string Object = "TargetLight";
> = float3(0.577f, 0.577f, 0.577f);


bool gUseDiffuseMap
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Bool";
> = false;

float4 gDiffuseColor
<
	string UIName = "Diffuse Color";
	string UIWidget = "Color";
> = float4(1, 1, 1, 1);

Texture2D gDiffuseMap
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Texture";
> ;

//SPECULAR
//********
float4 gSpecularColor
<
	string UIName = "Specular Color";
	string UIWidget = "Color";
> = float4(1, 1, 1, 1);

Texture2D gSpecularMap
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Texture";
> ;

bool gUseSpecularMap
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Bool";
> = false;

int gShininess <
	string UIName = "Shininess";
	string UIWidget = "Slider";
	float UIMin = 1;
	float UIMax = 100;
	float UIStep = 0.1f;
> = 15;

//AMBIENT
//*******
float4 gAmbientColor
<
	string UIName = "Ambient Color";
	string UIWidget = "Color";
> = float4(1.0, 1.0, 1.0, 1.0);

float gAmbientIntensity
<
	string UIName = "Ambient Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
> = 0.3f;

//NORMAL MAPPING
//**************
bool gFlipGreenChannel
<
	string UIName = "Flip Green Channel";
	string UIWidget = "Bool";
> = false;

bool gUseNormalMap
<
	string UIName = "Normal Mapping";
	string UIWidget = "Bool";
> = false;

Texture2D gNormalMap
<
	string UIName = "Normal Texture";
	string UIWidget = "Texture";
> ;

//OPACITY
//***************
float gOpacityLevel <
	string UIName = "Opacity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
> = 1.0f;

//VS IN & OUT
//***********
struct VS_Input
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float3 Tangent: TANGENT;
	float3 Binormal: BINORMAL;
	float2 texCoord : TEXCOORD;
	float4 blendWeight : BLENDWEIGHTS;
	float4 blendIndex : BLENDINDICES;
};

struct VS_Output
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 Tangent: TANGENT;
	float3 Binormal: BINORMAL;
	float2 texCoord : TEXCOORD;
};

struct PS_Output
{
	float4 LightAccumulation : SV_TARGET0;
	float4 Diffuse : SV_TARGET1;
	float4 Specular : SV_TARGET2;
	float4 Normal : SV_TARGET3;
};

// The main vertex shader
VS_Output MainVS(VS_Input input) {

	VS_Output output;

	float4 originalPosition = float4(input.pos, 1);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	for (int i = 0; i < 4; ++i)
	{
		float boneIndex = input.blendIndex[i];
		if (boneIndex > -1)
		{
			transformedPosition += mul(float4(input.pos, 1.0) * input.blendWeight[i], (float4x4) gBones[boneIndex]);
			transformedNormal += mul(input.normal * input.blendWeight[i], (float3x3) gBones[boneIndex]);
			transformedPosition.w = 1;

		}
	}
	output.pos = mul(transformedPosition, gWorldViewProj); //Non skinned position
	output.normal = normalize(mul(transformedNormal, (float3x3) gWorld));
	output.Tangent = normalize(mul(input.Tangent, (float3x3)gWorld));
	output.Binormal = normalize(mul(input.Binormal, (float3x3)gWorld));
	output.texCoord = input.texCoord;
	return output;
}

// The main pixel shader
PS_Output MainPS(VS_Output input){

	PS_Output output = (PS_Output)0;

	//Fill GBuffer
	// DIFFUSE

	float4 diffuse = gDiffuseColor;
	if (gUseDiffuseMap)
	{
		diffuse *= gDiffuseMap.Sample(gTextureSampler, input.texCoord);
	}
	output.Diffuse = diffuse;

	// ALPHA
	float alpha = diffuse.a * gOpacityLevel;
	clip(alpha - 0.1f);

	// AMBIENT
	float4 ambient = gAmbientColor;
	ambient *= diffuse;
	ambient *= gAmbientIntensity;

	// set Ambient (Light_Accumulation)
	output.LightAccumulation = ambient;

	// NORMAL

	float3 normal = input.normal;
	if (gUseNormalMap)
	{
		float3x3 TBN = float3x3(
			normalize(input.Tangent),
			normalize(input.Binormal),
			normalize(input.normal)
			);
		normal = gNormalMap.Sample(gTextureSampler, input.texCoord).xyz;
		normal = normal * 2.0f - 1.f;
		normal = mul(normal, TBN);
	}
	output.Normal = float4(normal, 0.f);

	// SPECULAR

	float3 specular = gSpecularColor.rgb;
	if (gUseSpecularMap)
	{
		specular *= gSpecularMap.Sample(gTextureSampler, input.texCoord).rgb;
	}
	float shininess = log2(gShininess) / 10.5f;
	// set specular
	output.Specular = float4 (specular, shininess);
	return output;
}

// Default Technique
technique10 Default {
	pass p0 {
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(NoCulling);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}