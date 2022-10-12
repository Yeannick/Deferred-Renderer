#include "stdafx.h"
#include "FloatingPlatform.h"
#include "Materials/DeferredShadow.h"

FloatingPlatform::FloatingPlatform(const XMFLOAT3& Position)
{
	GetTransform()->Translate(Position);
	GetTransform()->Rotate(90,0,0,true);
	GetTransform()->Scale(0.3f);
}

FloatingPlatform::~FloatingPlatform()
{
}

void FloatingPlatform::Initialize(const SceneContext&)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	const auto pPlatformMaterial = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pPlatformMaterial->SetDiffuseMap(L"Textures/Platform/Platform3.png");
	pPlatformMaterial->SetSpecularMap(L"Textures/Platform/Metalic.png");
	


	auto pPlatformModel = new ModelComponent(L"Meshes/FloatPlatform.ovm");

	AddComponent(pPlatformModel);
	pPlatformModel->SetMaterial(pPlatformMaterial, 0);
	
	

	const auto pPlatformActor = AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMeshPlatform = ContentManager::Load<PxTriangleMesh>(L"Meshes/FloatPlatform.ovpt");
	pPlatformActor->AddCollider(PxTriangleMeshGeometry(pTriangleMeshPlatform, PxMeshScale({ .3f,.3f,.3f })), *pDefaultMaterial);

}

void FloatingPlatform::Update(const SceneContext&)
{
}
