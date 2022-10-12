#pragma once
class BotParticle : public GameObject
{
public:
	BotParticle(const XMFLOAT3& position);
	//BotParticle(const XMFLOAT3& position, const std::wstring& assetfile, float lifeTime = 2.f);
	~BotParticle() override = default;

	BotParticle(const BotParticle& other) = delete;
	BotParticle(BotParticle&& other) noexcept = delete;
	BotParticle& operator=(const BotParticle& other) = delete;
	BotParticle& operator=(BotParticle&& other) noexcept = delete;

	
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	std::wstring m_ParticleFile = L"Textures/BotHalo.png";
	bool m_IsActive = false;
};

