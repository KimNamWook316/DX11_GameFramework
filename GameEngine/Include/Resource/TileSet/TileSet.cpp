#include "TileSet.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"
#include "../../PathManager.h"

CTileSet::CTileSet()	:
	mCSVFullPath{}
{
	SetTypeID<CTileSet>();
}

CTileSet::CTileSet(const CTileSet& tileSet)	:
	CRef(tileSet)
{
	mTextureAtlas = tileSet.mTextureAtlas;
	strcpy_s(mCSVFullPath, tileSet.mCSVFullPath);

	auto iter = mMapTileInfo.begin();
	auto iterEnd = mMapTileInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		TileSetInfo* info = new TileSetInfo;
		*info = *iter->second;

		mMapTileInfo.insert(std::make_pair(iter->first, info));
	}
}

CTileSet::~CTileSet()
{
	auto iter = mMapTileInfo.begin();
	auto iterEnd = mMapTileInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	mMapTileInfo.clear();
}

bool CTileSet::Init()
{
	return true;
}

TileSetInfo* CTileSet::FindInfo(const std::string& name)
{
	auto iter = mMapTileInfo.find(name);

	if (iter == mMapTileInfo.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool CTileSet::DeleteInfo(const std::string& name)
{
	auto iter = mMapTileInfo.begin();
	auto iterEnd = mMapTileInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->first == name)
		{
			SAFE_DELETE(iter->second);
			mMapTileInfo.erase(iter);
			return true;
		}
	}
	return false;
}

void CTileSet::AddTileSetInfo(const std::string& name, const eTileShape& shape, const eTileType& type, const Vector2& start, const Vector2& end)
{
	TileSetInfo* info = FindInfo(name);

	if (info)
	{
		return;
	}

	info = new TileSetInfo;
	info->Shape = shape;
	info->Type = type;
	info->ImageStart = start;
	info->ImageEnd = end;

	mMapTileInfo.insert(std::make_pair(name, info));
}

bool CTileSet::RenameTileSetInfo(const std::string& name, const std::string& changeName)
{
	TileSetInfo* info = FindInfo(name);

	if (!info)
	{
		return false;
	}
	
	TileSetInfo* newInfo = new TileSetInfo;
	newInfo->Type = info->Type;
	newInfo->Shape = info->Shape;
	newInfo->ImageStart = info->ImageStart;
	newInfo->ImageEnd= info->ImageEnd;
	
	DeleteInfo(name);

	mMapTileInfo.insert(std::make_pair(changeName, newInfo));
	return true;
}

bool CTileSet::ChangeType(const std::string& name, eTileType& type)
{
	TileSetInfo* info = FindInfo(name);

	if (!info)
	{
		return false;
	}

	info->Type = type;
	return true;
}

bool CTileSet::ChangeShape(const std::string& name, eTileShape& shape)
{
	TileSetInfo* info = FindInfo(name);

	if (!info)
	{
		return false;
	}

	info->Shape = shape;
	return true;
}

bool CTileSet::ChangeImageStart(const std::string& name, const Vector2& start)
{
	TileSetInfo* info = FindInfo(name);

	if (!info)
	{
		return false;
	}

	info->ImageStart = start;
	return true;
}

bool CTileSet::ChangeImageEnd(const std::string& name, const Vector2& end)
{
	TileSetInfo* info = FindInfo(name);

	if (!info)
	{
		return false;
	}

	info->ImageEnd = end;
	return true;
}

void CTileSet::Save(FILE* fp)
{
	CRef::Save(fp);

	int length = (int)strlen(mCSVFullPath);
	fwrite(mCSVFullPath, sizeof(char), length, fp);

	SaveCSVFullPath(mCSVFullPath);
}

void CTileSet::Load(FILE* fp)
{
	CRef::Load(fp);

	int length = 0;
	fread(&length, sizeof(int), 1, fp);
	fread(mCSVFullPath, sizeof(char), length, fp);
	
	LoadCSVFullPath(mCSVFullPath);
}

