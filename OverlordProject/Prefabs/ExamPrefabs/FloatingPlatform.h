#pragma once
class FloatingPlatform : public GameObject
{
public:
	FloatingPlatform(const XMFLOAT3& Position);
	~FloatingPlatform() override;

	FloatingPlatform(const FloatingPlatform& other) = delete;
	FloatingPlatform(FloatingPlatform&& other) noexcept = delete;
	FloatingPlatform& operator=(const FloatingPlatform& other) = delete;
	FloatingPlatform& operator=(FloatingPlatform&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:

};

