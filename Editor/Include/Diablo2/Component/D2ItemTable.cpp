#include "D2ItemTable.h"
#include "D2UsableItem.h"
#include "D2EquipItem.h"
#include "Resource/ResourceManager.h"
#include "Resource/Excel/ExcelData.h"
#include "../D2Util.h"

DEFINITION_SINGLE(CD2ItemTable)

CD2ItemTable::CD2ItemTable()
{
}

CD2ItemTable::~CD2ItemTable()
{
	auto iter = mMapItemInfo.begin();
	auto iterEnd = mMapItemInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mMapItemInfo.clear();
}

bool CD2ItemTable::Init()
{
	return true;
}

void CD2ItemTable::Start()
{
	if (!loadTable())
	{
		assert(false);
		return;
	}
}

CD2Item* CD2ItemTable::GenerateItem(const std::string& name)
{
	auto iter = mMapItemInfo.find(name);

	if (mMapItemInfo.end() == iter)
	{
		return nullptr;
	}

	CD2Item* item = nullptr;

	switch (iter->second->eUseType)
	{
	case eD2ItemUseType::None:
	{
		item = new CD2Item;
		item->mInfo = *iter->second;
		return item;
	}
	case eD2ItemUseType::Equip:
	{
		item = new CD2EquipItem;
		item->mInfo = *iter->second;
		return (CD2Item*)(item);
	}
	case eD2ItemUseType::Usable:
		item = new CD2UsableItem;
		item->mInfo = *iter->second;
		return (CD2Item*)(item);
	}

	assert(false);
	return nullptr;
}


CTexture* CD2ItemTable::FindItemUITexture(CD2Item* item)
{
	std::string textureName = item->GetName() + "UITexture";
	return CResourceManager::GetInst()->FindTexture(textureName);
}

CTexture* CD2ItemTable::FindItemObjTexture(CD2Item* item)
{
	std::string textureName = item->GetName() + "ObjTexture";
	return CResourceManager::GetInst()->FindTexture(textureName);
}

bool CD2ItemTable::loadTable()
{
	std::string outName;
	CResourceManager::GetInst()->LoadCSV(outName, "D2ItemTable.csv");

	std::unordered_map<std::string, std::vector<std::string>*> table = CResourceManager::GetInst()->FindCSV(outName)->GetTable();

	D2ItemInfo* info = nullptr;

	auto iter = table.begin();
	auto iterEnd = table.end();

	for (; iter != iterEnd; ++iter)
	{
		std::vector<std::string>* row = iter->second;

		info = new D2ItemInfo;
		info->Name = (*row)[eCSVLabel::Name];
		info->UniqueID = std::stoi((*row)[eCSVLabel::UniqueID]);
		info->eType = CD2Util::StringToItemType((*row)[eCSVLabel::Type]);
		info->CellX = std::stoi((*row)[eCSVLabel::CellXWidth]);
		info->CellY = std::stoi((*row)[eCSVLabel::CellYWidth]);
		info->eUseType = CD2Util::StringToItemUseType((*row)[eCSVLabel::UseType]);
		info->Hp = std::stoi((*row)[eCSVLabel::Hp]);
		info->Mp = std::stoi((*row)[eCSVLabel::Mp]);
		info->Melee = std::stoi((*row)[eCSVLabel::Melee]);
		info->Magic = std::stoi((*row)[eCSVLabel::Magic]);

		// �ؽ��� �ε�
		std::string uiTexturePath = (*row)[eCSVLabel::UITexturePath];
		TCHAR convertName[256] = {};
		int length = MultiByteToWideChar(CP_ACP, 0, uiTexturePath.c_str(), -1, nullptr, 0);
		MultiByteToWideChar(CP_ACP, 0, uiTexturePath.c_str(), length, convertName, length);

		// ������ UI �ؽ���
		std::string uiTextureName = info->Name + "UITexture";
		CResourceManager::GetInst()->LoadTexture(uiTextureName, convertName);

		std::string objTexturePath = (*row)[eCSVLabel::ObjTexturePath];
		length = MultiByteToWideChar(CP_ACP, 0, objTexturePath.c_str(), -1, nullptr, 0);
		MultiByteToWideChar(CP_ACP, 0, objTexturePath.c_str(), length, convertName, length);

		// ������ ������Ʈ �ؽ���
		std::string objTextureName = info->Name + "ObjTexture";
		CResourceManager::GetInst()->LoadTexture(objTextureName, convertName);

		mMapItemInfo.insert(std::make_pair(iter->first, info));
	}
	
	return true;
}
