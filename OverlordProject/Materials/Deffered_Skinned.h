#pragma once
class Deffered_Skinned : public Material<Deffered_Skinned>
{
	public:
		Deffered_Skinned();
		~Deffered_Skinned() override = default;

		Deffered_Skinned(const Deffered_Skinned& other) = delete;
		Deffered_Skinned(Deffered_Skinned&& other) noexcept = delete;
		Deffered_Skinned& operator=(const Deffered_Skinned& other) = delete;
		Deffered_Skinned& operator=(Deffered_Skinned&& other) noexcept = delete;

		void SetDiffuseMap(const std::wstring& assetFile);
		void SetDiffuseMap(TextureData* pTextureData);

		/*void SetNormalMap(const std::wstring& assetFile);
		void SetNormalMap(TextureData* pTextureData);
		*/
		void SetSpecularMap(const std::wstring& assetFile);
		void SetSpecularMap(TextureData* pTextureData);
		/*
		void UseTransparency(bool enable);*/

	protected:
		void InitializeEffectVariables() override;
		void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const;
};

