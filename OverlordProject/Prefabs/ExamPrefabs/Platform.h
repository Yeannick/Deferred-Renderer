#pragma once
class Platform : public GameObject
{
public:
	Platform(const XMFLOAT3& Position);
	~Platform() override;

	Platform(const Platform& other) = delete;
	Platform(Platform&& other) noexcept = delete;
	Platform& operator=(const Platform& other) = delete;
	Platform& operator=(Platform&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	
};

