#include "Button.h"
#include "WidgetWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Viewport.h"
#include "../Input.h"

CButton::CButton()	:
	meCurState(eButtonState::Normal),
	mbHoverSoundPlaying(false),
	mbClickSoundPlaying(false)
{
}

CButton::CButton(const CButton& button)	:
	CWidget(button)
{
	meCurState = eButtonState::Normal;
	mClickCallBack = nullptr;
	mbHoverSoundPlaying = false;
	mbClickSoundPlaying = false;
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

	if (eButtonState::Disabled != meCurState)
	{
		if (mbMouseHovered)
		{
			if (!mbHoverSoundPlaying)
			{
				mbHoverSoundPlaying = true;

				if (mSound[(int)eButtonSoundState::Hovered])
				{
					mSound[(int)eButtonSoundState::Hovered]->Play();
				}
			}

			if (CInput::GetInst()->GetMouseLButtonClicked())
			{
				meCurState = eButtonState::Clicked;

				if (!mbClickSoundPlaying)
				{
					mbClickSoundPlaying = true;

					if (mSound[(int)eButtonSoundState::Clicked])
					{
						mSound[(int)eButtonSoundState::Clicked]->Play();
					}
				}
			}
			else if (eButtonState::Clicked == meCurState)
			{
				if (mClickCallBack)
				{
					mClickCallBack();
				}

				meCurState = eButtonState::Hovered;
				mbClickSoundPlaying = false;
			}
			else
			{
				meCurState = eButtonState::Hovered;
				mbClickSoundPlaying = false;
			}
		}
		else
		{
			mbClickSoundPlaying = false;
			mbHoverSoundPlaying = false;
			meCurState = eButtonState::Normal;
		}
	}
}

void CButton::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CButton::Render()
{
	if (mInfo[(int)meCurState].Texture)
	{
		mInfo[(int)meCurState].Texture->SetShader(0, (int)eConstantBufferShaderTypeFlags::Pixel, 0);
	}
	mTint = mInfo[(int)meCurState].Tint;

	CWidget::Render();
}

CButton* CButton::Clone()
{
	return new CButton(*this);
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

	SetUseTexture(true);
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
	
	SetUseTexture(true);
	return true;
}

void CButton::SetTextureTint(eButtonState state, const Vector4& tint)
{
	mInfo[(int)state].Tint = tint;
}

void CButton::SetTextureTint(eButtonState state, const float r, const float g, const float b, const float a)
{
	mInfo[(int)state].Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CButton::AddFrameData(eButtonState state, const Vector2& start, const Vector2& size)
{
	AnimationFrameData data;
	data.Start = start;
	data.Size = size;
	
	mInfo[(int)state].vecFrameData.push_back(data);
}

void CButton::SetSound(eButtonSoundState state, const std::string& soundName)
{
	mSound[(int)state] = mOwner->GetViewport()->GetScene()->GetResource()->FindSound(soundName);
}

void CButton::SetSound(eButtonSoundState state, CSound* sound)
{
	mSound[(int)state] = sound;
}

void CButton::SetSound(eButtonSoundState state, const std::string& channelGroupName, const std::string& soundName, const char* fileName, const std::string& pathName)
{
	mOwner->GetViewport()->GetScene()->GetResource()->LoadSound(channelGroupName, false, soundName, fileName, pathName);
	mSound[(int)state] = mOwner->GetViewport()->GetScene()->GetResource()->FindSound(soundName);
}
