#include "IMGUIPopUpModal.h"

CIMGUIPopUpModal::CIMGUIPopUpModal()    :
    mSize(0)
{
}

CIMGUIPopUpModal::~CIMGUIPopUpModal()
{
    size_t size = mVecChild.size();

    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(mVecChild[i]);
    }
}

bool CIMGUIPopUpModal::Init()
{
    return true;
}

void CIMGUIPopUpModal::Render()
{
    if (ImGui::BeginPopupModal(mName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
		size_t size = mVecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
            mVecChild[i]->Render();
		}

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
