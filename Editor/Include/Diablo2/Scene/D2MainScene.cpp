#include "D2MainScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "GameObject/GameObject.h"
#include "Input.h"
#include "Collision/CollisionManager.h"
#include "../Resource/Mesh/D2ShadowMesh.h"
#include "../Resource/Shader/D2ShadowShader.h"

CD2MainScene::CD2MainScene()
{
}

CD2MainScene::~CD2MainScene()
{
	mMainMusic->Stop();
}

bool CD2MainScene::Init()
{
	if (!CCollisionManager::GetInst()->LoadProfile("CollsionProfileInfo.csv"))
	{
		assert(false);
		return false;
	}

	createMesh();
	createShader();
	loadSound();

	std::string outName;
	mScene->LoadGameObject(outName, "DataManager.gobj");
	mLoadingFunction(false, 0.1f);
	Sleep(500);
	mScene->LoadGameObject(outName, "ObjectPool.gobj");
	mLoadingFunction(false, 0.4f);
	Sleep(500);
	mScene->LoadGameObject(outName, "ItemTable.gobj");
	mLoadingFunction(false, 0.5f);
	Sleep(500);
	mScene->LoadGameObject(outName, "DungeonManager.gobj");
	mLoadingFunction(false, 0.8f);
	Sleep(500);

	createKey();
 
	return true;
}

void CD2MainScene::OnLoadEnd()
{
	mScene->GetResource()->SetVolume("BackGround", 10.f);
	mMainMusic->Play();
}

#ifdef _DEBUG

void CD2MainScene::OnF2Down(float deltaTime)
{
	if (CEngine::GetInst()->IsDebugMode())
	{
		CEngine::GetInst()->SetDebugMode(false);
	}
	else
	{
		CEngine::GetInst()->SetDebugMode(true);
	}
}

#endif // _DEBUG

void CD2MainScene::createKey()
{
#ifdef _DEBUG
	CInput::GetInst()->CreateKey("F2", VK_F2);
	CInput::GetInst()->SetKeyCallBack("F2", eKeyState::KeyState_Down, this, &CD2MainScene::OnF2Down);
#endif // _DEBUG
	CInput::GetInst()->CreateKey("MouseL", VK_LBUTTON);
	CInput::GetInst()->CreateKey("MouseR", VK_RBUTTON);
	CInput::GetInst()->CreateKey("LCtrl", VK_LCONTROL);
	CInput::GetInst()->SetCtrlKey("LCtrl", true);
	CInput::GetInst()->CreateKey("MouseLCtrl", VK_LBUTTON);
	CInput::GetInst()->SetCtrlKey("MouseLCtrl", true);
	CInput::GetInst()->CreateKey("MouseRCtrl", VK_RBUTTON);
	CInput::GetInst()->SetCtrlKey("MouseRCtrl", true);

	// UIKey
	CInput::GetInst()->CreateKey("ToggleInventory", 'I');
	CInput::GetInst()->CreateKey("ToggleSkillTree", 'K');
	CInput::GetInst()->CreateKey("CloseMenu", VK_ESCAPE);
	CInput::GetInst()->CreateKey("ClickableUI", VK_TAB);

	// Skill Debug Key
	CInput::GetInst()->CreateKey("SkillLevelUp", VK_NUMPAD0);
	CInput::GetInst()->CreateKey("NextSkill", VK_NUMPAD1);
}

void CD2MainScene::createMesh()
{
	// Custom Mesh & Shader
	mScene->GetResource()->CreateMesh<CD2ShadowMesh>("ShadowMesh");
}

void CD2MainScene::createShader()
{
	// Custom Mesh & Shader
	mScene->GetResource()->CreateShader<CD2ShadowShader>("ShadowShader");
}

