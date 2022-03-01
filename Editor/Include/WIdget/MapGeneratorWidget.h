#pragma once

#include "ComponentControlWidget.h"

class CMapGeneratorWidget :
    public CComponentControlWidget
{
    friend class CDetailWindow;
    friend class CIMGUIWindow;

protected:
    CMapGeneratorWidget();
    virtual ~CMapGeneratorWidget();

public:
    virtual bool Init() override;

public:
    void OnClickNextStep();
    void OnClickCreateMap();
    void OnClickFindTileComponent();

private:
    class CIMGUIInputInt2* mTileCountInput;
    class CIMGUIInputInt* mPartitionLevelInput;
    class CIMGUIButton* mFindTileComponentButton;
    class CIMGUIButton* mNextStepButton;
    class CIMGUIButton* mCreateMapButton;
};

