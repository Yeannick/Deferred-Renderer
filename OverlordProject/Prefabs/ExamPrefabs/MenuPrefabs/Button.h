#pragma once
class Button : public GameObject
{
public:
	Button(std::wstring text,XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 bounds);
	~Button() override = default;

	Button(const Button & other) = delete;
	Button(Button && other) noexcept = delete;
	Button& operator=(const Button & other) = delete;
	Button& operator=(Button && other) noexcept = delete;

	void SetOnClickFunction(std::function<void()> newClickFunction);
protected:
	void Initialize(const SceneContext&) override;
	void Draw(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	bool IsOverrlapping(const SceneContext&);

	std::function<void()> m_OnClickFunction{};
	SpriteFont* m_pFont{};

	std::wstring m_Text{};
	XMFLOAT2 m_Position{};
	XMFLOAT4 m_Color{};
	XMFLOAT2 m_Bounds{};

	FMOD::Sound* m_pButtonSound = nullptr;
	FMOD::Channel* m_pButtonChannel = nullptr;
};

