#pragma once

#include "IMGUIWidget.h"

class CIMGUIInputFloat :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;
    friend class CIMGUIChild;

protected:
    CIMGUIInputFloat();
    virtual ~CIMGUIInputFloat();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    void SetVal(const float val)
    {
        mVal = val;
    }

    float GetVal() const
    {
        return mVal;
    }

    void SetStep(const float step)
    {
        mStep = step;
    }

    float GetStep() const
    {
        return mStep;
    }

protected:
    float mVal;
    float mStep;
};

