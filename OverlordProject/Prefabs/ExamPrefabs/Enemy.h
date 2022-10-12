#pragma once
class Ratchet;
class BotParticle;
enum EnemyState
{
	patrol ,
	FollowPlayer,
	Attack 
};
struct EnemyDesc
{
	EnemyDesc(PxMaterial* pMaterial, float radius = 0.5f, float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;

	}

	float maxMovementSpeed = 40.f;
	float maxFallingSpeed = 20.f;


	float movementAccelerationTime = 0.3f;
	float fallOffAccelerationTime = 0.3f;

	PxCapsuleControllerDesc controller;

	float rotationSpeed = 60.f;

	float attackTime = 1.5f;

};
class Enemy : public GameObject
{
public:
	Enemy(const XMFLOAT3& position);

	Enemy(const Enemy& other) = delete;
	Enemy(Enemy&& other) noexcept = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy& operator=(Enemy&& other) noexcept = delete;

	void Destroy();
	bool IsHit() { return m_Hit; }
	
	void SetPatrolPoints(std::vector<XMFLOAT3> points);

	void SetPlayerCharacter(Ratchet* pPlayer);
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void Draw(const SceneContext&) override;

private:
	void Attacking(const SceneContext&);
	void Move(const SceneContext&);
	float GetDistance(const XMFLOAT3& v1, const XMFLOAT3& v2);
	void KillPlayer(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);



	std::vector<XMFLOAT3> m_PatrolTargets{};
	int m_CurrentIndex{ 0 };
	XMFLOAT3 m_NextTarget;
	bool m_ReachedTarget = false;

	BotParticle* m_pParticle;
	ModelComponent* m_pModel;
	ControllerComponent* m_pController;
	XMFLOAT3 m_TotalVelocity;

	Ratchet* m_pPlayer;
	EnemyState m_State = EnemyState::patrol;

	GameObject* m_pEnemyHitBox;
	bool IsInCollider = false;
	GameObject* m_pCollisionObject;


	float m_AttackTime = 1.f;
	float m_AttackCounter = 0.0f;
	float m_CooldownCounter = 0.0f;
	float m_AttackCooldown = 2.f;
	bool m_CanAttack = false;
	bool m_IsAttackOnCoolDown = false;
	float m_Speed{ 1.5f };
	float m_Angle = 0.f;

	bool m_Hit{ false };
	FMOD::Sound* m_pBotSound = nullptr;
	FMOD::Channel* m_pBotChannel = nullptr;
};

