#pragma once
class GameOver : public GameScene
{
public:
	GameOver() : GameScene(L"Game Over"){}
	~GameOver();

	GameOver(const GameOver& other) = delete;
	GameOver(GameOver&& other) noexcept = delete;
	GameOver& operator=(const GameOver& other) = delete;
	GameOver& operator=(GameOver&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	GameObject* m_pStartButton{ nullptr };
	GameObject* m_pHeader{ nullptr };

	SpriteFont* m_pFont{};
	FixedCamera* m_pFixedCamera{ nullptr };
};