void CD2MainScene::loadSound()
{
	// Main Music
	mScene->GetResource()->CreateSoundChannelGroup("BackGround");
	mScene->GetResource()->LoadSound("BackGround", true, "MainMusic",
		"Diablo2/wild.wav");
	mMainMusic = mScene->GetResource()->FindSound("MainMusic");

	// UI Sound
	mScene->GetResource()->CreateSoundChannelGroup("UI");
	mScene->GetResource()->SetVolume("UI", 30.f);
	mScene->GetResource()->LoadSound("UI", false, "WindowOpen", "Diablo2/UI/WindowOpen.wav");
	mScene->GetResource()->LoadSound("UI", false, "Button", "Diablo2/UI/Button.wav");
	mScene->GetResource()->LoadSound("UI", false, "ArmorEquip", "Diablo2/UI/Item/ArmorEquip.wav");
	mScene->GetResource()->LoadSound("UI", false, "BeltEquip", "Diablo2/UI/Item/BeltEquip.wav");
	mScene->GetResource()->LoadSound("UI", false, "BootsEquip", "Diablo2/UI/Item/BootsEquip.wav");
	mScene->GetResource()->LoadSound("UI", false, "HeadEquip", "Diablo2/UI/Item/HeadEquip.wav");
	mScene->GetResource()->LoadSound("UI", false, "ItemDrop", "Diablo2/UI/Item/ItemDrop.wav");
	mScene->GetResource()->LoadSound("UI", false, "PlateArmorEquip", "Diablo2/UI/Item/PlateArmorEquip.wav");
	mScene->GetResource()->LoadSound("UI", false, "PotionUI", "Diablo2/UI/Item/PotionUI.wav");
	mScene->GetResource()->LoadSound("UI", false, "StaffEquip", "Diablo2/UI/Item/StaffEquip.wav");
	mScene->GetResource()->LoadSound("UI", false, "UsePotion", "Diablo2/UI/Item/UsePotion.wav");

	// Player Sound
	mScene->GetResource()->CreateSoundChannelGroup("Player");
	mScene->GetResource()->LoadSound("Player", false, "PlayerHit", "Diablo2/Player/PlayerHit.mp3");
	mScene->GetResource()->LoadSound("Player", false, "PlayerNeedMana", "Diablo2/Player/PlayerNeedMana.mp3");
	mScene->GetResource()->LoadSound("Player", true, "PlayerRun", "Diablo2/Player/PlayerRun.mp3");
	mScene->GetResource()->LoadSound("Player", true, "PlayerWalk", "Diablo2/Player/PlayerWalk.mp3");

	// Effect Sound
	mScene->GetResource()->CreateSoundChannelGroup("FX");
	mScene->GetResource()->SetVolume("FX", 30.f);
	mScene->GetResource()->LoadSound("FX", false, "FireBall", "Diablo2/Player/FireBall.mp3");
	mScene->GetResource()->LoadSound("FX", false, "FireBolt", "Diablo2/Player/FireBolt.mp3");
	mScene->GetResource()->LoadSound("FX", false, "FrozenOrb", "Diablo2/Player/FrozenOrb.mp3");
	mScene->GetResource()->LoadSound("FX", false, "FrozenOrbEnd", "Diablo2/Player/FrozenOrbEnd.mp3");
	mScene->GetResource()->LoadSound("FX", false, "IceBolt", "Diablo2/Player/IceBolt.mp3");
	mScene->GetResource()->LoadSound("FX", true, "Fire", "Diablo2/Player/Blaze.mp3");
	mScene->GetResource()->LoadSound("FX", false, "FireCast", "Diablo2/Player/FireCast.mp3");
	mScene->GetResource()->LoadSound("FX", false, "IceCast", "Diablo2/Player/IceCast.mp3");
	mScene->GetResource()->LoadSound("FX", false, "Teleport", "Diablo2/Player/Teleport.mp3");
	mScene->GetResource()->LoadSound("FX", false, "MeteorTarget", "Diablo2/Player/MeteorTarget.mp3");
	mScene->GetResource()->LoadSound("FX", false, "Meteor", "Diablo2/Player/MeteorImpact.mp3");

	// Object Sound
	mScene->GetResource()->CreateSoundChannelGroup("Object");
	mScene->GetResource()->SetVolume("Object", 50.f);
	mScene->GetResource()->LoadSound("Object", false, "ItemDrop", "Diablo2/Item/ItemDrop.wav");
	mScene->GetResource()->LoadSound("Object", false, "Chest", "Diablo2/Objects/Chest.wav");
	mScene->GetResource()->LoadSound("Object", false, "PortalOpen", "Diablo2/Objects/PortalOpen.wav");
	mScene->GetResource()->LoadSound("Object", true, "Portal", "Diablo2/Objects/Portal.wav");
	
	// Monster Sound
	mScene->GetResource()->CreateSoundChannelGroup("Monster");
	mScene->GetResource()->SetVolume("Monster", 30.f);
	mScene->GetResource()->LoadSound("Monster", false, "AndarielAttack", "Diablo2/Monster/Andariel/AndarielAttack.wav");
	mScene->GetResource()->LoadSound("Monster", false, "AndarielDeath", "Diablo2/Monster/Andariel/AndarielDeath.wav");
	mScene->GetResource()->LoadSound("Monster", false, "AndarielHit", "Diablo2/Monster/Andariel/AndarielHit.wav");
	mScene->GetResource()->LoadSound("Monster", false, "AndarielProjectile", "Diablo2/Monster/Andariel/AndarielProjectile.wav");
	mScene->GetResource()->LoadSound("Monster", false, "AndarielSpray", "Diablo2/Monster/Andariel/AndarielSpray.wav");
	mScene->GetResource()->LoadSound("Monster", false, "DemonAttack", "Diablo2/Monster/Demon/DemonAttack.wav");
	mScene->GetResource()->LoadSound("Monster", false, "DemonDeath", "Diablo2/Monster/Demon/DemonDeath.wav");
	mScene->GetResource()->LoadSound("Monster", false, "DemonHit", "Diablo2/Monster/Demon/DemonHit.wav");
	mScene->GetResource()->LoadSound("Monster", false, "DemonStartChase", "Diablo2/Monster/Demon/DemonStartChase.wav");
	mScene->GetResource()->LoadSound("Monster", false, "SkeletonAttack", "Diablo2/Monster/Skeleton/SkeletonAttack.wav");
	mScene->GetResource()->LoadSound("Monster", false, "SkeletonDeath", "Diablo2/Monster/Skeleton/SkeletonDeath.wav");
	mScene->GetResource()->LoadSound("Monster", false, "SkeletonHit", "Diablo2/Monster/Skeleton/SkeletonHit.wav");
}
