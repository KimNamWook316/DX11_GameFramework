#pragma once

#include "IMGUIWindow.h"

class CObjectHierachyWindow :
    public CIMGUIWindow
{
public:
    CObjectHierachyWindow();
    virtual ~CObjectHierachyWindow();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;

public:
    void AddObjectList(const char* name);

public:
    void OnSelectObject(int idx, const char* item);
    void OnSelectComponent(int idx, const char* item);

public:
    class CIMGUIListBox* GetObjectListBox() const
    {
        return mObjectListBox;
    }

    class CIMGUIListBox* GetComponentListBox() const
    {
        return mComponentListBox;
    }

private:
    class CIMGUIListBox* mObjectListBox;
    class CIMGUIListBox* mComponentListBox;
};

