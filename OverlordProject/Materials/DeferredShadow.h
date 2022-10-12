#pragma once
class DeferredShadow : public Material<DeferredShadow>
{
public:
	DeferredShadow();
	~DeferredShadow() override = default;

	DeferredShadow(const DeferredShadow& other) = delete;
	DeferredShadow(DeferredShadow&& other) noexcept = delete;
	DeferredShadow& operator=(const DeferredShadow& other) = delete;
	DeferredShadow& operator=(DeferredShadow&& other) noexcept = delete;

	void SetDiffuseMap(const std::wstring& assetFile);
	void SetDiffuseMap(TextureData* pTextureData);

	void SetNormalMap(const std::wstring& assetFile);
	void SetNormalMap(TextureData* pTextureData);

	void SetSpecularMap(const std::wstring& assetFile);
	void SetSpecularMap(TextureData* pTextureData);

	void UseTransparency(bool enable);

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const;
};


