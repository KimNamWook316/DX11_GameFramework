#pragma once

#include "IMGUIWindow.h"

class CSpriteWindow :
    public CIMGUIWindow
{
public:
    CSpriteWindow();
    virtual ~CSpriteWindow();

public:
    virtual bool Init();
    virtual void Update(float deltaTime);

public:
    void UpdateCropImage();
    
// CallBacks
public:
    void OnClickLoadTexture();
    void OnClickSpriteEdit();
    void OnClickAddAnimation();
    void OnClickAddAnimationFrame();
    void OnClickDeleteAnimation();
    void OnClickDeleteAnimationFrame();
    void OnSelectAnimationList(int idx, const char* item);
    void OnSelectAnimationFrame(int idx, const char* item);

// Setter
public:
    void SetCropStartPos(const Vector2& pos)
    {
        mCropStartPos = pos;
    }

    void SetCropEndPos(const Vector2& pos)
    {
        mCropEndPos = pos;
    }

    void MoveCropPos(const float x, const float y);

// Members
private:
    class CIMGUIImage* mImage;
    class CIMGUIImage* mCropImage;
    class CIMGUIListBox* mAnimationList;
    class CIMGUITextInput* mAnimationNameInput;
    class CIMGUIListBox* mAnimationFrameList;
    CSharedPtr<class CSpriteEditObject> mSpriteEditObject;

    // Image Input
    class CIMGUIInputInt* mWidthInput;
    class CIMGUIInputInt* mHeightInput;
    class CIMGUIInputInt* mStartXInput;
    class CIMGUIInputInt* mStartYInput;

    // Anim Input
    class CIMGUICheckBox* mAnimLoopCheckBox;
    class CIMGUICheckBox* mAnimReverseCheckBox;
    class CIMGUIInputFloat* mPlayTimeInput;
    class CIMGUIInputFloat* mPlayScaleInput;

    Vector2 mCropStartPos;
    Vector2 mCropEndPos;
};

