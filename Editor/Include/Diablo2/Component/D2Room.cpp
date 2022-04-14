#include "D2Room.h"
#include "D2Space.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"
#include "Component/TileMapComponent.h"
#include "D2CharacterInfoComponent.h"

CD2Room::CD2Room()	:
	mSpace(nullptr),
	mRoomIdx(0),
	mSizeX(0),
	mSizeY(0),
	mLBIdxX(-1),
	mLBIdxY(-1),
	mLBIdx(-1),
	mRTIdxX(-1),
	mRTIdxY(-1),
	mRTIdx(-1),
	mCenterIdxX(-1),
	mCenterIdxY(-1),
	mCenterIdx(-1),
	mPlayerEnterCount(0),
	mPlayerExitCount(0)
{
}

CD2Room::~CD2Room()
{
}

bool CD2Room::GenerateRoomCollider()
{
	CD2ProcedualMapGenerator* owner = mSpace->GetOwner();

 //	mRoomColliderObjList.push_back(owner->GetScene()->CreateGameObject<CGameObject>("RoomCollider"));
 //	CColliderBox2D* collider =  mRoomColliderObjList.front()->CreateComponentAddChild<CColliderBox2D>("Collider");
 //	collider->SetCollisionProfile("RoomCollider");
 //	collider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Room::OnCollideEnter);
 //	collider->AddCollisionCallBack(eCollisionState::Exit, this, &CD2Room::OnCollideExit);
 //
 //	float tileDiagonal = owner->GetTileDiagonal();
 //	collider = mRoomColliderObjList.front()->FindSceneComponentFromType<CColliderBox2D>();
 //	collider->SetExtent(mSizeX * tileDiagonal / 2.f, tileDiagonal * mSizeY / 2.f);
 //
 //	CTileMapComponent* tileMap = owner->GetTileMapComponent();
 //	Vector3 tileMapPos = tileMap->GetWorldPos();
 //	mRoomColliderObjList.front()->SetWorldPos(tileMap->GetTile(mCenterIdx)->GetPos().x, tileMap->GetTile(mCenterIdx)->GetPos().y, 0.f);
 //
 //	mRoomColliderObjList.front()->SetWorldRotZ(26.6);

	return true;
}

bool CD2Room::Spawn()
{
	if (meSpawnType == eD2RoomSpawnType::Player)
	{
		std::string name;
		CGameObject* player = mSpace->GetOwner()->GetScene()->LoadGameObject(name, "Player3.gobj");
		player->SetWorldPos(mSpace->GetOwner()->GetTileMapComponent()->GetTile(mCenterIdx)->GetPos());
		player->FindObjectComponentFromType<CD2CharacterInfoComponent>()->SetSpawnRoomIdx(mRoomIdx);
		return true;
	}

	std::string name;
	CTileMapComponent* tileMap = mSpace->GetOwner()->GetTileMapComponent();
	CGameObject* barlog = mSpace->GetOwner()->GetScene()->LoadGameObject(name, "Barlog2.gobj");
	CGameObject* skeleton = mSpace->GetOwner()->GetScene()->LoadGameObject(name, "Skeleton2.gobj");
	barlog->Start();
	skeleton->Start();
	int monsterAmountX = mSizeX / 5.f + 1;
	int monsterAmountY = mSizeY / 5.f + 1;

	CGameObject* clone = nullptr;
	for (int y = -monsterAmountY / 2; y < monsterAmountY / 2; ++y)
	{
		for (int x = -monsterAmountX / 2; x < monsterAmountX / 2; ++x)
		{
			// 몬스터 스폰 얼마나 됐는지
			++mMonsterCount;

			Vector3 pos = tileMap->GetTile(mCenterIdxX + x, mCenterIdxY + y)->GetPos();

			if (meSpawnType == eD2RoomSpawnType::Barlog)
			{
				clone = barlog->Clone();
			}
			else if (meSpawnType == eD2RoomSpawnType::Skeleton)
			{
				clone = skeleton->Clone();
			}
			else if (meSpawnType == eD2RoomSpawnType::Mix)
			{
				eD2RoomSpawnType spawnType = (eD2RoomSpawnType)(rand() % (int)eD2RoomSpawnType::Mix);

				switch (spawnType)
				{
				case eD2RoomSpawnType::Barlog:
				clone = barlog->Clone();
					break;
				case eD2RoomSpawnType::Skeleton:
				clone = skeleton->Clone();
					break;
				}
			}
			else if (meSpawnType == eD2RoomSpawnType::Boss)
			{
				if (x == 0 && y == 0)
				{
					std::string name;
					CGameObject* boss = mSpace->GetOwner()->GetScene()->LoadGameObject(name, "Andariel3.gobj");
					boss->SetWorldPos(mSpace->GetOwner()->GetTileMapComponent()->GetTile(mCenterIdx)->GetPos());
					boss->FindObjectComponentFromType<CD2CharacterInfoComponent>()->SetSpawnRoomIdx(mRoomIdx);
					continue;
				}
				eD2RoomSpawnType spawnType = (eD2RoomSpawnType)(rand() % (int)eD2RoomSpawnType::Mix);

				switch (spawnType)
				{
				case eD2RoomSpawnType::Barlog:
				clone = barlog->Clone();
					break;
				case eD2RoomSpawnType::Skeleton:
				clone = skeleton->Clone();
					break;
				}
			}

			clone->FindObjectComponentFromType<CD2CharacterInfoComponent>()->SetSpawnRoomIdx(mRoomIdx);
			clone->SetWorldPos(pos.x, pos.y, 0.f);
			clone->Start();
		}
	}

	barlog->Destroy();
	skeleton->Destroy();

	return true;
}

void CD2Room::CreatePortal()
{
	std::string name;
	CGameObject* portalObject = mSpace->GetOwner()->GetScene()->LoadGameObject(name, "IzualPortal.gobj");
	portalObject->Start();
	portalObject->SetWorldPos(mSpace->GetOwner()->GetTileMapComponent()->GetTile(mCenterIdx)->GetPos());
}

void CD2Room::OnMonsterDead()
{
	--mMonsterCount;

	if (mMonsterCount == 0)
	{
		spawnChest();
	}
}

void CD2Room::OnCollideEnter(const CollisionResult& result)
{
}

void CD2Room::OnCollideExit(const CollisionResult& result)
{
}

void CD2Room::spawnChest()
{
	// 플레이어 방과 보스 방의 경우 몬스터 모두 처치시 보상 상자 생성하지 않는다.
	if (meSpawnType == eD2RoomSpawnType::Boss || meSpawnType == eD2RoomSpawnType::Player)
	{
		return;
	}

	std::string name;
	CGameObject* chest = mSpace->GetOwner()->GetScene()->LoadGameObject(name, "Chest.gobj");
	chest->SetWorldPos(mSpace->GetOwner()->GetTileMapComponent()->GetTile(mCenterIdx)->GetPos());
}
