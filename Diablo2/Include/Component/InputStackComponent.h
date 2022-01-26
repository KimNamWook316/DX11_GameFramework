#pragma once

#include "Component/ObjectComponent.h"
#include "../Dia2Info.h"

class CInputStackComponent :
    public CObjectComponent
{
    friend class CD2Player;

protected:
    CInputStackComponent();
    CInputStackComponent(const CInputStackComponent& com);
    virtual ~CInputStackComponent();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CInputStackComponent* Clone() override;

public:
    void PushMouseInput(eMouseClickState state, const Vector2& inputPos)
    {
        MouseInputInfo info;
        info.State = state;
        info.Position = inputPos;
        mMouseInputStack.push(info);
    }

    void PushKeyInput(const char& key)
    {
        mKeyInputStack.push(key);
    }

public:
    MouseInputInfo GetMouseInputTop() const
    {
        return mMouseInputStack.top();
    }

    char GetKeyInputTop() const
    {
        return mKeyInputStack.top();
    }

public:
    void PopMouseInput()
    {
        if (!mMouseInputStack.empty())
        {
            mMouseInputStack.pop();
        }
    }

    void PopKeyInput()
    {
        if (!mKeyInputStack.empty())
        {
            mKeyInputStack.pop();
        }
    }

private:
    std::stack<MouseInputInfo> mMouseInputStack;
    std::stack<char> mKeyInputStack;
    float mClearTimer;
};

