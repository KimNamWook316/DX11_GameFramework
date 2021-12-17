#include "ObjectHierachyWindow.h"
#include "IMGUIChild.h"
#include "IMGUIListBox.h"
#include "IMGUIText.h"
#include "IMGUISameLine.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneManager.h"

CObjectHierachyWindow::CObjectHierachyWindow()	:
	mObjectListBox(nullptr),
	mComponentListBox(nullptr)
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

void CObjectHierachyWindow::OnSelectObject(int idx, const char* item)
{
	CGameObject* obj = CSceneManager::GetInst()->GetScene()->FindObject(item);

	if (!obj)
	{
		return;
	}

	std::vector<FindComponentName> vecNames;

	obj->GetAllSceneComponentsName(vecNames);
	
	mComponentListBox->Clear();

	size_t size = vecNames.size();

	for (size_t i = 0; i < size; ++i)
	{
		mComponentListBox->AddItem(vecNames[i].Name);
	}
}

void CObjectHierachyWindow::OnSelectComponent(int idx, const char* item)
{
}
