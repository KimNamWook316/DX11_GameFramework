#pragma once

#include "ComponentControlWidget.h"

class CSpriteControlWidget :
    public CComponentControlWidget 
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CSpriteControlWidget();
    virtual ~CSpriteControlWidget();

public:
    virtual bool Init() override;

public:
    void OnChangePosition(float value[3]);
    void OnChangeScale(float value[3]);
    void OnChangeRot(float value[3]);
    void OnChangePivot(float value[3]);
    void OnChangeBaseColor(float val[4]);
    void OnChangeTransparency(const char* tag, bool bChecked);
    void OnChangeOpacity(float val);
    void OnChangeInheritScale(const char* tag, bool bChecked);
    void OnChangeInheritRot(const char* tag, bool bChecked);
    void OnChangeInheritPos(const char* tag, bool bChecked);
    void OnClickLoadAnimation();
    void OnChangeAnimationFrame(int frame);
    void OnClickPlayAnimation();
    void OnSelectAnimationList(int idx, const char* name);

    void OnUpdateAnimInstFrame();
    void OnChangeTransform();

protected:
    class CIMGUIInputFloat3* mPositionWidget;
    class CIMGUIInputFloat3* mScaleWidget;
    class CIMGUIInputFloat3* mRotWidget;
    class CIMGUIInputFloat3* mPivotWidget;
    class CIMGUIColor4* mColorWidget;
    class CIMGUICheckBox* mTransparencyWidget;
    class CIMGUISliderFloat* mOpacityWidget;
    class CIMGUICheckBox* mInheritScaleWidget;
    class CIMGUICheckBox* mInheritRotWidget;
    class CIMGUICheckBox* mInheritPosWidget;
    class CIMGUITree* mAnimationWidget;
    class CIMGUIButton* mLoadAnimationWidget;
    class CIMGUITextInput* mAnimationNameWidget;
    class CIMGUISliderInt* mAnimationFrameWidget;
    class CIMGUIButton* mAnimationPlayButton;
    class CIMGUIComboBox* mAnimationListWidget;

    //class CAnimationSequence2DInstance* mAnimInst;
};

