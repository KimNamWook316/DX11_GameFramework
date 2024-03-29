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
    void OnClickPlayAnimation();
    void OnClickStopAnimation();
    void OnWidthInputChanged(int val);
    void OnHeightInputChanged(int val);
    void OnStartXInputChanged(int val);
    void OnStartYInputChanged(int val);
    void OnClickLoopCheckBox(const char* name, bool bLoop);
    void OnClickReverseCheckBox(const char* name, bool bLoop);
    void OnPlayTimeInputChanged(float val);
    void OnPlayScaleInputChanged(float val);
    void OnClickSaveSequence();
    void OnClickSaveAllSequence();
    void OnClickLoadSequence();
    void OnClickSaveAnimation();
    void OnClickLoadAnimation();
    void OnDownReturnKey(float dummy);
    void OnClickAutoSplit();

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

private:
    void updateAnimList();
    void updateFrameList();

    void updateFrameUI();
    void updateAnimationUI();

    void clearFrameUI();
    void clearAnimationUI();

// Members
private:
    // Sprite Object
    CSharedPtr<class CSpriteEditObject> mSpriteEditObject;

    // Image
    class CIMGUIImage* mCropImage;
    class CIMGUIListBox* mAnimationList;
    class CIMGUITextInput* mAnimationNameInput;
    class CIMGUIListBox* mAnimationFrameList;

    // Auto Split Input
    class CIMGUITextInput* mAutoSplitNameInput;
    class CIMGUIInputInt* mSplitCountXInput;
    class CIMGUIInputInt* mSplitCountYInput;

    // Text
    class CIMGUIText* mSpriteWidthText;
    class CIMGUIText* mSpriteHeightText;

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
    
    // Anim Instance
    class CAnimationSequence2DInstance* mAnimationInstance;

    Vector2 mCropStartPos;
    Vector2 mCropEndPos;
};

