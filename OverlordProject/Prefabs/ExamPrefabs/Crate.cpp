#include "stdafx.h"
#include "Crate.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/BasicMaterial_Deferred.h"
#include "Prefabs/ExamPrefabs/CrateParticle.h"
#include "Prefabs/ExamPrefabs/BoltPickUp.h"

Crate::Crate(const XMFLOAT3& Position, CrateType type):
	m_Position(Position),
	m_Type(type)
{
	GetTransform()->Scale(0.25f);
	SoundManager::Get()->GetSystem()->createStream("Resources/Audio/Crate.wav", FMOD_DEFAULT, nullptr, &m_pCrateBreakSound);
}

Crate::~Crate()
{
}

void Crate::Destroy()
{
	m_Hit = true;
	if (this)
	{
		auto pos = GetTransform()->GetPosition();

		SoundManager::Get()->GetSystem()->playSound(m_pCrateBreakSound, nullptr, false, nullptr);
		SceneManager::Get()->GetActiveScene()->AddChild(new CrateParticle({ pos.x , pos.y + 1.5f , pos.z }, 1.0f));
		SceneManager::Get()->GetActiveScene()->AddChild(new BoltPickUp(GetTransform()->GetPosition()));
		RemoveComponent(m_pModel, true);
	}
	
}

void Crate::Initialize(const SceneContext&)
{
	GetTransform()->Translate(m_Position);
	SetTag(L"Crate");

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	auto pMaterial = MaterialManager::Get()->CreateMaterial<BasicMaterial_Deferred>();
	auto pGlassMaterial = MaterialManager::Get()->CreateMaterial<BasicMaterial_Deferred>();
	const auto pActor = AddComponent(new RigidBodyComponent(false));
	const auto convexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Crate.ovpc");
	auto PxConvex = PxConvexMeshGeometry{ convexMesh };
	PxConvex.scale = PxMeshScale{ 0.25f };
	pActor->AddCollider(PxConvex, *pDefaultMaterial , false);
	switch (m_Type)
	{
	case CrateType::BoltCrate:
		m_pModel = AddComponent(new ModelComponent(L"Meshes/Crate.ovm"));
		pMaterial->SetDiffuseMap(L"Textures/Crate.png");
		m_pModel->SetMaterial(pMaterial);
		break;
	case CrateType::NanoTechCrate:
		m_pModel = AddComponent(new ModelComponent(L"Meshes/CrateNano.ovm"));
		pMaterial->SetDiffuseMap(L"Textures/CrateNano.png");
		m_pModel->SetMaterial(pMaterial,0);
		pGlassMaterial->SetDiffuseMap(L"Textures/CrateGlass.png");
		m_pModel->SetMaterial(pGlassMaterial,1);

		break;
	case CrateType::ExplosiveCrate:
		m_pModel = AddComponent(new ModelComponent(L"Meshes/CrateExp.ovm"));
		pMaterial->SetDiffuseMap(L"Textures/CrateExp.png");
		m_pModel->SetMaterial(pMaterial);
		

		break;
	default:
		break;
	}

}
void Crate::Reset()
{
	
}
void Crate::Update(const SceneContext&  )
{
	
	if (m_Hit)
	{
	
		
		GetScene()->RemoveChild(this, true);
	}
}

