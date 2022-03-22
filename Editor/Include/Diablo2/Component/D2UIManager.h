#pragma once

#include "GameInfo.h"

class CD2UIManager
{
public:
	virtual bool Init();
	virtual void Start();

public:
	void OpenInventory();
	void CloseInventory();
	void ToggleInventory(float deltaTime);

private:
	void makeWindow();
	void makeWidget();

	DECLARE_SINGLE(CD2UIManager)

private:
	class CD2StatusBarWindow* mStatusBar;
	class CD2MenuBorder* mMenuBorder;
	class CD2InventoryWindow* mInven;
};

