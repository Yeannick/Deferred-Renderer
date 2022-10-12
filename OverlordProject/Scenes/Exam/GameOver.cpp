#include "stdafx.h"
#include "GameOver.h"
#include "Prefabs/ExamPrefabs/MenuPrefabs/Button.h"

GameOver::~GameOver()
{
}

void GameOver::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	auto pHeader = AddChild(new GameObject());
	pHeader->AddComponent(new SpriteComponent(L"Textures/GameOver.png", XMFLOAT2{ 0.5f, 0.5f }));
	pHeader->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f , 1.f);
	auto pButton = AddChild(new Button(L"Main Menu", XMFLOAT2{ 580.f, 400.f }, XMFLOAT4{ Colors::Yellow }, XMFLOAT2{ 210.f, 60.f }));
	pButton->SetOnClickFunction([&]()
		{
			SceneManager::Get()->SetActiveGameScene(L"Main menu");
		});
	pButton = AddChild(new Button(L"Quit", XMFLOAT2{ 580.f, 500.f }, XMFLOAT4{ Colors::Yellow }, XMFLOAT2{ 210.f, 60.f }));
	pButton->SetOnClickFunction([&]()
		{
			PostQuitMessage(1);
		});
}

void GameOver::Update()
{
}

void GameOver::Draw()
{
}

void GameOver::OnGUI()
{
}
