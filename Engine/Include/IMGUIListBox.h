#pragma once

#include "IMGUIWidget.h"

class CIMGUIListBox :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;

protected:
    CIMGUIListBox();
    virtual ~CIMGUIListBox();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    const std::string& GetItem(const int idx)
    {
        return mVecItem[idx];
    }

    const std::string& GetSelectItem() const
    {
        return mVecItem[mSelectIndex];
    }

    int GetSelectIndex() const
    {
        return mSelectIndex;
    }

    int GetItemCount() const
    {
        return (int)mVecItem.size();
    }

public:
    void AddItem(const std::string& item)
    {
        mVecItem.push_back(item);

        wchar_t wItem[1024] = {};
        char itemUTF8[1024] = {};

        int length = MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, 0, 0);
        MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wItem, length);

        // Convert To UTF8
        length = WideCharToMultiByte(CP_UTF8, 0, wItem, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, wItem, -1, itemUTF8, length, 0, 0);

        mVecItemUTF8.push_back(itemUTF8);

        // ABC Sort
        if (mbSort)
        {
            std::sort(mVecItem.begin(), mVecItem.end());
            std::sort(mVecItemUTF8.begin(), mVecItemUTF8.end());
        }
    }

    void SetPageItemCount(const int count)
    {
        mPageItemCount = count;
    }

    void DeleteItem(const int idx)
    {
        auto iter = mVecItem.begin() + idx;
        mVecItem.erase(iter);

        auto iterUTF = mVecItemUTF8.begin() + idx;
        mVecItemUTF8.erase(iter);
    }

    void Clear()
    {
        mVecItem.clear();
        mVecItemUTF8.clear();
    }

    void Sort(const bool bSort)
    {
        mbSort = bSort;
        
        if (mbSort)
        {
            std::sort(mVecItem.begin(), mVecItem.end());
            std::sort(mVecItemUTF8.begin(), mVecItemUTF8.end());
        }
    }

public:
    template <typename T>
    void SetSelectCallBack(T* obj, void(T::* func)(int, const char*))
    {
        mSelectCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
    }

protected:
    std::vector<std::string> mVecItem;
    std::vector<std::string> mVecItemUTF8;
    bool mbSelect;
    int mSelectIndex;
    int mPageItemCount;
    bool mbSort;
    std::function<void(int, const char*)> mSelectCallBack;
};

