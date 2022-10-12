#pragma once

class MainMenu : public GameScene
{
public:
	MainMenu() : GameScene(L"Main menu") {}
	~MainMenu();

	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) noexcept = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
private:
	GameObject* m_pStartButton{ nullptr };
	GameObject* m_pHeader{ nullptr };

	SpriteFont* m_pFont{};
	//FixedCamera* m_pFixedCamera{ nullptr };
	FMOD::Sound* m_pMainMenuSound = nullptr;
	FMOD::Channel* m_pMainMenuChannel = nullptr;

	
};

