#include "D2Item.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"

CD2Item::CD2Item()	:
	mInfo{},
	mOwner(nullptr),
	mCharInfo(nullptr)
{
}

CD2Item::~CD2Item()
{
}

void CD2Item::SetOwner(CGameObject* obj)
{
	mOwner = obj;

	if (mOwner)
	{
		mCharInfo = mOwner->FindObjectComponentFromType<CD2CharacterInfoComponent>();
	}
}
