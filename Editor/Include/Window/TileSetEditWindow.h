#pragma once

#include "IMGUIWindow.h"
#include "Resource/TileSet/TileSet.h"

class CTileSetEditWindow :
    public CIMGUIWindow
{
public:
    CTileSetEditWindow();
    ~CTileSetEditWindow();

public:
    virtual bool Init() override;

public:
    void Clear();

public:
    void OnClickLoadImage();
    void OnChangeSplitSize(int val[2]);
    void OnClickSplit();
    void OnSelectTileName(int idx, const char* label);
    void OnClickOpenCreateTile();
    void OnClickDeleteTile();
    void OnClickRename();
    void OnSelectTileType(int idx, const char* label);
    void OnClickSaveCSV();
    void OnClickLoadCSV();

private:
    // TileMap Creation Widget
    class CIMGUIButton* mLoadImageButton;
    class CIMGUIImage* mAtlasImage;
    class CIMGUIImage* mCropImage;
    class CIMGUIText* mAtlasSizeXText;
    class CIMGUIText* mAtlasSizeYText;
    class CIMGUIComboBox* mTileShapeList;
    class CIMGUIInputInt2* mSplitSizeInput;
    class CIMGUIButton* mSplitButton;

    // Tile Info Widget
    class CIMGUIComboBox* mTileNameList;
    class CIMGUIButton* mCreateTileButton;
    class CIMGUIButton* mDeleteTileButton;
    class CIMGUIImage* mTileImage;
    class CIMGUITextInput* mTileNameInput;
    class CIMGUIButton* mRenameButton;
    class CIMGUITextInput* mTileShapeWidget;
    class CIMGUIComboBox* mTileTypeList;
    class CIMGUIInputInt2* mTileImageStartInput;
    class CIMGUIInputInt2* mTileImageEndInput;

    class CIMGUITextInput* mTileSetNameInput;
    class CIMGUIButton* mSaveCSVButton;
    class CIMGUIButton* mLoadCSVButton;

    class CIMGUIPopUpModal* mCreateTilePopUp;

private:
    CSharedPtr<CTileSet> mTileSet;
};

