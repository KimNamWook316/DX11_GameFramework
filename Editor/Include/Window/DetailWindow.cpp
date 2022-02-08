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
#include "Component/ColliderCircle.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderPixel.h"
#include "../Widget/ComponentControlWidget.h"
#include "../Widget/SpriteControlWidget.h"
#include "../Widget/ColliderControlWidget.h"

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
    else if (comp->GetTypeID() == typeid(CColliderBox2D).hash_code())
    {
        CColliderControlWidget* widget = new CColliderControlWidget;
        widget->meType = eColliderType::Box2D;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CColliderCircle).hash_code())
    {
        CColliderControlWidget* widget = new CColliderControlWidget;
        widget->meType = eColliderType::Circle2D;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CColliderPixel).hash_code())
    {
        CColliderControlWidget* widget = new CColliderControlWidget;
        widget->meType = eColliderType::Pixel;
        return widget;
    }
    else
    {
        assert(false);
        return nullptr;
    }
}