bool CTileSet::SaveCSV(const char* fileName, const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	return SaveCSVFullPath(fullPath);
}

bool CTileSet::SaveCSVFullPath(const char* fullPath)
{
	CResourceManager::GetInst()->CreateCSV(mName);
	CExcelData* csv = CResourceManager::GetInst()->FindCSV(mName);

	csv->AddLabel("TextureFullPath");
	csv->AddLabel("TileShape");
	csv->AddLabel("TileType");
	csv->AddLabel("StartUVX");
	csv->AddLabel("StartUVY");
	csv->AddLabel("EndUVX");
	csv->AddLabel("EndUVY");

	std::string fullPath = fullPath;
	csv->SetData("TextureInfo", "TextureFullPath", fullPath);

	std::vector<std::string> row;
	auto iter = mMapTileInfo.begin();
	auto iterEnd = mMapTileInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		row.clear();
		row.push_back("");
		row.push_back(CUtil::TileShapeToString(iter->second->Shape));
		row.push_back(CUtil::TileTypeToString(iter->second->Type));
		row.push_back(std::to_string(iter->second->ImageStart.x));
		row.push_back(std::to_string(iter->second->ImageStart.y));
		row.push_back(std::to_string(iter->second->ImageEnd.x));
		row.push_back(std::to_string(iter->second->ImageEnd.y));
		csv->SetData(iter->first, row);
	}

	bool success = csv->SaveCSVFullPath(fullPath);
	CResourceManager::GetInst()->DeleteCSV(mName);

	return success;
}

bool CTileSet::LoadCSV(const char* fileName, const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	return LoadCSVFullPath(fullPath);
}

bool CTileSet::LoadCSVFullPath(const char* fullPath)
{
	CResourceManager::GetInst()->CreateCSV(mName);
	CExcelData* csv = CResourceManager::GetInst()->FindCSV(mName);

	csv->LoadCSVFullPath(fullPath);

	mName = csv->GetName();

	std::string textureFullPath = csv->FindData("TextureInfo", "TextureFullPath");

	TCHAR convertTextureFullPath[MAX_PATH] = {};
	int length = MultiByteToWideChar(CP_ACP, 0, textureFullPath.c_str(), -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, textureFullPath.c_str(), length, convertTextureFullPath, 0);

	if (mScene)
	{
		if (!mScene->GetResource()->LoadTextureFullPath(mName, convertTextureFullPath))
		{
			CResourceManager::GetInst()->DeleteCSV(mName);
			assert(false);
			return false;
		}
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(mName, convertTextureFullPath))
		{
			CResourceManager::GetInst()->DeleteCSV(mName);
			assert(false);
			return false;
		}
	}

	csv->DeleteRow("TextureInfo");
	
	mTextureAtlas = CResourceManager::GetInst()->FindTexture(mName);

	std::unordered_map<std::string, std::vector<std::string>*> table = csv->GetTable();

	TileSetInfo* info = nullptr;
	auto iter = table.begin();
	auto iterEnd = table.end();

	for (; iter != iterEnd; ++iter)
	{
		info->Shape = CUtil::StringToTileShape((*iter->second)[1]);
		info->Type = CUtil::StringToTileType((*iter->second)[2]);

		Vector2 start(std::stof((*iter->second)[3]), std::stof((*iter->second)[4]));
		Vector2 end(std::stof((*iter->second)[5]), std::stof((*iter->second)[6]));

		info->ImageStart = start;
		info->ImageEnd = end;

		mMapTileInfo.insert(std::make_pair(iter->first, info));
	}

	CResourceManager::GetInst()->DeleteCSV(mName);

	strcpy_s(mCSVFullPath, fullPath);
	return true;
}

void CTileSet::GetTileNames(std::vector<std::string>& outNames)
{
	auto iter = mMapTileInfo.begin();
	auto iterEnd = mMapTileInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		outNames.push_back(iter->first);
	}
}

