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
    // �����찡 ���� ���°� �ƴ϶�� �������� ����
    if (!mbOpen)
    {
        return;
    }
    
    // ���ο� IMGUI ������ ����, ���� ���¶�� ������ open -> false;
    if (!ImGui::Begin(mName.c_str(), &mbOpen, mWindowFlag))
    {
        // �ʿ��Ѱ�?
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
    
    // Widget�� ��� Render
    size_t size = mVecWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        mVecWidget[i]->Render();
    }
    
    //CIMGUIManager::GetInst()->PopCurrentFont();

    // Begin�� �ϸ� End�� �� �־�� �Ѵ�.
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
