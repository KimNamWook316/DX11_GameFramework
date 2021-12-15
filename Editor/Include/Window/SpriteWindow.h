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

    void UpdateCropImage();
    
public:
    void OnClickLoadTexture();
    void OnClickSpriteEdit();
    void OnClickAddAnimation();
    void OnClickAddAnimationFrame();
    void OnSelectAnimationList(int idx, const char* item);
    void OnSelectAnimationFrame(int idx, const char* item);

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
    class CIMGUIImage* mImage;
    class CIMGUIImage* mCropImage;
    class CIMGUIListBox* mAnimationList;
    class CIMGUITextInput* mAnimationNameInput;
    class CIMGUIListBox* mAnimationFrameList;
    CSharedPtr<class CSpriteEditObject> mSpriteEditObject;

private:
    class CIMGUIText* mCropImageTextWidth;
    class CIMGUIText* mCropImageTextHeight;
    class CIMGUIText* mCropImageTextStart;
    class CIMGUIText* mCropImageTextEnd;
    Vector2 mCropStartPos;
    Vector2 mCropEndPos;
};

