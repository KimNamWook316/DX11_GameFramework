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
#include "Component/TileMapComponent.h"
#include "Component/ProcedualMapGenerator.h"
#include "Component/NavAgentComponent.h"
#include "Component/StateComponent.h"
#include "Component/CameraComponent.h"
#include "../Diablo2/Component/D2StateComponent.h"
#include "../Diablo2/Component/D2CharacterInfoComponent.h"
#include "../Diablo2/Component/D2ProjectTile.h"
#include "../Diablo2/Component/D2MeleeAttack.h"
#include "../Diablo2/Component/D2PlayerSkillComponent.h"
#include "../Diablo2/Component/D2ProcedualMapGenerator.h"
#include "../Diablo2/Component/D2ShadowComponent.h"
#include "../Widget/ComponentControlWidget.h"
#include "../Widget/SpriteControlWidget.h"
#include "../Widget/ColliderControlWidget.h"
#include "../Widget/TileMapControlWidget.h"
#include "../Widget/MapGeneratorWidget.h"
#include "../Widget/StateControlWidget.h"
#include "../Widget/CameraControlWidget.h"
#include "../Widget/D2CharInfoWidget.h"
#include "../Widget/D2ProjectileWidget.h"
#include "../Widget/D2ProcedualMapGeneratorControlWidget.h"
#include "../Widget/D2ShadowComponentWidget.h"

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
    else if (comp->GetTypeID() == typeid(CTileMapComponent).hash_code())
    {
        CTileMapControlWidget* widget = new CTileMapControlWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CProcedualMapGenerator).hash_code())
    {
        CMapGeneratorWidget* widget = new CMapGeneratorWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CStateComponent).hash_code())
    {
        CStateControlWidget* widget = new CStateControlWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CD2StateComponent).hash_code())
    {
        CStateControlWidget* widget = new CStateControlWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CCameraComponent).hash_code())
    {
        CCameraControlWidget* widget = new CCameraControlWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CD2CharacterInfoComponent).hash_code())
    {
        CD2CharInfoWidget* widget = new CD2CharInfoWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CD2Projectile).hash_code())
    {
        CD2ProjectileWidget* widget = new CD2ProjectileWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CD2ProcedualMapGenerator).hash_code())
    {
        CD2ProcedualMapGeneratorControlWidget* widget = new CD2ProcedualMapGeneratorControlWidget;
        return widget;
    }
    else if (comp->GetTypeID() == typeid(CD2ShadowComponent).hash_code())
    {
        CD2ShadowComponentWidget* widget = new CD2ShadowComponentWidget;
        return widget;
    }
    else
    {
        CComponentControlWidget* widget = new CComponentControlWidget;
        return widget;
    }
}
