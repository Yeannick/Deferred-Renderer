#include "stdafx.h"
#include "Enemy.h"
#include "Materials/BasicMaterial_Deferred.h"
#include "Prefabs/ExamPrefabs/Ratchet.h"
#include "Prefabs/ExamPrefabs/CrateParticle.h"
#include "Prefabs/ExamPrefabs/BotParticle.h"

Enemy::Enemy(const XMFLOAT3& pos)
{
	GetTransform()->Translate(pos);
	SoundManager::Get()->GetSystem()->createStream("Resources/Audio/BotHit.wav", FMOD_DEFAULT, nullptr, &m_pBotSound);
}
void Enemy::Destroy()
{
	m_Hit = true;
	RemoveComponent(m_pModel,true);
	m_pController->Move(XMFLOAT3{ -100000,-1000000,-1000000 });
	SoundManager::Get()->GetSystem()->playSound(m_pBotSound, nullptr, false, nullptr);
	auto pos = GetTransform()->GetPosition();
	SceneManager::Get()->GetActiveScene()->AddChild(new CrateParticle({ pos.x , pos.y + 1.5f , pos.z }, 1.0f));
}


void Enemy::SetPatrolPoints(std::vector<XMFLOAT3> points)
{
	m_PatrolTargets = points;
}

void Enemy::SetPlayerCharacter(Ratchet* pPlayer)
{
	m_pPlayer = pPlayer;
}

