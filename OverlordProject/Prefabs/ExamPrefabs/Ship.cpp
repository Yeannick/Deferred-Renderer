#include "stdafx.h"
#include "Ship.h"
#include "Materials/DeferredShadow.h"
Ship::Ship(const XMFLOAT3& Position)
{
	GetTransform()->Translate(Position);
	GetTransform()->Rotate(0,90,0,true);
}

Ship::~Ship()
{
}

void Ship::Initialize(const SceneContext&)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Ship.ovm"));
	const auto pShipMaterial = MaterialManager::Get()->CreateMaterial<DeferredShadow>();
	pShipMaterial->SetDiffuseMap(L"Textures/Ship/Texture0.png");
	auto convexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Ship.ovpc");
	//AddComponent(pModel);
	pModel->SetMaterial(pShipMaterial);
	auto RigidBody = AddComponent(new RigidBodyComponent(true));
	auto PxConvex = PxConvexMeshGeometry{ convexMesh };
	auto PxConvexTrigger = PxConvexMeshGeometry{ convexMesh };
	PxConvex.scale = PxMeshScale{ 0.7f };
	
	RigidBody->AddCollider(PxConvex, *pDefaultMaterial, false);

	GetTransform()->Scale(0.7f);
}

void Ship::Update(const SceneContext&)
{
}
