#include "ObjectHierachyWindow.h"
#include "IMGUIChild.h"
#include "IMGUIListBox.h"
#include "IMGUIText.h"
#include "IMGUISameLine.h"
#include "IMGUIButton.h"
#include "IMGUIDummy.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneManager.h"
#include "DetailWindow.h"
#include "IMGUIManager.h"
#include "Scene/SceneManager.h"
#include "DetailWindow.h"
#include "../EditorInfo.h"

CObjectHierachyWindow::CObjectHierachyWindow()	:
	mObjectListBox(nullptr),
	mComponentListBox(nullptr),
	mDeleteObjectButton(nullptr),
	mDeleteComponentButton(nullptr)
{
}

CObjectHierachyWindow::~CObjectHierachyWindow()
{
}

bool CObjectHierachyWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUIChild* child = AddWidget<CIMGUIChild>("hierachyChild1", 150.f, 550.f);
	CIMGUIText* text = child->AddWidget<CIMGUIText>("text");
	text->SetText("GameObjects");
	mObjectListBox = child->AddWidget<CIMGUIListBox>("objectList", 150.f, 500.f);
	mObjectListBox->SetSelectCallBack(this, &CObjectHierachyWindow::OnSelectObject);
	mObjectListBox->SetHideName(true);
	mObjectListBox->SetPageItemCount(30);

	AddWidget<CIMGUISameLine>("line");

	child = AddWidget<CIMGUIChild>("hierachyChild2", 150.f, 550.f);
	text = child->AddWidget<CIMGUIText>("text");
	text->SetText("Components");
	mComponentListBox = child->AddWidget<CIMGUIListBox>("componentList", 150.f, 500.f);
	mComponentListBox->SetSelectCallBack(this, &CObjectHierachyWindow::OnSelectComponent);
	mComponentListBox->SetHideName(true);
	mComponentListBox->SetPageItemCount(30);

	mDeleteObjectButton = AddWidget<CIMGUIButton>("Delete Object", 0.f, 0.f);
	mDeleteObjectButton->SetClickCallBack(this, &CObjectHierachyWindow::OnClickDeleteObject);
	AddWidget<CIMGUISameLine>("line");
	CIMGUIDummy* dummy = AddWidget<CIMGUIDummy>("dummy");
	dummy->SetSize(100.f, 0.f);
	AddWidget<CIMGUISameLine>("line");
	mDeleteComponentButton = AddWidget<CIMGUIButton>("Delete Component", 0.f, 0.f);
	mDeleteComponentButton->SetClickCallBack(this, &CObjectHierachyWindow::OnClickDeleteComponent);

	return true;
}

void CObjectHierachyWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CObjectHierachyWindow::AddObjectList(const char* name)
{
	mObjectListBox->AddItem(name);
}

void CObjectHierachyWindow::RefreshObjectList()
{
	mObjectListBox->Clear();

	std::vector<std::string> objNames;

	CSceneManager::GetInst()->GetScene()->GetObjNames(objNames);

	size_t size = objNames.size();

	for (size_t i = 0; i < size; ++i)
	{
		AddObjectList(objNames[i].c_str());
	}
}

void CObjectHierachyWindow::RefreshComponentList()
{
	mComponentListBox->Clear();

	std::vector<FindComponentName> names;

	CGameObject* obj = CSceneManager::GetInst()->GetScene()->FindObject(mObjectListBox->GetSelectItem());
	obj->GetAllComponentsName(names);

	size_t size = names.size();

	for (size_t i = 0; i < size; ++i)
	{
		mComponentListBox->AddItem(names[i].Name);
	}

	CDetailWindow* inspector = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(INSPECTOR_WINDOW_NAME);
	inspector->UpdateComponentUI(obj);
}

void CObjectHierachyWindow::OnSelectObject(int idx, const char* item)
{
	CGameObject* obj = CSceneManager::GetInst()->GetScene()->FindObject(item);

	if (!obj)
	{
		return;
	}

	std::vector<FindComponentName> vecNames;

	obj->GetAllComponentsName(vecNames);
	
	mComponentListBox->Clear();

	size_t size = vecNames.size();

	for (size_t i = 0; i < size; ++i)
	{
		mComponentListBox->AddItem(vecNames[i].Name);
	}

	// Update Inspector Window
	CDetailWindow* inspectorWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Inspector");
	inspectorWindow->UpdateComponentUI(obj);
}

void CObjectHierachyWindow::OnSelectComponent(int idx, const char* item)
{
}

void CObjectHierachyWindow::OnClickDeleteObject()
{
	if (-1 == mObjectListBox->GetSelectIndex())
	{
		return;
	}
	
	CGameObject* obj = CSceneManager::GetInst()->GetScene()->FindObject(mObjectListBox->GetSelectItem());

	if (obj)
	{
		obj->Destroy();
		mObjectListBox->DeleteItem(mObjectListBox->GetSelectIndex());
		mComponentListBox->Clear();
	}
}

void CObjectHierachyWindow::OnClickDeleteComponent()
{
	if (-1 == mComponentListBox->GetSelectIndex())
	{
		return;
	}

	CGameObject* obj = CSceneManager::GetInst()->GetScene()->FindObject(mObjectListBox->GetSelectItem());
	obj->DeleteComponent(mComponentListBox->GetSelectItem());

	RefreshComponentList();
}
