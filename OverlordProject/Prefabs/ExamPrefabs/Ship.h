#pragma once

class Ship :public GameObject
{
public:
	Ship(const XMFLOAT3& Position);
	~Ship() override;

	Ship(const Ship& other) = delete;
	Ship(Ship&& other) noexcept = delete;
	Ship& operator=(const Ship& other) = delete;
	Ship& operator=(Ship&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
};

