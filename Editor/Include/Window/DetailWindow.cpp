#include "DetailWindow.h"
#include "EditorMenuWindow.h"
#include "IMGUITree.h"
#include "IMGUIInputFloat.h"
#include "IMGUIText.h"
#include "IMGUIChild.h"
#include "IMGUIListBox.h"
#include "IMGUIColor4.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIManager.h"
#include "ObjectHierachyWindow.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "../WIdget/ComponentControlWidget.h"
#include "../Widget/SpriteControlWidget.h"

CDetailWindow::CDetailWindow()
{
}

CDetailWindow::~CDetailWindow()
{
}

bool CDetailWindow::Init()
{
    CIMGUIWindow::Init();

    return true;
}

void CDetailWindow::Update(float deltaTime)
{
    CIMGUIWindow::Update(deltaTime);
}

void CDetailWindow::UpdateComponentUI(class CGameObject* obj)
{
    // 기존 Component List 비우기
    ClearWidget();

    // 오브젝트가 가진 모든 컴포넌트 포인터
    std::vector<CComponent*> vecComponents;
    obj->GetAllComponentsPointer(vecComponents);

    size_t size = vecComponents.size();

    for (size_t i = 0; i < size; ++i)
    {
        // Component List에 종류 맞춰 추가
        CComponentControlWidget* widget = CreateControlWidget(vecComponents[i]);
        widget->mComponent = vecComponents[i];
        widget->Init();
        mVecWidget.push_back((CIMGUIWidget*)widget);
    }
}

CComponentControlWidget* CDetailWindow::CreateControlWidget(CComponent* comp)
{
    if (comp->GetTypeID() == typeid(CSpriteComponent).hash_code())
    {
        return (CComponentControlWidget*)(new CSpriteControlWidget);
    }
    else
    {
        assert(false);
        return nullptr;
    }
}
