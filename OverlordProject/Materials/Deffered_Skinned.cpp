#include "stdafx.h"
#include "Deffered_Skinned.h"

Deffered_Skinned::Deffered_Skinned() :
	Material(L"Effects/Deferred/Deferred_Skinned.fx")

{
}

void Deffered_Skinned::SetDiffuseMap(const std::wstring& assetFile)
{
	SetDiffuseMap(ContentManager::Load<TextureData>(assetFile));
}

void Deffered_Skinned::SetDiffuseMap(TextureData* pTextureData)
{
	SetVariable_Scalar(L"gUseDiffuseMap", pTextureData != nullptr);
	SetVariable_Texture(L"gDiffuseMap", pTextureData);
}

void Deffered_Skinned::SetSpecularMap(const std::wstring& assetFile)
{
	SetSpecularMap(ContentManager::Load<TextureData>(assetFile));
}

void Deffered_Skinned::SetSpecularMap(TextureData* pTextureData)
{
	SetVariable_Scalar(L"gUseSpecularMap", pTextureData != nullptr);
	SetVariable_Texture(L"gSpecularMap", pTextureData);
}

void Deffered_Skinned::InitializeEffectVariables()
{
}

void Deffered_Skinned::OnUpdateModelVariables(const SceneContext&, const ModelComponent* pModel) const
{
	auto animator = pModel->GetAnimator();
	ASSERT_NULL_(animator);
	auto boneTransforms = animator->GetBoneTransforms();

	SetVariable_MatrixArray(L"gBones", &boneTransforms[0]._11, UINT(boneTransforms.size()));
}
