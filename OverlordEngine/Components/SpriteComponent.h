#pragma once

class TextureData;

class SpriteComponent : public BaseComponent
{
public:
	SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot = XMFLOAT2{ 0, 0 }, const XMFLOAT4& color = XMFLOAT4{ Colors::White });
	~SpriteComponent() override = default;
	SpriteComponent(const SpriteComponent& other) = delete;
	SpriteComponent(SpriteComponent&& other) noexcept = delete;
	SpriteComponent& operator=(const SpriteComponent& other) = delete;
	SpriteComponent& operator=(SpriteComponent&& other) noexcept = delete;

	const XMFLOAT2& GetPivot() const { return m_Pivot; }
	const XMFLOAT4& GetColor() const { return m_Color; }
	const XMFLOAT2& GetPosition() const { return m_Position; }
	const XMFLOAT2& GetScale() const { return m_Scale; }

	void SetPivot(const XMFLOAT2& pivot) { m_Pivot = pivot; }
	void SetColor(const XMFLOAT4& color) { m_Color = color; }
	void SetPosition(const XMFLOAT2& pos) { m_Position = pos; }
	void SetScale(const XMFLOAT2& scale) { m_Scale = scale; }
	void SetTexture(const std::wstring& spriteAsset);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& sceneContext) override;

private:
	TextureData* m_pTexture{};
	std::wstring m_SpriteAsset{};
	XMFLOAT2 m_Position{};
	XMFLOAT2 m_Pivot{};
	XMFLOAT4 m_Color{};
	XMFLOAT2 m_Scale{ 1,1 };
};
