#pragma once

#include "Component/ObjectComponent.h"
#include "Component/SceneComponent.h"
#include "../Dia2Info.h"

class CInputStackComponent :
    public CSceneComponent
{
    friend class CGameObject;

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
        if (!mMouseInputStack.empty())
        {
            return mMouseInputStack.top();
        }
    }

    char GetKeyInputTop() const
    {
        if (!mMouseInputStack.empty())
        {
            return mKeyInputStack.top();
        }
    }

    bool IsMouseInputEmpty()
    {
        return mMouseInputStack.empty();
    }

    bool IsKeyInputEmpty()
    {
        return mKeyInputStack.empty();
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

    void ClearMouseInput()
    {
		while (!mMouseInputStack.empty())
		{
			mMouseInputStack.pop();
		}
        mClearTimer = 0.f;
    }

    void ClearKeyInput()
    {
		while (!mKeyInputStack.empty())
		{
			mKeyInputStack.pop();
		}
        mClearTimer = 0.f;
    }

private:
    std::stack<MouseInputInfo> mMouseInputStack;
    std::stack<char> mKeyInputStack;
    float mClearTimer;
};

