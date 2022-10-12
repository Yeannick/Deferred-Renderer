#include "stdafx.h"
#include "MainMenu.h"

#include "Materials\ColorMaterial.h"
#include "Prefabs\ExamPrefabs\MenuPrefabs\Button.h"
#include "Scenes/Exam/RatchetScene.h"


MainMenu::~MainMenu()
{
	auto fmodSys = SoundManager::Get()->GetSystem();
	FMOD_RESULT fmodRes;
	fmodRes = fmodSys->close();

}

void MainMenu::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;
	m_SceneContext.settings.drawPhysXDebug = false;

	auto fmodSys = SoundManager::Get()->GetSystem();
	FMOD_RESULT fmodRes;
	m_pMainMenuSound->setLoopCount(-1);
	fmodRes = fmodSys->createStream("Resources/Audio/MainMenu.wav", FMOD_LOOP_NORMAL, nullptr, &m_pMainMenuSound);

	auto pHeader = AddChild(new GameObject());
	pHeader->AddComponent(new SpriteComponent(L"Textures/Menu.png", XMFLOAT2{ 0.5f, 0.5f }));
	pHeader->GetTransform()->Translate(m_SceneContext.windowWidth/2.f, m_SceneContext.windowHeight / 2.f , 1.f);
	

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");

	auto pButton = AddChild(new Button(L"Start", XMFLOAT2{ 580.f, 410.f }, XMFLOAT4{ Colors::Yellow }, XMFLOAT2{ 210.f,60.f }));
	pButton->SetOnClickFunction([]()
		{
			SceneManager::Get()->NextScene();
			/*auto ratchetscene = new RatchetScene();
			SceneManager::Get()->AddGameScene(ratchetscene);
			SceneManager::Get()->SetActiveGameScene(L"RatchetScene");*/
			
			
		});
	pButton = AddChild(new Button(L"Options", XMFLOAT2{ 580.f, 480.f }, XMFLOAT4{ Colors::Yellow }, XMFLOAT2{ 210.f, 60.f }));
	pButton = AddChild(new Button(L"Quit", XMFLOAT2{ 580.f, 550.f }, XMFLOAT4{ Colors::Yellow }, XMFLOAT2{ 210.f, 60.f }));
	pButton->SetOnClickFunction([&]()
		{
			PostQuitMessage(0);
		});

	
	fmodRes = fmodSys->playSound(m_pMainMenuSound, nullptr, false, nullptr);
	
}

void MainMenu::Update()
{
	

}
void MainMenu::Draw()
{
}

void MainMenu::OnGUI()
{
}