void Enemy::Initialize(const SceneContext&)
{
	SetTag(L"Enemy");
	// Model + material
	m_pModel = AddComponent(new ModelComponent(L"Meshes/Bot.ovm"));
	auto material = MaterialManager::Get()->CreateMaterial<BasicMaterial_Deferred>();
	m_pModel->SetMaterial(material);
	material->SetDiffuseMap(L"Textures/Bot.png");


	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	EnemyDesc BotDesc{ pDefaultMaterial,3.0f,10.f };
	m_pController = new ControllerComponent(BotDesc.controller);
	
	auto rigidBody = new RigidBodyComponent(true);
	rigidBody->AddCollider(PxBoxGeometry{ 5.f,4.f,5.f }, *pDefaultMaterial, true);
	m_pEnemyHitBox = new GameObject();
	m_pEnemyHitBox->AddComponent(rigidBody);

	AddChild(m_pEnemyHitBox);
	m_pEnemyHitBox->SetOnTriggerCallBack(std::bind(&Enemy::KillPlayer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	AddComponent(m_pController);
	m_pParticle = new BotParticle(GetTransform()->GetPosition());
	AddChild(m_pParticle);
	m_State = EnemyState::patrol;


}

void Enemy::Update(const SceneContext& sceneContext )
{
	m_pParticle->GetTransform()->Translate(GetTransform()->GetPosition());
	m_pEnemyHitBox->GetTransform()->Translate(GetTransform()->GetPosition());
	if (m_Hit)
	{
		
		SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
		
		
		
		return;
	}
		

	if (m_pPlayer)
	{
		if (GetDistance(m_pController->GetTransform()->GetPosition(), m_pPlayer->GetTransform()->GetPosition()) <= 50.f)
		{
			if (m_State != EnemyState::Attack)
			{
				m_State = EnemyState::FollowPlayer;
				m_NextTarget = m_pPlayer->GetTransform()->GetPosition();
				m_ReachedTarget = false;
			}
			else if (m_State == EnemyState::Attack)
			{
				m_NextTarget = m_pPlayer->GetTransform()->GetPosition();
			}
			
		}
	}
	switch (m_State)
	{
	case patrol:
		/*m_NextTarget = m_PatrolTargets[m_CurrentIndex];
		if (!m_ReachedTarget)
		{
			Move(sceneContext);
		}
		if (GetDistance(m_pController->GetTransform()->GetPosition(), m_NextTarget) <= 20.f)
		{
			m_ReachedTarget = true;
			if (m_CurrentIndex == m_PatrolTargets.size() - 1)
			{
				m_CurrentIndex = 0;
				m_NextTarget = m_PatrolTargets[m_CurrentIndex];
			}
			else
			{
				m_CurrentIndex++;
				m_NextTarget = m_PatrolTargets[m_CurrentIndex];
			}
		}
		else
		{
			m_ReachedTarget = false;
		}*/
		break;
	case FollowPlayer:
		
		if (!m_ReachedTarget)
		{
			Move(sceneContext);
		}
		if (GetDistance(m_pController->GetTransform()->GetPosition(), m_NextTarget) <= 15.f)
		{
			m_ReachedTarget = true;
			m_State = EnemyState::Attack;
		}
		break;
	case Attack:
		if (!m_IsAttackOnCoolDown)
		{
			m_AttackCounter += sceneContext.pGameTime->GetElapsed();
			m_CanAttack = true;
		}
		
		if (m_AttackCounter >= m_AttackTime)
		{
			m_IsAttackOnCoolDown = true;
			m_AttackCounter = 0.f;
			m_CanAttack = false;
		}
		if (m_CooldownCounter >= m_AttackCooldown)
		{
			m_CooldownCounter = 0.f;
			m_IsAttackOnCoolDown = false;
			m_CanAttack = true;
		}
		else
		{
			m_CooldownCounter += sceneContext.pGameTime->GetElapsed();
		}
		if (!m_ReachedTarget)
		{
			Move(sceneContext);
		}
		if (GetDistance(m_pController->GetTransform()->GetPosition(), m_NextTarget) <= 10.f)
		{
			m_ReachedTarget = true;
			
		}
		else
		{
			m_ReachedTarget = false;
		}

		if (GetDistance(m_pController->GetTransform()->GetPosition(), m_NextTarget) <= 25.f)
		{
			m_ReachedTarget = false;
			m_State = EnemyState::patrol;

		}
		//m_Angle += 60 * sceneContext.pGameTime->GetElapsed();
	//	m_pModel->GetTransform()->Rotate(0, m_Angle, 0, false);
		Attacking(sceneContext);
		break;

	}
	

}

void Enemy::Draw(const SceneContext&)
{
}

void Enemy::Attacking(const SceneContext& sceneContext)
{

	if (!m_IsAttackOnCoolDown && m_CanAttack)
	{
		m_Angle += 60 * sceneContext.pGameTime->GetElapsed();
		m_pModel->GetTransform()->Rotate(0, m_Angle, 0, false);

		if (IsInCollider && m_pCollisionObject)
		{
			if (m_pCollisionObject->GetTag() == L"Player")
			{
				auto pPlayer = static_cast<Ratchet*>(m_pCollisionObject);
				pPlayer->PlayerHit();
				m_IsAttackOnCoolDown = true;
				m_CanAttack = false;
				m_AttackCounter = 0.f;
				m_Angle = 0.0f;
			}
		}
	}
	
}

void Enemy::Move(const SceneContext& sceneContext)
{
	auto currentPosition = GetTransform()->GetPosition();

	XMVECTOR tempTarget = XMLoadFloat3(&m_NextTarget);
	XMVECTOR tempPosition = XMLoadFloat3(&currentPosition);

	auto differenceVec = XMVectorSubtract(tempTarget, tempPosition);

	XMVector3Normalize(differenceVec);
	XMFLOAT3 vec{};

	XMStoreFloat3(&vec, differenceVec);

	vec.x *= m_Speed * sceneContext.pGameTime->GetElapsed();
	vec.y *= m_Speed * sceneContext.pGameTime->GetElapsed();
	vec.z *= m_Speed * sceneContext.pGameTime->GetElapsed();

	m_pController->Move(vec);
	m_TotalVelocity = vec;
}
float Enemy::GetDistance(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMVECTOR vector1 = XMLoadFloat3(&v1);
	XMVECTOR vector2 = XMLoadFloat3(&v2);
	XMVECTOR vectorSub = XMVectorSubtract(vector1, vector2);
	XMVECTOR length = XMVector3Length(vectorSub);

	float distance = 0.0f;
	XMStoreFloat(&distance, length);
	return distance;
}
void Enemy::KillPlayer(GameObject* , GameObject* pOtherObject, PxTriggerAction action)
{
	if (pOtherObject->GetTag() == L"Player" && action == PxTriggerAction::ENTER)
	{
		IsInCollider = true;
		m_pCollisionObject = pOtherObject;
		
	}
	if (pOtherObject->GetTag() == L"Player" && action == PxTriggerAction::LEAVE)
	{
		IsInCollider = false;
		m_pCollisionObject = nullptr;

	}
}
