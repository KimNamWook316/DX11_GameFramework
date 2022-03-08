#include "../D2Info.h"

class CD2DataManager
{
	friend class CD2DataManagerComponent;

public:
	virtual bool Init();
	virtual void Start();

public:
	D2CharInfo* FindCharcterInfo(const std::string& name);
	void GetCharacterNames(std::vector<std::string>& charNames);

public:
	D2SkillInfo* FindSkillInfo(const std::string& name);
	void GetSkillNames(std::vector<std::string>& skillNames);

private:
	bool loadD2CharInfo();
	bool loadD2SkillInfo();

	DECLARE_SINGLE(CD2DataManager)

public:
	std::unordered_map<std::string, D2CharInfo*> mMapCharInfo;
	std::unordered_map<std::string, D2SkillInfo*> mMapSkillInfo;
};