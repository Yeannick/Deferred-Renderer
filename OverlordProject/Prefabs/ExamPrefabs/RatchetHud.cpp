#include "stdafx.h"
#include "RatchetHud.h"
#include "Prefabs/ExamPrefabs/Ratchet.h"

RatchetHud::RatchetHud()
{
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
}

void RatchetHud::GetPlayerCharacter(Ratchet* pPlayer)
{
	m_pPlayer = pPlayer;

	
}

void RatchetHud::Initialize(const SceneContext& sceneContext)
{
	m_pBoltHud = new GameObject();
	auto spriteComp = new SpriteComponent(L"Textures/BoltUI.png", XMFLOAT2{ 0.5f,0.5f }, { 1.f,1.f,1.f,1.f });
	m_pBoltHud->AddComponent(spriteComp);
	m_pBoltHud->GetTransform()->Translate(sceneContext.windowWidth - 175.f, 30.f, 1.f);
	m_pBoltHud->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	
	AddChild(m_pBoltHud);

	for (int i = 0; i < 5; i++)
	{
		auto go = new GameObject();
		auto spriteCompHealth = new SpriteComponent(L"Textures/Health.png", XMFLOAT2{ 0.5f,0.5f }, { 1.f,1.f,1.f,1.f });
		go->AddComponent(spriteCompHealth);
		go->GetTransform()->Translate(50.f + i*40.f, 30.f, 1.f);
		go->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
		m_pHealthHud.push_back(go);
		AddChild(go);

	}
	

}

void RatchetHud::Update(const SceneContext& )
{
	m_Bolts = m_pPlayer->GetBolts();
	m_Lives = m_pPlayer->GetLives();

	if (m_Lives != m_pHealthHud.size())
	{
		if (m_Lives >= 0)
		{
			RemoveChild(m_pHealthHud.at(m_pHealthHud.size() - 1), true);
			m_pHealthHud.pop_back();
		}
	
	}
}
void RatchetHud::Draw(const SceneContext& sceneContext)
{
	TextRenderer::Get()->DrawText(m_pFont, std::to_wstring(m_Bolts), XMFLOAT2{ sceneContext.windowWidth - 120.f, 20.f}, XMFLOAT4{Colors::White});
	//TextRenderer::Get()->DrawText(m_pFont, std::to_wstring(m_Lives), XMFLOAT2{ sceneContext.windowWidth - 100.f, 10.f }, XMFLOAT4{ Colors::Orange });
}