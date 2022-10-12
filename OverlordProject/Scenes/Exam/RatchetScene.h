#pragma once
class Ratchet;
class Character;
class Crate;
class Enemy;
class PostVignette;
class PostBrightness;

class RatchetScene final : public GameScene
{
public:
	RatchetScene() :GameScene(L"RatchetScene") 
	{
		//SoundManager::Get()->GetSystem()->createStream("Resources/Audio/Lava.wav", FMOD_DEFAULT, nullptr, &m_pLavaSound);
	}
	~RatchetScene() override;
	RatchetScene(const RatchetScene& other) = delete;
	RatchetScene(RatchetScene&& other) noexcept = delete;
	RatchetScene& operator=(const RatchetScene& other) = delete;
	RatchetScene& operator=(RatchetScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void PostDraw() override;
	void OnGUI() override;
private:

	void EndLevel(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
	void Reset();
	void LoadLevel();
	void LoadCrates();
	void LoadEnemies();
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterAttack
	};
	GameObject* m_SkyBox;
	GameObject* m_Hud;
	Ratchet* m_pCharacter{};
	bool m_DrawShadowMap{ false };
	float m_ShadowMapScale{ 0.3f };
	
	std::vector<Enemy*> m_pEnemies;
	std::vector<Crate*> m_pCrates;

	GameObject* m_pLandingPad;

	PostVignette* m_PostVignette{};
	PostBrightness* m_pPostBrightness{};

	FMOD::Sound* m_pLavaSound = nullptr;
	FMOD::Channel* m_pLavaChannel = nullptr;

	void CreateMenu();
	void RemoveMenu();
	GameObject* m_pMenu{ nullptr };
	bool m_Paused;

	GameObject* m_pLayout;
	bool deletedLayout = false;
	float layoutTimer = 5.f;
	float layoutCounter = 0.f;
	void BrightnessUp();
	void BrightnessDown();

};

