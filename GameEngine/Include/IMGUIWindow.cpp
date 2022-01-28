#include "IMGUIWindow.h"
#include "IMGUIWidget.h"
#include "IMGUIManager.h"

CIMGUIWindow::CIMGUIWindow() :
    mbOpen(true),
    mWindowFlag(ImGuiWindowFlags_NoCollapse),
    mbModalPopUp(false),
    mbIsFocused(false)
{
}

CIMGUIWindow::~CIMGUIWindow()
{
    size_t size = mVecPopUpWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(mVecPopUpWidget[i]);
    }

    size = mVecWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(mVecWidget[i]);
    }

    mVecWidget.clear();
}

bool CIMGUIWindow::Init()
{
    return true;
}

void CIMGUIWindow::Update(float deltaTime)
{
    // 윈도우가 열린 상태가 아니라면 렌더하지 않음
    if (!mbOpen)
    {
        return;
    }
    
    // 새로운 IMGUI 윈도우 시작, 닫힌 상태라면 윈도우 open -> false;
    if (!ImGui::Begin(mName.c_str(), &mbOpen, mWindowFlag))
    {
        // 필요한가?
        // mbOpen = false;
        ImGui::End();
        return;
    }

    if (ImGui::IsWindowFocused())
    {
        mbIsFocused = true;
    }
    else
    {
        mbIsFocused = false;
    }

    //CIMGUIManager::GetInst()->PushCurrentFont();
    
    // Widget들 모두 Render
    size_t size = mVecWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        mVecWidget[i]->Render();
    }
    
    //CIMGUIManager::GetInst()->PopCurrentFont();

    // Begin을 하면 End를 해 주어야 한다.
    ImGui::End();
}

void CIMGUIWindow::Open()
{
    mbOpen = true;
}

void CIMGUIWindow::Close()
{
    mbOpen = false;
}

void CIMGUIWindow::ClosePopUp()
{
    ImGui::CloseCurrentPopup();

    size_t size = mVecPopUpWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(mVecPopUpWidget[i]);
    }

    mVecPopUpWidget.clear();
}

void CIMGUIWindow::ClearWidget()
{
    size_t size = mVecPopUpWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(mVecPopUpWidget[i]);
    }

    size = mVecWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(mVecWidget[i]);
    }

    mVecWidget.clear();
}

CIMGUIWidget* CIMGUIWindow::FindWidget(const std::string& name)
{
    size_t size = mVecWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        if (mVecWidget[i]->mName == name)
        {
            return mVecWidget[i];
        }
    }
    return nullptr;
}

bool CIMGUIWindow::IsWindowFocused(ImGuiFocusedFlags_ eFlag) const
{
    return mbIsFocused;
}
