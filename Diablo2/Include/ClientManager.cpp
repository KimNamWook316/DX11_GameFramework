#include "ClientManager.h"
#include "resource.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Input.h"
#include "Resource/ResourceManager.h"
#include "Scene/D2IntroScene.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/TileMapComponent.h"
#include "Component/ProcedualMapGenerator.h"
#include "Component/DissolveComponent.h"
#include "Component/NavAgentComponent.h"
#include "Component/StateComponent.h"
#include "UI/D2FrameWidget.h"
#include "UI/D2MouseNormal.h"
#include "UI/D2IntroWindow.h"
#include "UI/D2MouseItemSelect.h"
#include "Component/D2CharacterInfoComponent.h"
#include "Component/D2PlayerCollider.h"
#include "Component/D2EnemyCollider.h"
#include "Component/D2DataManagerComponent.h"
#include "Component/D2NavAgentComponent.h"
#include "Component/D2EnemyNavAgentComponent.h"
#include "Component/D2ObjectPoolComponent.h"
#include "Component/D2StateComponent.h"
#include "Component/D2ProjectTile.h"
#include "Component/D2MeleeAttack.h"
#include "Component/D2Blaze.h"
#include "Component/D2FrozenOrb.h"
#include "Component/D2Meteor.h"
#include "Component/D2MeteorFire.h"
#include "Component/D2MeteorTarget.h"
#include "Component/D2Teleport.h"
#include "Component/D2EnemyMeleeAttack.h"
#include "Component/D2PlayerSkillComponent.h"
#include "Component/D2EnemySkillComponent.h"
#include "Component/D2Effect.h"
#include "Component/D2ItemTableComponent.h"
#include "Component/D2UIManagerComponent.h"
#include "Component/D2Inventory.h"
#include "Component/D2ItemObjectComponent.h"
#include "Component/D2ProcedualMapGenerator.h"
#include "Component/D2DungeonManagerComponent.h"
#include "Component/D2ShadowComponent.h"
#include "Component/D2PortalObject.h"
#include "Component/D2ClickableUIComponent.h"
#include "Component/D2ChestObject.h"
#include "State/PlayerIdleState.h"
#include "State/D2EnemyIdleState.h"
#include "State/D2AndarielIdleState.h"
#include "State/D2SkeletonIdleState.h"
#include "Engine.h"
#include "Resource.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Render/RenderManager.h"
#include "Input.h"

DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("Diablo2"), 800, 600, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}
	CEngine::GetInst()->SetClearColor(0.f, 0.f, 0.f, 0.f);

	CEngine::GetInst()->CreateMouse<CD2MouseNormal>(eMouseState::Normal, "MouseNormal");
	CEngine::GetInst()->CreateMouse<CD2MouseItemSelect>(eMouseState::State1, "MouseItemSelect");

	CResourceManager::GetInst()->CreateFontFile("Dia2Font", TEXT("Exocet2.ttf"));
	const TCHAR* fontFace = CResourceManager::GetInst()->GetFontFaceName("Dia2Font");
	CResourceManager::GetInst()->LoadFont("Dia2Font", fontFace, 600, 20.f, TEXT("eng"));

	// SceneManager CallBack
	CSceneManager::GetInst()->SetCreateObjectCallBack<CClientManager>(this, &CClientManager::OnCreateObject);
	CSceneManager::GetInst()->SetCreateComponentCallBack<CClientManager>(this, &CClientManager::OnCreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceCallBack<CClientManager>(this, &CClientManager::OnCreateAnimInstance);
	CSceneManager::GetInst()->SetCreateStateCallBack<CClientManager>(this, &CClientManager::OnCreateState);

	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CD2IntroScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CClientManager::CreateSceneMode(CScene* scene, size_t type)
{
}

class CGameObject* CClientManager::OnCreateObject(CScene* scene, size_t type)
{
	if (type == typeid(CGameObject).hash_code())
	{
		CGameObject* obj = scene->CreateEmptyObjectByType<CGameObject>();
		return obj;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

void CClientManager::OnCreateAnimInstance(CSpriteComponent* comp, size_t type)
{
	if (type == typeid(CAnimationSequence2DInstance).hash_code())
	{
		comp->SceneLoadAnimationInstance<CAnimationSequence2DInstance>();
	}
}

CComponent* CClientManager::OnCreateComponent(CGameObject* obj, size_t type)
{
	if (type == typeid(CSceneComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSceneComponent>();
		return component;
	}
	else if (type == typeid(CSpriteComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSpriteComponent>();
		return component;
	}
	else if(type == typeid(CStaticMeshComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CStaticMeshComponent>();
		return component;
	}
	else if(type == typeid(CColliderBox2D).hash_code())
	{
		CComponent* component = obj->LoadComponent<CColliderBox2D>();
		return component;
	}
	else if(type == typeid(CColliderCircle).hash_code())
	{
		CComponent* component = obj->LoadComponent<CColliderCircle>();
		return component;
	}
	else if(type == typeid(CColliderPixel).hash_code())
	{
		CComponent* component = obj->LoadComponent<CColliderPixel>();
		return component;
	}
	else if(type == typeid(CCameraComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CCameraComponent>();
		return component;
	}
	else if(type == typeid(CWidgetComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CWidgetComponent>();
		return component;
	}
	else if(type == typeid(CParticleComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CParticleComponent>();
		return component;
	}
	else if(type == typeid(CTileMapComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CTileMapComponent>();
		return component;
	}
	else if(type == typeid(CProcedualMapGenerator).hash_code())
	{
		CComponent* component = obj->LoadComponent<CProcedualMapGenerator>();
		return component;
	}
	else if(type == typeid(CDissolveComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CDissolveComponent>();
		return component;
	}
	else if(type == typeid(CNavAgentComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CNavAgentComponent>();
		return component;
	}
	else if(type == typeid(CStateComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CStateComponent>();
		return component;
	}
	else if(type == typeid(CD2CharacterInfoComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2CharacterInfoComponent>();
		return component;
	}
	else if(type == typeid(CD2DataManagerComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2DataManagerComponent>();
		return component;
	}
	else if(type == typeid(CD2NavAgentComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2NavAgentComponent>();
		return component;
	}
	else if(type == typeid(CD2EnemyNavAgentComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemyNavAgentComponent>();
		return component;
	}
	else if(type == typeid(CD2ObjectPoolComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ObjectPoolComponent>();
		return component;
	}
	else if(type == typeid(CD2Projectile).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Projectile>();
		return component;
	}
	else if(type == typeid(CD2MeleeAttack).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2MeleeAttack>();
		return component;
	}
	else if(type == typeid(CD2PlayerSkillComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2PlayerSkillComponent>();
		return component;
	}
	else if(type == typeid(CD2Blaze).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Blaze>();
		return component;
	}
	else if(type == typeid(CD2FrozenOrb).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2FrozenOrb>();
		return component;
	}
	else if(type == typeid(CD2Meteor).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Meteor>();
		return component;
	}
	else if(type == typeid(CD2MeteorFire).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2MeteorFire>();
		return component;
	}
	else if(type == typeid(CD2MeteorTarget).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2MeteorTarget>();
		return component;
	}
	else if(type == typeid(CD2Teleport).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Teleport>();
		return component;
	}
	else if(type == typeid(CD2StateComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2StateComponent>();
		return component;
	}
	else if(type == typeid(CD2PlayerCollider).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2PlayerCollider>();
		return component;
	}
	else if(type == typeid(CD2EnemyCollider).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemyCollider>();
		return component;
	}
	else if(type == typeid(CD2EnemySkillComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemySkillComponent>();
		return component;
	}
	else if(type == typeid(CD2EnemyMeleeAttack).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemyMeleeAttack>();
		return component;
	}
	else if(type == typeid(CD2Effect).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Effect>();
		return component;
	}
	else if(type == typeid(CD2UIManagerComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2UIManagerComponent>();
		return component;
	}
	else if(type == typeid(CD2ItemTableComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ItemTableComponent>();
		return component;
	}
	else if(type == typeid(CD2Inventory).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Inventory>();
		return component;
	}
	else if(type == typeid(CD2ItemObjectComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ItemObjectComponent>();
		return component;
	}
	else if(type == typeid(CD2ProcedualMapGenerator).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ProcedualMapGenerator>();
		return component;
	}
	else if(type == typeid(CD2DungeonManagerComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2DungeonManagerComponent>();
		return component;
	}
	else if(type == typeid(CD2ShadowComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ShadowComponent>();
		return component;
	}
	else if(type == typeid(CD2PortalObject).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2PortalObject>();
		return component;
	}
	else if(type == typeid(CD2ClickableUIComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ClickableUIComponent>();
		return component;
	}
	else if(type == typeid(CD2ChestObject).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ChestObject>();
		return component;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

void CClientManager::OnCreateState(CStateComponent* comp, size_t type)
{
	if (type == typeid(CPlayerIdleState).hash_code())
	{
		comp->SetInitialState<CPlayerIdleState>();
	}
	else if (type == typeid(CD2EnemyIdleState).hash_code())
	{
		comp->SetInitialState<CD2EnemyIdleState>();
	}
	else if (type == typeid(CD2AndarielIdleState).hash_code())
	{
		comp->SetInitialState<CD2AndarielIdleState>();
	}
	else if (type == typeid(CD2SkeletonIdleState).hash_code())
	{
		comp->SetInitialState<CD2SkeletonIdleState>();
	}
}
