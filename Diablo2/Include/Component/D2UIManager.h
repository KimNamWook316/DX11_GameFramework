#pragma once

#include "GameInfo.h"
#include "../D2Info.h"

class CD2UIManager
{
public:
	virtual bool Init();
	virtual void Start();

public:
	void OpenMenu(eD2MenuWindowType type);
	void CloseMenu();
	void ToggleMenu(eD2MenuWindowType type);
	void ToggleInventory(float deltaTime);
	void ToggleSkillTree(float deltaTime);
	void EnableMonsterStateWindow(bool bEnable);
	void UpdateMonsterStateWindow(const std::string& monsterName, const float hpPercent);
	void EnableClickableUI(bool bEnable);

public:
	void OnESCDown(float deltaTime);
	void OnTABDown(float deltaTime);
	void OnTABUp(float deltaTime);

public:
	class CD2StatusBarWindow* GetStatusBarWindow()
	{
		return mStatusBar;
	}

	class CD2InventoryWindow* GetInventoryWindow();

	bool IsTABDown() const
	{
		return mbTABDown;
	}

public:
	void AddClickableUI(class CD2ClickableUIComponent* ui)
	{
		mClickableUIList.push_back(ui);
	}

	bool DeleteClickableUI(class CD2ClickableUIComponent* ui);

private:
	void makeWindow();
	void makeWidget();

	DECLARE_SINGLE(CD2UIManager)

private:
	class CWidgetWindow* mMenuWindow[(int)eD2MenuWindowType::Max];
	class CWidgetWindow* mCurOpenWindow;
	class CWidgetWindow* mPrevOpenWindow;

	class CD2StatusBarWindow* mStatusBar;
	class CD2MenuBorder* mMenuBorder;
	class CD2MonsterStatusWindow* mMonsterStatus;

	std::list<CD2ClickableUIComponent*> mClickableUIList;
	bool mbTABDown;
};

