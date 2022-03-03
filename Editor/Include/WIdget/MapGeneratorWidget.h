#pragma once

#include "ComponentControlWidget.h"

enum eGenerationStep
{
    SplitArea,
    MakeRoom,
    MakePath,
    MakeWall,
    Max
};

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
    void OnClickLoadTileSet();
    void OnClickLoadWallTileSet();

private:
    class CIMGUITextInput* mTileSetName;
    class CIMGUIButton* mLoadTileSetButton;
    class CIMGUIImage* mTileSetImage;
    class CIMGUITextInput* mWallTileSetName;
    class CIMGUIButton* mLoadWallTileSetButton;
    class CIMGUIImage* mWallTileSetImage;
    class CIMGUIInputInt2* mTileCountInput;
    class CIMGUIInputInt* mPartitionLevelInput;
    class CIMGUIButton* mNextStepButton;
    class CIMGUIButton* mCreateMapButton;

    CSharedPtr<class CTileSet> mTileSet;
    CSharedPtr<class CTileSet> mWallTileSet;

    bool mbGenerationStep[(int)eGenerationStep::Max];
};

