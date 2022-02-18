#pragma once

#include "IMGUIWindow.h"
#include "Resource/Texture/Texture.h"

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
    TileSetInfo* FindInfo(const std::string& name);
    bool DeleteInfo(const std::string& name);

private:
    class CIMGUIButton* mLoadImageButton;
    class CIMGUIImage* mAtlasImage;
    class CIMGUIImage* mCropImage;
    class CIMGUIText* mAtlasSizeXText;
    class CIMGUIText* mAtlasSizeYText;
    class CIMGUIInputInt2* mSplitSizeInput;
    class CIMGUIButton* mSplitButton;

    class CIMGUIComboBox* mTileNameList;
    class CIMGUIButton* mOpenCreateTileButton;
    class CIMGUIButton* mDeleteTileButton;
    class CIMGUIImage* mTileImage;
    class CIMGUITextInput* mTileNameInput;
    class CIMGUIButton* mRenameButton;
    class CIMGUIComboBox* mTileTypeList;
    class CIMGUIInputInt2* mTileImageStartInput;
    class CIMGUIInputInt2* mTileImageEndInput;

    class CIMGUIButton* mSaveCSVButton;
    class CIMGUIButton* mLoadCSVButton;

    class CIMGUIPopUpModal* mCreateTilePopUp;

private:
    CSharedPtr<CTexture> mTextureAtlas;
    char mTextureFullPath[MAX_PATH];
    std::unordered_map<std::string, TileSetInfo*> mMapTileSet;
    class CExcelData* mCSV;
};

