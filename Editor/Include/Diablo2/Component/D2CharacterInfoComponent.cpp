#include "D2CharacterInfoComponent.h"
#include "D2DataManager.h"
#include "../D2Util.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/NavAgentComponent.h"
#include "D2NavAgentComponent.h"
#include "D2EnemyNavAgentComponent.h"

CD2CharacterInfoComponent::CD2CharacterInfoComponent():
	mCharInfo{},
	meCC(eD2ElementType::None),
	mCCTime(0.f),
	mCCTick(0.f)
{
	SetTypeID<CD2CharacterInfoComponent>();
}

CD2CharacterInfoComponent::CD2CharacterInfoComponent(const CD2CharacterInfoComponent& com) :
	CObjectComponent(com)
{
	mCharInfo = com.mCharInfo;
	mCharInfo.Speed = 0.f;
	meCC = eD2ElementType::None;
	mCCTime = 0.f;
}

CD2CharacterInfoComponent::~CD2CharacterInfoComponent()
{
}

bool CD2CharacterInfoComponent::Init()
{
	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	mNavAgent = mObject->FindObjectComponentFromType<CD2NavAgentComponent>();

	if (!mNavAgent)
	{
		mNavAgent = mObject->FindObjectComponentFromType<CD2EnemyNavAgentComponent>();
	}

	if (!mSprite)
	{
		assert(false);
		return false;
	}

	return true;
}

void CD2CharacterInfoComponent::Start()
{
	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();

	if (!mSprite)
	{
		assert(false);
		return;
	}

	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	mNavAgent = mObject->FindObjectComponentFromType<CD2NavAgentComponent>();

	if (!mNavAgent)
	{
		mNavAgent = mObject->FindObjectComponentFromType<CD2EnemyNavAgentComponent>();
	}
}

void CD2CharacterInfoComponent::Update(float deltaTime)
{
	if (mCCTime >= mCharInfo.MaxCCTime)
	{
		SetSpeed(mPrevSpeed);
		mSprite->SetBaseColor(1.f, 1.f, 1.f, 1.f);
		mCCTime = 0.f;
		mCCTick = 0.f;
		meCC = eD2ElementType::None;
	}

	if (meCC != eD2ElementType::None)
	{
		mCCTime += deltaTime;
		mCCTick += deltaTime;
	}

	switch (meCC)
	{
	case eD2ElementType::Fire:
		mSprite->SetBaseColor(1.f, 0.f, 0.f, 1.f);
		if (mCCTick >= 1.f)
		{
			mCCTick = 0.f;
			SetHp(-mCharInfo.Hp * 0.01f);
		}
		break;
	case eD2ElementType::Ice:
		mSprite->SetBaseColor(0.f, 0.f, 1.f, 1.f);
		SetSpeed(mCharInfo.MaxSpeed * 0.3f);
		break;
	case eD2ElementType::Poison:
		mSprite->SetBaseColor(0.f, 1.f, 0.f, 1.f);
		if (mCCTick >= 1.f)
		{
			mCCTick = 0.f;
			SetHp(-mCharInfo.Hp * 0.1f);
		}
		break;
	case eD2ElementType::Lightning:
		break;

	}
}

void CD2CharacterInfoComponent::PostUpdate(float deltaTime)
{
}

void CD2CharacterInfoComponent::PrevRender()
{
}

void CD2CharacterInfoComponent::Render()
{
}

void CD2CharacterInfoComponent::PostRender()
{
}

CD2CharacterInfoComponent* CD2CharacterInfoComponent::Clone()
{
	return new CD2CharacterInfoComponent(*this);
}

void CD2CharacterInfoComponent::Save(FILE* fp)
{
	CComponent::Save(fp);
	int length = mCharInfo.Name.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mCharInfo.Name.c_str(), sizeof(char), length, fp);
}

void CD2CharacterInfoComponent::Load(FILE* fp)
{
	CComponent::Load(fp);

	char name[256] = {};
	int length = 0;
	fread(&length, sizeof(int), 1, fp);
	fread(name, sizeof(char), length, fp);

	SetData(name);
}

void CD2CharacterInfoComponent::SetData(const std::string& name)
{
	D2CharInfo* info = CD2DataManager::GetInst()->FindCharcterInfo(name);

	if (!info)
	{
		assert(false);
		return;
	}

	mCharInfo = *info;
}

void CD2CharacterInfoComponent::SetSpeed(const float speed)
{
	mCharInfo.Speed = speed;
}

