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
	SetTypeID<CButton>();
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
	for (int i = 0; i < (int)eButtonState::Max; ++i)
	{
		SAFE_DELETE(mImageData[i]);
	}
}

bool CButton::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}

	for (int i = 0; i < (int)eButtonState::Max; ++i)
	{
		mImageData[i] = new CWidgetImageData;
		mImageData[i]->SetOwnerWidget(this);
		mImageData[i]->Init();
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

	// Sound
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

	// Animation
	for (int i = 0; i < (int)eButtonState::Max; ++i)
	{
		mImageData[i]->Update(deltaTime);
	}
}

void CButton::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CButton::Render()
{
	// 텍스쳐, 애니메이션 처리
	mTint = mImageData[(int)meCurState]->GetImageTint();
	mImageData[(int)meCurState]->SetShaderData();

	// 실제 렌더
	CWidget::Render();
}

CButton* CButton::Clone()
{
	return new CButton(*this);
}

bool CButton::SetTexture(eButtonState state, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	mImageData[(int)state]->SetTexture(name, fileName, pathName);
	SetUseTexture(true);
	return true;
}

bool CButton::SetTextureFullPath(eButtonState state, const std::string& name, const TCHAR* fullPath)
{
	mImageData[(int)state]->SetTexture(name, fullPath);
	SetUseTexture(true);
	return true;
}

void CButton::SetTextureTint(eButtonState state, const Vector4& tint)
{
	mImageData[(int)state]->SetTextureTint(tint);
}

void CButton::SetTextureTint(eButtonState state, const float r, const float g, const float b, const float a)
{
	mImageData[(int)state]->SetTextureTint(r,g,b,a);
}

void CButton::AddFrameData(eButtonState state, const Vector2& start, const Vector2& size)
{
	mImageData[(int)state]->AddFrameData(start, size);
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
