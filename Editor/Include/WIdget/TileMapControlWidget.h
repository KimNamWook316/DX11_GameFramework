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
    void OnChangeFrameStart(int val[2]);
    void OnChangeFrameEnd(int val[2]);
    void OnClickDefaultFrame();
    void OnClickOpenTileSetEdit();

private:
    class CIMGUIImage* mTileImageWidget;
    class CIMGUIButton* mLoadImageButton;

    class CIMGUIComboBox* mTileShapeWidget;
    class CIMGUIInputInt2* mTileCountWidget;
    class CIMGUIInputInt2* mTileSizeWidget;
    class CIMGUIButton* mCreateMapButton;
    
    class CIMGUIComboBox* mTileTypeWidget;
    class CIMGUIInputInt2* mFrameStartWidget;
    class CIMGUIInputInt2* mFrameEndWidget;
    class CIMGUIButton* mDefaultFrameButon;

    class CIMGUIButton* mOpenTileSetEditButton;
};

