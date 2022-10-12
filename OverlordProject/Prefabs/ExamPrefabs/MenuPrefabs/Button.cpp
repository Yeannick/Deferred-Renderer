#include "stdafx.h"
#include "Button.h"



Button::Button(std::wstring text, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 bounds):
    m_Text{text},
    m_Position{position},
    m_Color{color},
    m_Bounds{bounds}
{
    m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
}

void Button::SetOnClickFunction(std::function<void()> newClickFunction)
{
    m_OnClickFunction = newClickFunction;
	
}

void Button::Initialize(const SceneContext&)
{
	auto sprite = new SpriteComponent(L"Textures/Button.png", XMFLOAT2{ 0.5f, 0.5f });
	AddComponent(sprite);
	sprite->GetTransform()->Translate(m_Position.x+ 64 ,m_Position.y + 25.f, 1.f);

	auto fmodSys = SoundManager::Get()->GetSystem();
	FMOD_RESULT fmodRes;

	fmodRes = fmodSys->createStream("Resources/Audio/Button.wav", FMOD_DEFAULT, nullptr, &m_pButtonSound);
}


void Button::Draw(const SceneContext& sceneContext)
{
	DebugRenderer::DrawLine(XMFLOAT3{ m_Position.x, m_Position.y, 0.f }, XMFLOAT3{ m_Position.x + m_Bounds.x, m_Position.y ,0 });

	if (!IsOverrlapping(sceneContext))
	{
		TextRenderer::Get()->DrawText(m_pFont, m_Text, m_Position, m_Color);
		return;
	}

	TextRenderer::Get()->DrawText(m_pFont, m_Text, m_Position, XMFLOAT4{ Colors::White });
	if (sceneContext.pInput->IsMouseButton(InputState::pressed, VK_LBUTTON) && m_OnClickFunction != nullptr)
	{
		auto fmodSystem = SoundManager::Get()->GetSystem();
		FMOD_RESULT fmodres;
		fmodres = fmodSystem->playSound(m_pButtonSound, nullptr, false, nullptr);
		m_OnClickFunction();
	}
}
		

void Button::Update(const SceneContext&)
{
}

bool Button::IsOverrlapping(const SceneContext& sceneContext)
{
	auto mousePos = sceneContext.pInput->GetMousePosition();

	if (mousePos.x > m_Position.x - 64 && mousePos.x < m_Position.x + m_Bounds.x
		&& mousePos.y > m_Position.y && mousePos.y < m_Position.y + m_Bounds.y)
	{
		return true;
	}

	return false;
}
