#include "IMGUITree.h"

CIMGUITree::CIMGUITree()	:
	mParent(nullptr)
{
}

CIMGUITree::~CIMGUITree()
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecChild[i]);
	}

	size = mVecWidget.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecWidget[i]);
	}
}

CIMGUITree* CIMGUITree::GetNode(const std::string& name)
{
	if (mName == name)
	{
		return this;
	}

	size_t size = mVecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		CIMGUITree* result = mVecChild[i]->GetNode(name);

		if (result)
		{
			return result;
		}
	}

	return nullptr;
}

CIMGUITree* CIMGUITree::GetNode(const int idx)
{
	return mVecChild[idx];
}

CIMGUITree* CIMGUITree::AddChild(const std::string& name, const float width, const float height)
{
	CIMGUITree* tree = new CIMGUITree;

	tree->SetName(name);
	tree->SetOwner(mOwner);
	tree->SetSize(width, height);
	tree->SetParent(this);

	if (!tree->Init())
	{
		SAFE_DELETE(tree);
		return nullptr;
	}
	mVecChild.push_back(tree);
	mVecRender.push_back(tree);
	return tree;
}

bool CIMGUITree::Init()
{
	return true;
}

void CIMGUITree::Render()
{
	if (ImGui::TreeNode(mName.c_str()))
	{
		size_t size = mVecRender.size();

		for (size_t i = 0; i < size; ++i)
		{
			mVecRender[i]->Render();
		}
		ImGui::TreePop();
	}
}
