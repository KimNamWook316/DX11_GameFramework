#pragma once

#include "Widget.h"
#include "WidgetImageData.h"

struct Cell
{
    bool bOccupied;
    eButtonState State;
    Vector3 Position;

    Cell() :
        bOccupied(false),
        State(eButtonState::Normal)
    {
    }
};

struct PrevCellState
{
    int Index;
    eButtonState State;
};

class CGrid :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CGrid();
    CGrid(const CGrid& widget);
    virtual ~CGrid();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CGrid* Clone() override;

public:
    bool OccupyCell(const int idx);
    bool OccupyCell(const std::vector<int>& indexes);
    bool UnOccupyCell(const int idx);
    bool UnOccupyCell(const std::vector<int>& indexes);
    void SetCellCount(const int x, const int y);
    void SetCellSize(const Vector2& size);
    void SetCellSize(const int x, const int y);
    void SetTint(eButtonState state, const Vector4& color);
    void SetOccupiedTint(const Vector4& color);
    void SetSelectWidth(const int width);
    void SetSelectHeight(const int height);
    void SetSelectRange(const int width, const int height);

public:
    Vector2 GetCellPos(const int idx);
    const Vector2& GetCellSize() const
    {
        return mCellSize;
    }

    int GetCellCountX() const
    {
        return mCellCountX;
    }

    int GetCellCountY() const
    {
        return mCellCountY;
    }
    
    int GetCellCount() const
    {
        return mCellCount;
    }

public:
    template <typename T>
    void SetCallBack(eButtonState state, T* obj, void(T::* func)(int))
    {
        mCallBack[(int)state] = std::bind(func, obj, std::placeholders::_1);
    }

private:
    void makeCell();
    void makeCellInfo();

private:
    class CStructuredBuffer* mCellInfoBuffer;
    std::vector<CellRenderInfo> mVecCellRenderInfo;
    std::vector<Cell*> mVecCell;
    Vector4 mTint[(int)eButtonState::Max];
    Vector4 mOccupiedTint;
    Cell* mMouseOnCell;
    std::vector<PrevCellState> mVecPrevMouseOn;
    std::vector<Cell*> mVecCurMouseOn;
    int mCellCountX;
    int mCellCountY;
    int mCellCount;
    Vector2 mCellSize;
    int mCellSelectWidth;
    int mCellSelectHeight;
    std::function<void(int)> mCallBack[(int)eButtonState::Max];
    bool mbIsMouseLPush;
    bool mbIsMouseRPush;
};

