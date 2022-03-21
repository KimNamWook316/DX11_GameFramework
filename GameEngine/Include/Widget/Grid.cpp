#include "Grid.h"
#include "WidgetWindow.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Input.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"

CGrid::CGrid()	:
	mCellInfoBuffer(nullptr),
	mCellCountX(0),
	mCellCountY(0),
	mCellCount(0),
	mCellSize(0.f, 0.f),
	mCellSelectWidth(1),
	mCellSelectHeight(1)
{
}

CGrid::CGrid(const CGrid& widget)	:
	CWidget(widget)
{
}

CGrid::~CGrid()
{
}

bool CGrid::Init()
{
	CWidget::Init();

	if (mOwner->GetViewport())
	{
		mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader("GridShader");
	}
	else
	{
		mShader = CResourceManager::GetInst()->FindShader("GridShader");
	}
	return true;
}

void CGrid::Start()
{
	CWidget::Start();
}

void CGrid::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
}

void CGrid::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);

	// 이전에 선택되었던 셀들은 Normal 상태로
	size_t sizePrev = mVecPrevMouseOn.size();
	for (size_t i = 0; i < sizePrev; ++i)
	{
		mVecPrevMouseOn[i]->State = eCellState::Normal;
	}
	mVecPrevMouseOn.clear();

	if (mbMouseHovered)
	{
		Vector2 mousePos = CInput::GetInst()->GetMousePos();

		mousePos -= mRenderPos;

		int idxX = mousePos.x / mCellSize.x;
		int idxY = (mCellCountY) - (mousePos.y / mCellSize.y);

		if (idxX < 0)
		{
			idxX = 0;
		}
		else if (idxX >= mCellCountX)
		{
			idxX = mCellCountX - 1;
		}
		if (idxY < 0)
		{
			idxY = 0;
		}
		else if (idxY >= mCellCountY)
		{
			idxY = mCellCountY - 1;
		}

		int idx = idxY * mCellCountX + idxX;
		mMouseOnCell = mVecCell[idx];

		// 현재 선택 범위 체크
		// 점유 상태 셀들이 포함되거나, 인덱스가 범위 밖인 경우
		bool bOutOfRange = false;
		for (int y = 0; y < mCellSelectHeight; ++y)
		{
			int curY = 0;
			for (int x = 0; x < mCellSelectWidth; ++x)
			{
				int curIndex = (idxY + y) * mCellCountX + (idxX + x);
				int curY = curIndex / mCellCountX;

				// 같은 y 축을 벗어난 경우
				if (curY != idxY + y)
				{
					bOutOfRange = true;
					break;
				}

				if ((curIndex < 0 || curIndex >= mCellCount) ||
					(mVecCell[curIndex]->State == eCellState::Occupied))
				{
					bOutOfRange = true;
					break;
				}
			}

			if (bOutOfRange)
			{
				break;
			}
		}

		// 범위를 벗어난 경우, 새로운 선택 처리 하지 않는다.
		if (bOutOfRange)
		{
			return;
		}

		// 현재 선택된 범위 셀들 처리
		for (int y = 0; y < mCellSelectHeight; ++y)
		{
			for (int x = 0; x < mCellSelectWidth; ++x)
			{
				int curIndex = (idxY + y) * mCellCountX + (idxX + x);

				mVecPrevMouseOn.push_back(mVecCell[curIndex]);

				if (mVecCell[curIndex]->State == eCellState::Normal)
				{
					if (CInput::GetInst()->GetMouseLButtonClicked())
					{
						mVecCell[curIndex]->State = eCellState::Clicked;
					}
					else
					{
						mVecCell[curIndex]->State = eCellState::Hovered;
					}
				}
			}
		}

		// 현재 마우스가 올라가 있는 셀 콜백처리
		if (mMouseOnCell->State == eCellState::Hovered)
		{
			if (mCallBack[(int)eCellState::Hovered])
			{
				mCallBack[(int)eCellState::Hovered](idx);
			}
		}
		else if (mMouseOnCell->State == eCellState::Clicked)
		{
			if (mCallBack[(int)eCellState::Clicked])
			{
				mCallBack[(int)eCellState::Clicked](idx);
			}
		}

	}

	// Render Info
	Matrix matScale, matTrans;
	matScale.Scaling(mCellSize.x, mCellSize.y, 1.f);

	CCameraComponent* cam = nullptr;

	if (mOwner->GetViewport())
	{
		cam = mOwner->GetViewport()->GetScene()->GetCameraManager()->GetUICamera();
	}
	else
	{
		cam = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetUICamera();
	}

	if (!cam)
	{
		assert(false);
	}

	// 항상 고정 위치에 출력하기 때문에, 뷰공간 변환 필요 없음
	size_t size = mVecCellRenderInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		matTrans.Translation(mRenderPos.x + mVecCell[i]->Position.x, mRenderPos.y + mVecCell[i]->Position.y, 0.f);
		mVecCellRenderInfo[i].MatWP = matScale * matTrans * cam->GetProjMatrix();
		mVecCellRenderInfo[i].Tint = mTint[(int)mVecCell[i]->State];

		mVecCellRenderInfo[i].MatWP.Transpose();
	}

	mCellInfoBuffer->UpdateBuffer(&mVecCellRenderInfo[0], mCellCount);
}

