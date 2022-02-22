#pragma once

#include "ComponentControlWidget.h"

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

public:
    void OnClickLoadImage();
    void OnClickCreateMap();
    void OnSelectType(int idx, const char* label);
    void OnClickDefaultFrame();
    void OnClickOpenTileSetEdit();

private:
    class CIMGUITextInput* mTileSetNameWidget;
    class CIMGUIButton* mTileSetLoadButton;
    class CIMGUIInputInt2* mTileCountWidget;
    class CIMGUIInputInt2* mTileSizeWidget;
    class CIMGUIButton* mCreateMapButton;
    
    class CIMGUIComboBox* mTileSetTileList;
    class CIMGUIImage* mTileImageWidget;
    class CIMGUITextInput* mTileShapeWidget;
    class CIMGUITextInput* mTileTypeWidget;
    class CIMGUIButton* mDefaultFrameButon;

    class CIMGUIButton* mOpenTileSetEditButton;
};

