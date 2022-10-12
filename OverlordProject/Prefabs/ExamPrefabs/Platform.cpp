#include "stdafx.h"
#include "Platform.h"
#include "Materials/DeferredShadow.h"

Platform::Platform(const XMFLOAT3& Position)
{
	GetTransform()->Translate(Position);
}

Platform::~Platform()
{
}

void Platform::Initialize(const SceneContext&)
{

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	
	const auto pPlatformMaterial = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pPlatformMaterial->SetDiffuseMap(L"Textures/Platform/Platform2.png");
	pPlatformMaterial->SetSpecularMap(L"Textures/Platform/Metallic2.png");
	const auto pPlatformMaterial2 = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pPlatformMaterial2->SetDiffuseMap(L"Textures/Platform/Platform2.png");
	pPlatformMaterial2->SetSpecularMap(L"Textures/Platform/Metallic2.png");
	

	auto pPlatformModel = new ModelComponent(L"Meshes/Platform.ovm");

	AddComponent(pPlatformModel);
	pPlatformModel->SetMaterial(pPlatformMaterial, 0);
	pPlatformModel->SetMaterial(pPlatformMaterial2, 1);

	const auto pPlatformActor = AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMeshPlatform = ContentManager::Load<PxTriangleMesh>(L"Meshes/Platform.ovpt");
	pPlatformActor->AddCollider(PxTriangleMeshGeometry(pTriangleMeshPlatform, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);

	//pPlatform->GetTransform()->Translate();
	
}

void Platform::Update(const SceneContext&)
{
}