void CGrid::Render()
{
	if (!mVecCell.empty())
	{
		mCellInfoBuffer->SetShader();
		mShader->SetShader();
		mMesh->RenderInstancing(mCellCount);
		mCellInfoBuffer->ResetShader();
	}
}

CGrid* CGrid::Clone()
{
	return new CGrid(*this);
}

bool CGrid::OccupyCell(const int idx)
{
	if (idx < 0 || idx >= mCellCount)
	{
		return false;
	}

	if (mVecCell[idx]->State == eCellState::Occupied)
	{
		return false;
	}

	int idxX = idx % mCellCountX;
	int idxY = idx / mCellCountX;

	// 범위에 이미 점유된 셀이 있거나, 인덱스를 벗어난 경우, 점유하지 않는다.
	for (int y = 0; y < mCellSelectHeight; ++y)
	{
		for (int x = 0; x < mCellSelectWidth; ++x)
		{
			int curIdx = (idxY + y) * mCellCountX + (idxX + x);

			if (curIdx < 0 || idx >= mCellCount ||
				mVecCell[curIdx]->State == eCellState::Occupied)
			{
				return false;
			}
		}
	}

	for (int y = 0; y < mCellSelectHeight; ++y)
	{
		for (int x = 0; x < mCellSelectWidth; ++x)
		{
			int curIdx = (idxY + y) * mCellCountX + (idxX + x);
			mVecCell[curIdx]->State = eCellState::Occupied;
		}
	}

	if (mCallBack[(int)eCellState::Occupied])
	{
		mCallBack[(int)eCellState::Occupied](idx);
	}

	return true;
}

bool CGrid::UnOccupycell(const int idx)
{
	if (idx < 0 || idx >= mCellCount)
	{
		return false;
	}

	int idxX = idx % mCellCountX;
	int idxY = idx / mCellCountX;

	// 범위를 벗어난 경우
	for (int y = 0; y < mCellSelectHeight; ++y)
	{
		for (int x = 0; x < mCellSelectWidth; ++x)
		{
			int curIdx = (idxY + y) * mCellCountX + (idxX + x);

			if (curIdx < 0 || idx >= mCellCount)
			{
				return false;
			}
		}
	}

	for (int y = 0; y < mCellSelectHeight; ++y)
	{
		for (int x = 0; x < mCellSelectWidth; ++x)
		{
			int curIdx = (idxY + y) * mCellCountX + (idxX + x);
			mVecCell[curIdx]->State = eCellState::Normal;
		}
	}

	return true;
}

void CGrid::SetCellCount(const int x, const int y)
{
	mCellCountX = x;
	mCellCountY = y;
	mCellCount = mCellCountX * mCellCountY;
	mSize.x = mCellCountX * mCellSize.x;
	mSize.y = mCellCountY * mCellSize.y;
	makeCell();
}

void CGrid::SetCellSize(const Vector2& size)
{
	mCellSize = size;
	mSize.x = mCellCountX * mCellSize.x;
	mSize.y = mCellCountY * mCellSize.y;
	makeCell();
}

void CGrid::SetCellSize(const int x, const int y)
{
	mCellSize = Vector2(x, y);
	mSize.x = mCellCountX * mCellSize.x;
	mSize.y = mCellCountY * mCellSize.y;
	makeCell();
}

void CGrid::SetTint(eCellState state, const Vector4& color)
{
	mTint[(int)state] = color;
}

void CGrid::SetSelectWidth(const int width)
{
	mCellSelectWidth = width;
}

void CGrid::SetSelectHeight(const int height)
{
	mCellSelectHeight = height;
}

void CGrid::SetSelectRange(const int width, const int height)
{
	SetSelectWidth(width);
	SetSelectHeight(height);
}

void CGrid::makeCell()
{
	size_t size = mVecCell.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecCell[i]);
	}

	mVecCell.clear();

	Cell* cell = nullptr;

	for (int y = 0; y < mCellCountY; ++y)
	{
		for (int x = 0; x < mCellCountX; ++x)
		{
			cell = new Cell;
			cell->State = eCellState::Normal;
			cell->Position = Vector3(x * mCellSize.x,
				mSize.y - ((y + 1) * mCellSize.y), 0.f);

			mVecCell.push_back(cell);
		}
	}

	makeCellInfo();
}

void CGrid::makeCellInfo()
{
	SAFE_DELETE(mCellInfoBuffer);

	mCellInfoBuffer = new CStructuredBuffer;

	mCellInfoBuffer->Init("CellInfo", sizeof(CellRenderInfo), 
		mCellCount, 42, true, (int)(eBufferShaderTypeFlags::Vertex) | (int)(eBufferShaderTypeFlags::Pixel));

	mVecCellRenderInfo.resize(mCellCount);

	for (size_t i = 0; i < mCellCount; ++i)
	{
		mVecCellRenderInfo[i].Tint = Vector4(1.f, 1.f, 1.f, 1.f);
	}
}
