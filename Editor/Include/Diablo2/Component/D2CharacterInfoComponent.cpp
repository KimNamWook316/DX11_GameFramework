#include "D2CharacterInfoComponent.h"
#include "D2DataManager.h"
#include "../D2Util.h"

CD2CharacterInfoComponent::CD2CharacterInfoComponent():
	mCharInfo{}
{
	SetTypeID<CD2CharacterInfoComponent>();
}

CD2CharacterInfoComponent::CD2CharacterInfoComponent(const CD2CharacterInfoComponent& com) :
	CObjectComponent(com)
{
	mCharInfo = com.mCharInfo;
	mCharInfo.Speed = 0.f;
}

CD2CharacterInfoComponent::~CD2CharacterInfoComponent()
{
}

bool CD2CharacterInfoComponent::Init()
{
	return true;
}

void CD2CharacterInfoComponent::Start()
{
}

void CD2CharacterInfoComponent::Update(float deltaTime)
{
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
