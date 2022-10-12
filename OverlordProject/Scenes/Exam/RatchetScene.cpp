#include "stdafx.h"
#include "RatchetScene.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/ColorMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseAmbient.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/BasicMaterial_Deferred.h"
#include "Materials/DeferredShadow.h"

#include "Prefabs/Character.h"
#include "Prefabs\ExamPrefabs\Ratchet.h"
#include "Prefabs/ExamPrefabs/Crate.h"
#include "Prefabs/ExamPrefabs/CrateParticle.h"
#include "Prefabs/ExamPrefabs/BoltPickUp.h"
#include "Prefabs/ExamPrefabs/Enemy.h"
#include "Materials/SkyBoxMaterial.h"
#include "Prefabs/ExamPrefabs/RatchetHud.h"
#include "Materials/Post/PostBrightness.h"
#include "Materials/Post/PostVignette.h"
#include "Prefabs/ExamPrefabs/MenuPrefabs/Button.h"
#include "Prefabs/ExamPrefabs/Platform.h"
#include "Prefabs/ExamPrefabs/Ship.h"
#include "Prefabs/ExamPrefabs/FloatingPlatform.h"
RatchetScene::~RatchetScene()
{
	
	delete m_SkyBox;
	m_SkyBox = nullptr;
	
}

void RatchetScene::Initialize()
{
	m_SceneContext.useDeferredRendering = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	LoadLevel();
	// Crates
	//-40.f, 10.f, 75.f
	LoadCrates();


	AddChild(new Platform(XMFLOAT3{ -115.f, 0.f, -20.f }));
	AddChild(new Platform(XMFLOAT3{ -180.f, 63.f, -360.f }));
	AddChild(new FloatingPlatform (XMFLOAT3{ -180, 20, -70 }));
	AddChild(new FloatingPlatform (XMFLOAT3{ -120, 35, -90 }));
	AddChild(new FloatingPlatform (XMFLOAT3{ -180, 50, -120 }));


	AddChild(new Ship(XMFLOAT3{ -180.f, 77.f, -420.f }));
	
	auto TriggerEnd = new GameObject();
	auto rigidBody = new RigidBodyComponent(true);
	rigidBody->AddCollider(PxBoxGeometry{ 30.f,30.f,30.f }, *pDefaultMaterial, true);
	TriggerEnd->SetOnTriggerCallBack(std::bind(&RatchetScene::EndLevel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	TriggerEnd->AddComponent(rigidBody);
	AddChild(TriggerEnd);
	TriggerEnd->GetTransform()->Translate(-180.f, 70.f, -420.f);

	m_pLandingPad = new GameObject();
	auto pModel = new ModelComponent(L"Meshes/Landing_Pad.ovm"); 
	auto pMaterial = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pMaterial->SetDiffuseMap(L"Textures/LandingPad.png");
	pModel->SetMaterial(pMaterial);
	
	const auto pPadActor = m_pLandingPad->AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMeshPad = ContentManager::Load<PxTriangleMesh>(L"Meshes/Landing_Pad.ovpt");
	pPadActor->AddCollider(PxTriangleMeshGeometry(pTriangleMeshPad, PxMeshScale({ 40.f,40.f,40.f })), *pDefaultMaterial);
	m_pLandingPad->AddComponent(pModel);
	
	m_pLandingPad->GetTransform()->Translate(-180.f, 70.f, -420.f);
	m_pLandingPad->GetTransform()->Rotate(90, 0, 0, true);
	m_pLandingPad->GetTransform()->Scale(40.f);
	AddChild(m_pLandingPad);
	m_SkyBox = new GameObject();

	const auto pSkyBoxMaterial = MaterialManager::Get()->CreateMaterial<SkyBoxMaterial>();
	pSkyBoxMaterial->SetSkyBoxTexture(L"Textures/SkyBox2.dds");

	m_SkyBox->AddComponent(new ModelComponent(L"Meshes/Box.ovm"));
	m_SkyBox->GetComponent<ModelComponent>()->SetMaterial(pSkyBoxMaterial);
	m_SkyBox->GetTransform()->Rotate(0.f, 0.f, 90.f);
	AddChild(m_SkyBox);

	RatchetDesc characterDesc{ pDefaultMaterial,2.5f,10.f };
	characterDesc.action_id_MoveForward = CharacterMoveForward;
	characterDesc.action_id_MoveBackward = CharacterMoveBackward;
	characterDesc.action_id_MoveLeft = CharacterMoveLeft;
	characterDesc.action_id_MoveRight = CharacterMoveRight;
	characterDesc.action_id_Jump = CharacterJump;
	characterDesc.action_id_Attack = CharacterAttack;

	m_pCharacter = AddChild(new Ratchet(characterDesc));
	m_pCharacter->GetTransform()->Translate(80.f, 20.f, 0.f);
	//m_pCharacter->GetTransform()->Scale(.f);
	//m_pCharacter->GetTransform()->Rotate(0.f,0.f,90.f);

	RatchetHud* pHud = new RatchetHud();

	pHud->GetPlayerCharacter(m_pCharacter);
	//pHud->GetTransform()->Translate(0, 0, 0.9f);

	AddChild(pHud);
	
	LoadEnemies();

	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterAttack, InputState::pressed, 'Q');
	m_SceneContext.pInput->AddInputAction(inputAction);
	
	//Post Processing Stack
	//=====================
	m_PostVignette = MaterialManager::Get()->CreateMaterial<PostVignette>();

	m_pPostBrightness = MaterialManager::Get()->CreateMaterial<PostBrightness>();
	//AddPostProcessingEffect(m_PostVignette);
	AddPostProcessingEffect(m_pPostBrightness);

	m_pLayout = new GameObject();
	m_pLayout->AddComponent(new SpriteComponent(L"Textures/Layout.png", XMFLOAT2{ 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	m_pLayout->GetTransform()->Translate(m_SceneContext.windowWidth / 2, m_SceneContext.windowHeight / 2, 0.9f);
	AddChild(m_pLayout);
}

void RatchetScene::Update()
{
	layoutCounter += m_SceneContext.pGameTime->GetElapsed();
	if (layoutCounter >= layoutTimer)
	{
		if (!deletedLayout)
		{
			RemoveChild(m_pLayout, true);
			deletedLayout = true;
		}
		
		
	}
	for (int i = 0; i < m_SceneContext.pLights->GetLights().size(); i++)
	{
		m_SceneContext.pLights->GetLight(i).intensity = MathHelper::randF(2.5f, 3.0f);
		auto& dirLight = m_SceneContext.pLights->GetDirectionalLight();
		auto transform = m_pCharacter->GetTransform()->GetPosition();
		dirLight.position = {transform.x + 100 , transform.y+ 100 , transform.z , 1.f} ;
	}

	if (m_pCharacter->GetTransform()->GetPosition().y < -15.f)
	{
		m_pCharacter->PlayerHit();
		m_pCharacter->Respawn();
	}
	
	if (GetSceneContext().pInput->IsKeyboardKey(InputState::pressed, VK_ESCAPE))
	{
		if (m_Paused)
		{
			RemoveMenu();
			m_Paused = false;
			m_SceneContext.pGameTime->Start();
		}
		else
		{
			CreateMenu();
			m_Paused = true;
			m_SceneContext.pGameTime->Stop();
		}
		if (!m_Paused && m_pMenu != nullptr)
		{
			RemoveMenu();
		}
	}

	
		
}

void RatchetScene::PostDraw()
{
	if (m_DrawShadowMap) {

		ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { m_ShadowMapScale, m_ShadowMapScale }, { 1.f,0.f });
	}
}

void RatchetScene::OnGUI()
{
	ImGui::Checkbox("Draw ShadowMap", &m_DrawShadowMap);
	ImGui::SliderFloat("ShadowMap Scale", &m_ShadowMapScale, 0.f, 1.f);
	MaterialManager::Get()->GetMaterial(6)->DrawImGui();

	/*bool isEnabled = m_pPostBrightness->IsEnabled();
	ImGui::Checkbox("Brightness PP", &isEnabled);
	m_pPostBrightness->SetIsEnabled(isEnabled);

	float multiplier = m_pPostBrightness->GetMultiplier();
	ImGui::SliderFloat("Brightness Multiplier", &multiplier, 0.f, 1.f, "%.001f", 1.f);
	m_pPostBrightness->SetMultiplier(multiplier);*/
	DeferredRenderer::Get()->DrawImGui();
	
}

void RatchetScene::EndLevel(GameObject* , GameObject* pOtherObject, PxTriggerAction action)
{
	if (pOtherObject->GetTag() == L"Player" && action== PxTriggerAction::ENTER)
	{
		
		SceneManager::Get()->SetActiveGameScene(L"Game Over");
		Reset();
		return;
	}
	

	
}

void RatchetScene::Reset()
{
	m_pCharacter->Reset();
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		if (m_pEnemies[i])
		{
			if (!m_pEnemies[i]->IsHit())
			{
				m_pEnemies[i]->Destroy();
			}
			
		
			//SceneManager::Get()->GetActiveScene()->RemoveChild(m_pEnemies[i], true);
		}
	}
	LoadEnemies();
	
	for (size_t i = 0; i < m_pCrates.size(); i++)
	{
		if (m_pCrates[i])
		{
			if (!m_pCrates[i]->IsHit())
			{
				m_pCrates[i]->Destroy();
			}
		
			//SceneManager::Get()->GetActiveScene()->RemoveChild(m_pCrates[i], true);
		}
	}
	LoadCrates();
	
}

void RatchetScene::LoadLevel()
{
	auto& dirLight = m_SceneContext.pLights->GetDirectionalLight();
	dirLight.isEnabled = true;
	dirLight.intensity = 0.5f;
	dirLight.direction = { -1.f, -1.f, 0.0f , 1.0f };
	dirLight.position = { 100.f , 100.f , 0.f,1.f };

	Light light = {};
	//Point Light
	light = {};
	light.isEnabled = true;
	light.position = { -40.f,10.f,75.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 1.f;
	light.range = 60.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { -90.f,10.f,-15.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 2.f;
	light.range = 80.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { 70.f,10.f,-60.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 3.f;
	light.range = 120.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { -290.f,10.f,75.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 1.f;
	light.range = 60.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { -340.f,10.f,-15.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 2.f;
	light.range = 80.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { -180.f,10.f,-60.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 3.f;
	light.range = 120.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { 300.f,10.f,0.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 5.f;
	light.range = 200.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { -160.f,70.f,-200.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 2.f;
	light.range = 80.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);
	
	light = {};
	light.isEnabled = true;
	light.position = { -180.f,70.f,-330.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 3.f;
	light.range = 120.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

	light = {};
	light.isEnabled = true;
	light.position = { -270.f,70.f,-290.f,1.0f };
	light.color = { 1.f,0.7f,0.f,1.f };
	light.intensity = 5.f;
	light.range = 80.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);
	//// Materials

	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pGroundMaterial->SetDiffuseMap(L"Textures/Lava/Ground.png");
	pGroundMaterial->UseTransparency(false);
	pGroundMaterial->SetSpecularMap(L"Textures/Lava/Specular.png");
	const auto pLavaMaterial = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pLavaMaterial->SetDiffuseMap(L"Textures/Lava/Lava.png");
	pLavaMaterial->SetSpecularMap(L"Textures/Lava/Specular.png");


	const auto pRock1Material = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pRock1Material->SetDiffuseMap(L"Textures/Lava/Rock1.png");
	pRock1Material->SetSpecularMap(L"Textures/Lava/Specular.png");

	const auto pRock2Material = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pRock2Material->SetDiffuseMap(L"Textures/Lava/Rock2.png");
	pRock2Material->SetSpecularMap(L"Textures/Lava/Specular.png");
	const auto pRock3Material = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pRock3Material->SetDiffuseMap(L"Textures/Lava/Rock3.png");
	pRock3Material->SetSpecularMap(L"Textures/Lava/Specular.png");
	const auto pRock4Material = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pRock4Material->SetDiffuseMap(L"Textures/Lava/Rock4.png");
	pRock4Material->SetSpecularMap(L"Textures/Lava/Specular.png");

	const auto pLavaFloorMaterial = MaterialManager::Get()->CreateMaterial<DiffuseAmbient>();
	pLavaFloorMaterial->SetDiffuseTexture(L"Textures/Lava/LavaTile.png");
	//pLavaFloorMaterial->SetSpecularMap(L"Textures/Lava/Specular.png");
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	// Level Models

	const auto pIsland = new GameObject();
	const auto pIslandModel = new ModelComponent(L"Meshes/Lava1.ovm");
	pIslandModel->SetMaterial(pGroundMaterial, 0);
	pIslandModel->SetMaterial(pLavaMaterial, 1);
	pIslandModel->SetMaterial(pRock1Material, 4);
	pIslandModel->SetMaterial(pRock2Material, 5);
	pIslandModel->SetMaterial(pRock3Material, 3);
	pIslandModel->SetMaterial(pRock4Material, 2);
	pIslandModel->SetMaterial(pLavaMaterial, 6);

	const auto pIslandActor = pIsland->AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMeshIsland = ContentManager::Load<PxTriangleMesh>(L"Meshes/Lava.ovpt");
	pIslandActor->AddCollider(PxTriangleMeshGeometry(pTriangleMeshIsland, PxMeshScale({ 10.f,10.f,10.f })), *pDefaultMaterial);

	pIsland->AddComponent(pIslandModel);
	//pIsland->GetTransform()->Rotate(, 0, 0);
	pIsland->GetTransform()->Scale(10.f);
	AddChild(pIsland);

	const auto pIsland2 = new GameObject();
	const auto pIslandModel2 = new ModelComponent(L"Meshes/Lava1.ovm");
	pIslandModel2->SetMaterial(pGroundMaterial, 0);
	pIslandModel2->SetMaterial(pLavaMaterial, 1);
	pIslandModel2->SetMaterial(pRock1Material, 4);
	pIslandModel2->SetMaterial(pRock2Material, 5);
	pIslandModel2->SetMaterial(pRock3Material, 3);
	pIslandModel2->SetMaterial(pRock4Material, 2);
	pIslandModel2->SetMaterial(pLavaMaterial, 6);

	const auto pIslandActor2 = pIsland2->AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMeshIsland2 = ContentManager::Load<PxTriangleMesh>(L"Meshes/Lava.ovpt");
	pIslandActor2->AddCollider(PxTriangleMeshGeometry(pTriangleMeshIsland2, PxMeshScale({ 10.f,10.f,10.f })), *pDefaultMaterial);

	pIsland2->AddComponent(pIslandModel2);
	pIsland2->GetTransform()->Translate(-250, 0, 0);
	pIsland2->GetTransform()->Scale(10.f);
	AddChild(pIsland2);

	const auto pIsland3 = new GameObject();
	const auto pIslandModel3 = new ModelComponent(L"Meshes/Lava1.ovm");
	pIslandModel3->SetMaterial(pGroundMaterial, 0);
	pIslandModel3->SetMaterial(pLavaMaterial, 1);
	pIslandModel3->SetMaterial(pRock1Material, 4);
	pIslandModel3->SetMaterial(pRock2Material, 5);
	pIslandModel3->SetMaterial(pRock3Material, 3);
	pIslandModel3->SetMaterial(pRock4Material, 2);
	pIslandModel3->SetMaterial(pLavaMaterial, 6);

	const auto pIslandActor3 = pIsland3->AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMeshIsland3 = ContentManager::Load<PxTriangleMesh>(L"Meshes/Lava.ovpt");
	pIslandActor3->AddCollider(PxTriangleMeshGeometry(pTriangleMeshIsland3, PxMeshScale({ 10.f,10.f,10.f })), *pDefaultMaterial);

	pIsland3->AddComponent(pIslandModel3);
	pIsland3->GetTransform()->Translate(-200, 60, -250);
	pIsland3->GetTransform()->Rotate(0, 270, 0);
	pIsland3->GetTransform()->Scale(10.f);
	AddChild(pIsland3);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			const auto pLavaFloor = new GameObject();
			const auto pFloorModel = new ModelComponent(L"Meshes/LavaFloor.ovm");
			pFloorModel->SetMaterial(pLavaFloorMaterial);
			pLavaFloor->AddComponent(pFloorModel);
			pLavaFloor->GetTransform()->Scale(0.05f);
			pLavaFloor->GetTransform()->Rotate(90.f, 0.f, 0.f);
			pLavaFloor->GetTransform()->Translate( 500+(i*-100.f), -10.f, 500+(j*-100.f));
			AddChild(pLavaFloor);
		}
	}
	
}

void RatchetScene::LoadCrates()
{
	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -32.f,10.f,75.f }, CrateType::ExplosiveCrate)));
	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -30.f,10.f,65.f }, CrateType::BoltCrate)));
	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -25.f,10.f,50.f }, CrateType::NanoTechCrate)));

	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -282.f,10.f,75.f }, CrateType::ExplosiveCrate)));
	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -280.f,10.f,65.f }, CrateType::BoltCrate)));
	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -275.f,10.f,50.f }, CrateType::NanoTechCrate)));

	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -195.f,68.f,-310.f }, CrateType::ExplosiveCrate)));
	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -180.f,68.f,-300.f }, CrateType::BoltCrate)));
	m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ -165.f,68.f,-320.f }, CrateType::NanoTechCrate)));
}

