#pragma once

#include "Widget.h"
#include "WidgetImageData.h"

struct Cell
{
    eCellState State;
    Vector3 Position;
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
    bool UnOccupycell(const int idx);
    void SetCellCount(const int x, const int y);
    void SetCellSize(const Vector2& size);
    void SetCellSize(const int x, const int y);
    void SetTint(eCellState state, const Vector4& color);
    void SetSelectWidth(const int width);
    void SetSelectHeight(const int height);
    void SetSelectRange(const int width, const int height);

public:
    template <typename T>
    void SetCallBack(eCellState state, T* obj, void(T::* func)(int))
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
    Vector4 mTint[(int)eCellState::Max];
    Cell* mMouseOnCell;
    std::vector<Cell*> mVecPrevMouseOn;
    std::vector<Cell*> mVecCurMouseOn;
    int mCellCountX;
    int mCellCountY;
    int mCellCount;
    Vector2 mCellSize;
    int mCellSelectWidth;
    int mCellSelectHeight;
    std::function<void(int)> mCallBack[(int)eCellState::Max];
};

