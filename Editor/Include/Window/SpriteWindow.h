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
    void OnClickLoadTexture();
    void OnClickSpriteEdit();
    void OnClickAddAnimation();
    void OnClickAddAnimationFrame();

private:
    class CIMGUIImage* mImage;
    class CIMGUIListBox* mAnimationList;
    class CIMGUIListBox* mAnimationFrameList;
    CSharedPtr<class CSpriteEditObject> mSpriteEditObject;
};

