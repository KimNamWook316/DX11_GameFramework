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
    void OnChangeTileSize(int val[2]);
    void OnChangeTileShape(int idx, const char* label);
    void OnChangeRoomSizeMin(int val);
    void OnChangeTileCount(int val[2]);
    void OnChangePartitionLevel(int val);

private:
    class CIMGUIInputInt2* mTileSizeInput;
    class CIMGUIComboBox* mTileShapeComboBox;
    class CIMGUIInputInt* mRoomSizeMinInput;
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

    bool mbGenerationStep[(int)eGenerationStep::Max];
};

