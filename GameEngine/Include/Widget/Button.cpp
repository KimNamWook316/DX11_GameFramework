#include "Button.h"
#include "WidgetWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Viewport.h"

CButton::CButton()	:
	meCurState(eButtonState::Normal)
{
}

CButton::CButton(const CButton& button)	:
	CWidget(button)
{
	meCurState = eButtonState::Normal;
}

CButton::~CButton()
{
}

bool CButton::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}
	return true;
}

void CButton::Start()
{
	CWidget::Start();
}

void CButton::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
}

void CButton::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CButton::Render()
{
	CWidget::Render();
}

bool CButton::SetTexture(eButtonState state, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CSceneResource* sceneResource = mOwner->GetViewport()->GetScene()->GetResource();

	if (!sceneResource->LoadTexture(name, fileName, pathName))
	{
		assert(false);
		return false;
	}
	
	mInfo[(int)state].Texture = sceneResource->FindTexture(name);
	return true;
}

bool CButton::SetTextureFullPath(eButtonState state, const std::string& name, const TCHAR* fullPath)
{
	CSceneResource* sceneResource = mOwner->GetViewport()->GetScene()->GetResource();

	if (!sceneResource->LoadTextureFullPath(name, fullPath))
	{
		assert(false);
		return false;
	}
	
	mInfo[(int)state].Texture = sceneResource->FindTexture(name);
	return true;
}

void CButton::SetTextureTint(eButtonState state, const Vector4& tint)
{
	mInfo[(int)state].Tint = tint;
}

void CButton::SetTextureTint(eButtonState state, const float r, const float g, const float b, const float a)
{
	mInfo[(int)state].Tint = Vector4(r, g, b, a);
}

void CButton::AddFrameData(eButtonState state, const Vector2& start, const Vector2& size)
{
	AnimationFrameData data;
	data.Start = start;
	data.Size = size;
	
	mInfo[(int)state].vecFrameData.push_back(data);
}
