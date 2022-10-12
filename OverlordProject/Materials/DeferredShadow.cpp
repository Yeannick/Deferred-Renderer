#include "stdafx.h"
#include "DeferredShadow.h"

DeferredShadow::DeferredShadow():
	Material(L"Effects/Deferred/ShadowDeferred.fx")
{
}

void DeferredShadow::SetDiffuseMap(const std::wstring& assetFile)
{
	SetDiffuseMap(ContentManager::Load<TextureData>(assetFile));
	
}

void DeferredShadow::SetDiffuseMap(TextureData* pTextureData)
{
	SetVariable_Scalar(L"gUseDiffuseMap", pTextureData != nullptr);
	SetVariable_Texture(L"gDiffuseMap", pTextureData);
}

void DeferredShadow::SetNormalMap(const std::wstring& assetFile)
{
	SetNormalMap(ContentManager::Load<TextureData>(assetFile));

}

void DeferredShadow::SetNormalMap(TextureData* pTextureData)
{
	SetVariable_Scalar(L"gUseNormalMap", pTextureData != nullptr);
	SetVariable_Texture(L"gNormalMap", pTextureData);
}

void DeferredShadow::SetSpecularMap(const std::wstring& assetFile)
{
	SetSpecularMap(ContentManager::Load<TextureData>(assetFile));

}

void DeferredShadow::SetSpecularMap(TextureData* pTextureData)
{
	SetVariable_Scalar(L"gUseSpecularMap", pTextureData != nullptr);
	SetVariable_Texture(L"gSpecularMap", pTextureData);
}

void DeferredShadow::UseTransparency(bool )
{
	//SetTechnique(enable ? L"Default_Alpha" : L"Default");
}

void DeferredShadow::InitializeEffectVariables()
{
}

void DeferredShadow::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	auto model_World = XMLoadFloat4x4(&pModel->GetTransform()->GetWorld());
	auto light_viewProjection = XMLoadFloat4x4(&pShadowMapRenderer->GetLightVP());
	auto lightWVP = model_World * light_viewProjection;
	XMFLOAT4X4 lightWVPloaded{};
	XMStoreFloat4x4(&lightWVPloaded, lightWVP);

	
	
	SetVariable_Matrix(L"gWorldViewProj_Light", lightWVPloaded);
	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
}