void CD2CharacterInfoComponent::SetDir(const Vector2& dir)
{
	mCharInfo.Dir = dir;
	mCharInfo.eSpriteDir = CD2Util::GetSpriteDir(dir);

	callEventCallBack(eD2CharInfoEventType::ChangeDir);
}

void CD2CharacterInfoComponent::SetHp(const float hp)
{
	mCharInfo.Hp += hp;

	if (mCharInfo.Hp >= mCharInfo.MaxHp)
	{
		mCharInfo.Hp = mCharInfo.MaxHp;
	}

	if (mCharInfo.Hp < 0)
	{
		mCharInfo.Hp = 0;
	}

	if (hp == 0)
	{
		callEventCallBack(eD2CharInfoEventType::Die);
		return;
	}

	if (hp > 0)
	{
		callEventCallBack(eD2CharInfoEventType::HpInc);
	}
	else
	{
		callEventCallBack(eD2CharInfoEventType::HpDec);
	}
}

void CD2CharacterInfoComponent::SetMaxHp(const float hp)
{
	mCharInfo.MaxHp += hp;

	if (mCharInfo.MaxHp <= 0)
	{
		mCharInfo.MaxHp = 0;
	}

	if (hp > 0)
	{
		callEventCallBack(eD2CharInfoEventType::MaxHpInc);
	}
	else
	{
		if (mCharInfo.Hp > mCharInfo.MaxHp)
		{
			mCharInfo.Hp = mCharInfo.MaxHp;
		}
		callEventCallBack(eD2CharInfoEventType::MaxHpDec);
	}
}

void CD2CharacterInfoComponent::SetCC(eD2ElementType type)
{
	if (meCC == eD2ElementType::None)
	{
		if (type == eD2ElementType::Ice)
		{
			mPrevSpeed = mCharInfo.Speed;
		}
	}
	mCCTime = 0.f;
	meCC = type;

	callEventCallBack(eD2CharInfoEventType::CC);
}

void CD2CharacterInfoComponent::SetMp(const float mp)
{
	mCharInfo.Mp += mp;

	if (mCharInfo.Mp >= mCharInfo.MaxMp)
	{
		mCharInfo.Mp = mCharInfo.MaxMp;
	}

	if (mCharInfo.Mp < 0)
	{
		mCharInfo.Mp = 0;
	}

	if (mp == 0)
	{
		return;
	}

	if (mp > 0)
	{
		callEventCallBack(eD2CharInfoEventType::MpInc);
	}
	else
	{
		callEventCallBack(eD2CharInfoEventType::MpDec);
	}
}

void CD2CharacterInfoComponent::SetMaxMp(const float mp)
{
	mCharInfo.MaxMp += mp;

	if (mCharInfo.MaxMp <= 0)
	{
		mCharInfo.MaxMp = 0;
	}

	if (mp > 0)
	{
		callEventCallBack(eD2CharInfoEventType::MaxMpInc);
	}
	else
	{
		if (mCharInfo.Mp > mCharInfo.MaxMp)
		{
			mCharInfo.Mp = mCharInfo.MaxMp;
		}
		callEventCallBack(eD2CharInfoEventType::MaxMpDec);
	}
}

void CD2CharacterInfoComponent::SetMeleeBonus(const float val)
{
	mCharInfo.MeleeBonus += val;

	if (val > 0)
	{
		callEventCallBack(eD2CharInfoEventType::MeleeBonusInc);
	}
	else
	{
		callEventCallBack(eD2CharInfoEventType::MeleeBonusDec);
	}
}

void CD2CharacterInfoComponent::SetMagicBonus(const float val)
{
	mCharInfo.MagicBonus += val;

	if (val > 0)
	{
		callEventCallBack(eD2CharInfoEventType::MagicBonusInc);
	}
	else
	{
		callEventCallBack(eD2CharInfoEventType::MagicBonusDec);
	}
}

void CD2CharacterInfoComponent::DeleteEventCallBack(const std::string& name, eD2CharInfoEventType type)
{
	auto iter = mEventCallBackList[(int)type].begin();
	auto iterEnd = mEventCallBackList[(int)type].end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter).Name == name)
		{
			mEventCallBackList[(int)type].erase(iter);
			return;
		}
	}
}

void CD2CharacterInfoComponent::callEventCallBack(eD2CharInfoEventType eType)
{
	if (!mEventCallBackList[(int)eType].empty())
	{
		auto iter = mEventCallBackList[(int)eType].begin();
		auto iterEnd = mEventCallBackList[(int)eType].end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter).CallBack();
		}
	}
}
