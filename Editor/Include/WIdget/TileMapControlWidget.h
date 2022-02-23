#pragma once

#include "ComponentControlWidget.h"
#include "Resource/TileSet/TileSet.h"

class CTileMapControlWidget :
    public CComponentControlWidget
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CTileMapControlWidget();
    virtual ~CTileMapControlWidget();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    void OnClickOpenTileSetEdit();
    void OnClickLoadTileSet();
    void OnClickCreateMap();
    void OnSelectTileList(int idx, const char* label);
    void OnClickDefaultFrame();

private:
    void clearWidget();

private:
    class CIMGUIButton* mOpenTileSetEditButton;

    class CIMGUITextInput* mTileSetNameWidget;
    class CIMGUIButton* mTileSetLoadButton;
    class CIMGUIImage* mTileSetPreviewImage;
    class CIMGUIInputInt2* mTileCountWidget;
    class CIMGUIInputInt2* mTileSizeWidget;
    class CIMGUIButton* mCreateMapButton;
    
    class CIMGUIComboBox* mTileSetTileList;
    class CIMGUIImage* mTileImageWidget;
    class CIMGUITextInput* mTileShapeWidget;
    class CIMGUITextInput* mTileTypeWidget;
    class CIMGUIButton* mDefaultFrameButon;

    TileSetInfo* mSelectInfo;
    bool mbIsTileMapCreate;
};

