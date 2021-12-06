#include "IMGUIWindow.h"
#include "IMGUIWidget.h"

CIMGUIWindow::CIMGUIWindow() :
    mbOpen(true),
    mWindowFlag(0),
    mbModalPopUp(false)
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
    if (!mbOpen)
    {
        return;
    }

    if (!ImGui::Begin(mName.c_str(), &mbOpen, mWindowFlag))
    {
        mbOpen = false;
    }

    size_t size = mVecWidget.size();

    for (size_t i = 0; i < size; ++i)
    {
        mVecWidget[i]->Render();
    }

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
