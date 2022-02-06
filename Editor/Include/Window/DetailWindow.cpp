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
    // ���� Component List ����
    ClearWidget();

    // ������Ʈ�� ���� ��� ������Ʈ ������
    std::vector<CComponent*> vecComponents;
    obj->GetAllComponentsPointer(vecComponents);

    size_t size = vecComponents.size();

    for (size_t i = 0; i < size; ++i)
    {
        // Component List�� ���� ���� �߰�
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
