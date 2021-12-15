#pragma once

#include "IMGUIWidget.h"
#include "IMGUIInfo.h"

class CIMGUIRadioButton :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;
    friend class CIMGUIChild;

protected:
    CIMGUIRadioButton();
    virtual ~CIMGUIRadioButton();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    void AddCheckInfo(const char* label)
    {
        CheckInfo* info = new CheckInfo;
        
        memset(info->Label, 0, sizeof(char) * 1024);
        memset(info->wLabel, 0, sizeof(wchar_t) * 1024);
        memset(info->LabelUTF8, 0, sizeof(char) * 1024);

        strcpy_s(info->Label, label);
        
        int length = MultiByteToWideChar(CP_ACP, 0, label, -1, nullptr, 0);
        MultiByteToWideChar(CP_ACP, 0, label, -1, info->wLabel, length);

        length = WideCharToMultiByte(CP_UTF8, 0, info->wLabel, -1, nullptr, 0, nullptr, nullptr);
        WideCharToMultiByte(CP_UTF8, 0, info->wLabel, -1, info->LabelUTF8, length, nullptr, nullptr);

        info->bCheck = false;
        mVecCheckInfo.push_back(info);
    }
public:
    void SetColNum(const int val)
    {
        mbMultiColumn = true;
        mColNum = val;
    }

    void SetSpacingX(const float spacing)
    {
        mSpacingX = spacing;
    }

    void SetMultiCheck(const bool bMultiCheck)
    {
        mbMultiCheck = bMultiCheck;
    }

public:
    bool GetCheck(const int idx) const
    {
        return mVecCheckInfo[idx]->bCheck;
    }

protected:
    std::vector<CheckInfo*> mVecCheckInfo; 
    bool mbMultiColumn;
    int mColNum;
    float mSpacingX;
    
    bool mbMultiCheck;
    int mCheckItemIdx;
};

