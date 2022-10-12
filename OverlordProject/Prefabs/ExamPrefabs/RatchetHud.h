#pragma once
class Ratchet;

class RatchetHud : public GameObject
{
public:
	RatchetHud();
	~RatchetHud() override = default;

	RatchetHud(const RatchetHud& other) = delete;
	RatchetHud(RatchetHud&& other) noexcept = delete;
	RatchetHud& operator=(const RatchetHud& other) = delete;
	RatchetHud& operator=(RatchetHud&& other) noexcept = delete;

	void GetPlayerCharacter(Ratchet* pPlayer);
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void Draw(const SceneContext&) override;

private:
	SpriteFont* m_pFont;
	Ratchet* m_pPlayer;
	std::vector<GameObject*>  m_pHealthHud;
	GameObject* m_pBoltHud;
	int m_Bolts = 0;
	int m_Lives = 0;
};

