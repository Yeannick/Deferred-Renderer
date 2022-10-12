#pragma once
class PostBrightness : public PostProcessingMaterial
{
public:
	PostBrightness();
	~PostBrightness() override = default;
	PostBrightness(const PostBrightness& other) = delete;
	PostBrightness(PostBrightness&& other) noexcept = delete;
	PostBrightness& operator=(const PostBrightness& other) = delete;
	PostBrightness& operator=(PostBrightness&& other) noexcept = delete;

	float GetMultiplier() { return m_Multiplier; }
	void SetMultiplier(float m) { m_Multiplier = m; }
protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
	void UpdateBaseEffectVariables(const SceneContext& sceneContext, RenderTarget* pSource);

private:
	float m_Multiplier = 0.f;
};
