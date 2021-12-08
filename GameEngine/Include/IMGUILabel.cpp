#include "IMGUILabel.h"

CIMGUILabel::CIMGUILabel()
{
	mAlign = ImVec2(0.f, 0.f);
}

CIMGUILabel::~CIMGUILabel()
{
}

bool CIMGUILabel::Init()
{
	return true;
}

void CIMGUILabel::Render()
{
	// style push
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, mAlign);

	// color push -> Label Button이므로 hover, click 모두 같은 색
	ImGui::PushStyleColor(ImGuiCol_Button, mColor.Value);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mColor.Value);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, mColor.Value);

	ImGui::Button(mName.c_str(), mSize);

	// pop
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
}
