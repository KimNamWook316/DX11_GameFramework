#include "TileSetManager.h"

CTileSetManager::CTileSetManager()
{
}

CTileSetManager::~CTileSetManager()
{
}

bool CTileSetManager::Init()
{
	return true;
}

bool CTileSetManager::CreateTileSet(const std::string& name)
{
	CTileSet* found = FindTileSet(name);

	if (found)
	{
		return true;
	}

	found = new CTileSet;

	if (!found->Init())
	{
		return false;
	}

	found->mName = name;

	return true;
}

CTileSet* CTileSetManager::FindTileSet(const std::string& name)
{
	auto found = mMapTileSet.find(name);

	if (found == mMapTileSet.end())
	{
		return nullptr;
	}

	return found->second;
}

void CTileSetManager::ReleaseTileSet(const std::string& name)
{
	auto found = mMapTileSet.find(name);

	if (found != mMapTileSet.end())
	{
		if (found->second->GetRefCount() == 1)
		{
			mMapTileSet.erase(found);
		}
	}
}

bool CTileSetManager::SaveTileSet(const std::string& name, const char* fileName, const std::string& pathName)
{
	CTileSet* found = FindTileSet(name);

	if (!found)
	{
		return false;
	}

	return found->SaveCSV(fileName, pathName);
}

bool CTileSetManager::SaveTileSetFullPath(const std::string& name, const char* fullPath)
{
	CTileSet* found = FindTileSet(name);

	if (!found)
	{
		return false;
	}

	return found->SaveCSVFullPath(fullPath);
}

bool CTileSetManager::LoadTileSet(const char* fileName, const std::string& pathName)
{
	CTileSet* tileSet = new CTileSet;

	if (!tileSet->Init())
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (!tileSet->LoadCSV(fileName, pathName))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (FindTileSet(tileSet->GetName()))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	mMapTileSet.insert(std::make_pair(tileSet->GetName(), tileSet));
	return true;
}

bool CTileSetManager::LoadTileSet(std::string& outName, const char* fileName, const std::string& pathName)
{	
	CTileSet* tileSet = new CTileSet;

	if (!tileSet->Init())
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (!tileSet->LoadCSV(fileName, pathName))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (FindTileSet(tileSet->GetName()))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	mMapTileSet.insert(std::make_pair(tileSet->GetName(), tileSet));
	outName = tileSet->GetName();
	return true;
}

bool CTileSetManager::LoadTileSetFullPath(const char* fullPath)
{
	CTileSet* tileSet = new CTileSet;

	if (!tileSet->Init())
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (!tileSet->LoadCSVFullPath(fullPath))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (FindTileSet(tileSet->GetName()))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	mMapTileSet.insert(std::make_pair(tileSet->GetName(), tileSet));
	return true;
}

bool CTileSetManager::LoadTileSetFullPath(std::string& outName, const char* fullPath)
{
	CTileSet* tileSet = new CTileSet;

	if (!tileSet->Init())
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (!tileSet->LoadCSVFullPath(fullPath))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	if (FindTileSet(tileSet->GetName()))
	{
		SAFE_DELETE(tileSet);
		return false;
	}

	mMapTileSet.insert(std::make_pair(tileSet->GetName(), tileSet));
	outName = tileSet->GetName();
	return true;
}