void RatchetScene::LoadEnemies()
{
	auto pEnemy = new Enemy(XMFLOAT3{ -50 , 20 ,0 });
	m_pEnemies.push_back(AddChild(pEnemy));
	pEnemy->SetPlayerCharacter(m_pCharacter);

	auto pEnemy2 = new Enemy(XMFLOAT3{ -300 , 20 ,-15 });
	m_pEnemies.push_back(AddChild(pEnemy2));
	pEnemy2->SetPlayerCharacter(m_pCharacter);

	auto pEnemy3 = new Enemy(XMFLOAT3{ -250 , 20 ,50 });
	m_pEnemies.push_back(AddChild(pEnemy3));
	pEnemy3->SetPlayerCharacter(m_pCharacter);

	auto pEnemy4 = new Enemy(XMFLOAT3{ -200 , 80 ,-170 });
	m_pEnemies.push_back(AddChild(pEnemy4));
	pEnemy4->SetPlayerCharacter(m_pCharacter);

	auto pEnemy5 = new Enemy(XMFLOAT3{ -220 , 80 ,-200 });
	m_pEnemies.push_back(AddChild(pEnemy5));
	pEnemy5->SetPlayerCharacter(m_pCharacter);
}

void RatchetScene::CreateMenu()
{
	m_pMenu = AddChild(new GameObject());

	m_pMenu->AddComponent(new SpriteComponent(L"Textures/Panel.png", XMFLOAT2{ 0.5f,0.5f }));
	m_pMenu->GetTransform()->Translate(XMFLOAT3{ m_SceneContext.windowWidth / 2.f ,m_SceneContext.windowHeight / 2.f ,1.f });
	Button* pButton = m_pMenu->AddChild(new Button(L"Main Menu", XMFLOAT2{ (m_SceneContext.windowWidth / 2.f) - 300.f,(m_SceneContext.windowHeight / 2.f) -120.f }, XMFLOAT4{ Colors::Yellow }, { 200.f,50.f }));
	pButton->SetOnClickFunction([&]()
		{
			Reset();
			SceneManager::Get()->SetActiveGameScene(L"Main menu");
		});
	
	pButton = m_pMenu->AddChild(new Button(L"Brightness +", XMFLOAT2{ (m_SceneContext.windowWidth / 2.f) - 300.f,(m_SceneContext.windowHeight / 2.f)-20  }, XMFLOAT4{ Colors::Yellow }, { 200.f,50.f }));
	pButton->SetOnClickFunction([&]()
		{
			BrightnessUp();
		});
	pButton = m_pMenu->AddChild(new Button(L"Brightness -", XMFLOAT2{ (m_SceneContext.windowWidth / 2.f) - 300.f,(m_SceneContext.windowHeight / 2.f) + 80.f }, XMFLOAT4{ Colors::Yellow }, { 200.f,50.f }));
	pButton->SetOnClickFunction([&]()
		{
			BrightnessDown();
		});
	pButton = m_pMenu->AddChild(new Button(L"Quit", XMFLOAT2{ (m_SceneContext.windowWidth / 2.f) - 300.f,(m_SceneContext.windowHeight / 2.f) + 180.f }, XMFLOAT4{ Colors::Yellow }, { 200.f,50.f }));
	pButton->SetOnClickFunction([&]()
		{
			PostQuitMessage(1);
		});
}

void RatchetScene::RemoveMenu()
{
	if (m_pMenu)
	{
		for (auto& child : m_pMenu->GetChildren<GameObject>())
		{
			m_pMenu->RemoveChild(child, true);
		}

		RemoveChild(m_pMenu, true);

		m_pMenu = nullptr;
	}
	
}

void RatchetScene::BrightnessUp()
{
	float bright = m_pPostBrightness->GetMultiplier();
	bright += 0.1f;
	m_pPostBrightness->SetMultiplier(bright);
}

void RatchetScene::BrightnessDown()
{
	float bright = m_pPostBrightness->GetMultiplier();
	bright -= 0.1f;
	m_pPostBrightness->SetMultiplier(bright);
}
