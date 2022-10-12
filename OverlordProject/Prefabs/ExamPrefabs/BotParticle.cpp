#include "stdafx.h"
#include "BotParticle.h"

BotParticle::BotParticle(const XMFLOAT3& position)
{
	GetTransform()->Translate(position);
}

void BotParticle::Initialize(const SceneContext&)
{
	ParticleEmitterSettings setting;
	setting.velocity = { 0.f,-15.f,0.f };
	setting.minSize = 1.f;
	setting.maxSize = 2.f;
	setting.minEnergy = 3.f;
	setting.maxEnergy = 10.5f;
	setting.minScale = 1.0f;
	setting.maxScale = 1.20f;
	setting.minEmitterRadius = 0.f;
	setting.maxEmitterRadius = 0.1f;
	setting.color = { 1.f,1.f,1.f,1.f };

	AddComponent(new ParticleEmitterComponent(m_ParticleFile, setting, 200));
}

void BotParticle::Update(const SceneContext&)
{
}
