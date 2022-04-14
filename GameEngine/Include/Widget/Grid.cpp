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
	SAFE_DELETE(mCellInfoBuffer);
	size_t size = mVecCell.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecCell[i]);
	}
	mVecCell.clear();
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

	// ���� �浹���� ���� ���� ���� �������� ������ Normal���·� �����ش�.
	if (!mbMouseHovered)
	{
		// ������ ���õǾ��� ������ Normal ���·�
		size_t sizePrev = mVecPrevMouseOn.size();
		for (size_t i = 0; i < sizePrev; ++i)
		{
			int index = mVecPrevMouseOn[i].Index;
			mVecCell[index]->State = eButtonState::Normal;
		}
		mVecPrevMouseOn.clear();
	}
	// ���콺�� �浹�� ����
	else
	{
		Vector2 mousePos = CInput::GetInst()->GetMousePos();

		mousePos -= mRenderPos;

		int idxX = mousePos.x / mCellSize.x;
		int idxY = (mCellCountY)-(mousePos.y / mCellSize.y);

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

		// ���� ���� ���� üũ
		// �ε����� ���� ���� ���
		bool bOutOfRange = false;
		for (int y = 0; y < mCellSelectHeight; ++y)
		{
			int curY = 0;
			for (int x = 0; x < mCellSelectWidth; ++x)
			{
				int curIndex = (idxY + y) * mCellCountX + (idxX + x);
				int curY = curIndex / mCellCountX;

				if (curIndex < 0 || curIndex >= mCellCount)
				{
					bOutOfRange = true;
					break;
				}

				// ���� y ���� ��� ���
				if (curY != idxY + y)
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

		// ������ ��� ���, ���ο� ���� ó�� ���� �ʴ´�.
		if (bOutOfRange)
		{
			// ������ ���õǾ��� ������ Normal ���·�
			size_t sizePrev = mVecPrevMouseOn.size();
			for (size_t i = 0; i < sizePrev; ++i)
			{
				int index = mVecPrevMouseOn[i].Index;
				mVecCell[index]->State = eButtonState::Normal;
			}
			mVecPrevMouseOn.clear();
			return;
		}

		bool bIncludeOuccpiedCell = false;
		// ���� ���õ� ���� ���� ó��
		for (int y = 0; y < mCellSelectHeight; ++y)
		{
			for (int x = 0; x < mCellSelectWidth; ++x)
			{
				int curIndex = (idxY + y) * mCellCountX + (idxX + x);
				if (mVecCell[curIndex]->bOccupied)
				{
					bIncludeOuccpiedCell = true;
				}
			}
		}

		// Hover ó��
		if (bIncludeOuccpiedCell)
		{
			if (mMouseOnCell->State == eButtonState::Normal ||
				mMouseOnCell->State == eButtonState::Hovered)
			{
				mMouseOnCell->State = eButtonState::Hovered;
			}
		}
		else
		{
			for (int y = 0; y < mCellSelectHeight; ++y)
			{
				for (int x = 0; x < mCellSelectWidth; ++x)
				{
					int curIndex = (idxY + y) * mCellCountX + (idxX + x);

					if (x == 0 && y == 0)
					{
						continue;
					}
					mVecCell[curIndex]->State = eButtonState::Hovered;
				}
			}

			if (mMouseOnCell->State == eButtonState::Normal ||
				mMouseOnCell->State == eButtonState::Hovered)
			{
				mMouseOnCell->State = eButtonState::Hovered;
			}
		}

		if (!CInput::GetInst()->GetMouseLButtonClicked())
		{
			mbIsMouseLPush = false;
		}

		if (CInput::GetInst()->GetMouseRButtonClicked())
		{
			mbIsMouseRPush = false;
		}
		
		if (bIncludeOuccpiedCell)
		{
			if (!mbIsMouseLPush && CInput::GetInst()->GetMouseLButtonClicked())
			{
				mbIsMouseLPush = true;
				mMouseOnCell->State = eButtonState::LClicked;
			}
			else if (!mbIsMouseRPush && CInput::GetInst()->GetMouseRButtonClicked())
			{
				mbIsMouseRPush = true;
				mMouseOnCell->State = eButtonState::RClicked;
			}
			else if (mMouseOnCell->State == eButtonState::LClicked)
			{
				if (mCallBack[(int)eButtonState::LClicked])
				{
					mCallBack[(int)eButtonState::LClicked](idx);
				}
				mMouseOnCell->State = eButtonState::Hovered;
			}
			else if (mMouseOnCell->State == eButtonState::RClicked)
			{
				if (mCallBack[(int)eButtonState::RClicked])
				{
					mCallBack[(int)eButtonState::RClicked](idx);
				}
				mMouseOnCell->State = eButtonState::Hovered;
			}
		}
		else
		{
			if (mMouseOnCell->State == eButtonState::Hovered)
			{
				if (!mbIsMouseLPush && CInput::GetInst()->GetMouseLButtonClicked())
				{
					mbIsMouseLPush = true;
					mMouseOnCell->State = eButtonState::LClicked;
				}
				else if (!mbIsMouseRPush && CInput::GetInst()->GetMouseRButtonClicked())
				{
					mbIsMouseRPush = true;
					mMouseOnCell->State = eButtonState::RClicked;
				}
				else
				{
					if (mCallBack[(int)eButtonState::Hovered])
					{
						mCallBack[(int)eButtonState::Hovered](idx);
					}
				}
			}
			else if (mMouseOnCell->State == eButtonState::LClicked)
			{
				if (mCallBack[(int)eButtonState::LClicked])
				{
					mCallBack[(int)eButtonState::LClicked](idx);
				}
				mMouseOnCell->State = eButtonState::Hovered;
			}
			else if (mMouseOnCell->State == eButtonState::RClicked)
			{
				if (mCallBack[(int)eButtonState::RClicked])
				{
					mCallBack[(int)eButtonState::RClicked](idx);
				}
				mMouseOnCell->State = eButtonState::Hovered;
			}
		}

		// ���� ���õ� ���� ó��
		// ���� �������� ��� ������ normal�� ó��
		{
			int minX = idxX;
			int minY = idxY;
			int maxX = idxX + mCellSelectWidth - 1;
			int maxY = idxY + mCellSelectHeight - 1;

			size_t sizePrev = mVecPrevMouseOn.size();
			for (size_t i = 0; i < sizePrev; ++i)
			{
				int index = mVecPrevMouseOn[i].Index;
				int curIdxX = index % mCellCountX;
				int curIdxY = index / mCellCountX;

				if (curIdxX < minX || curIdxX > maxX || curIdxY < minY || curIdxY > maxY)
				{
					mVecCell[index]->State = eButtonState::Normal;
					continue;
				}

				if (bIncludeOuccpiedCell && mVecCell[index]->State == eButtonState::Hovered)
				{
					mVecCell[index]->State = eButtonState::Normal;
				}
			}
			mVecPrevMouseOn.clear();
		}

		for (int y = 0; y < mCellSelectHeight; ++y)
		{
			for (int x = 0; x < mCellSelectWidth; ++x)
			{
				int curIndex = (idxY + y) * mCellCountX + (idxX + x);

				if (curIndex < 0 || curIndex >= mCellCount)
				{
					continue;
				}
				PrevCellState state;
				state.Index = curIndex;
				state.State = mVecCell[curIndex]->State;
				mVecPrevMouseOn.push_back(state);
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

	// �׻� ���� ��ġ�� ����ϱ� ������, ����� ��ȯ �ʿ� ����
	size_t size = mVecCellRenderInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		matTrans.Translation(mRenderPos.x + mVecCell[i]->Position.x, mRenderPos.y + mVecCell[i]->Position.y, 0.f);
		mVecCellRenderInfo[i].MatWP = matScale * matTrans * cam->GetProjMatrix();

		if (mVecCell[i]->bOccupied)
		{
			mVecCellRenderInfo[i].Tint = mOccupiedTint;
		}
		else
		{
			mVecCellRenderInfo[i].Tint = mTint[(int)mVecCell[i]->State];
		}

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

	if (mVecCell[idx]->bOccupied)
	{
		return false;
	}

	int idxX = idx % mCellCountX;
	int idxY = idx / mCellCountX;

	// ������ �̹� ������ ���� �ְų�, �ε����� ��� ���, �������� �ʴ´�.
	for (int y = 0; y < mCellSelectHeight; ++y)
	{
		for (int x = 0; x < mCellSelectWidth; ++x)
		{
			int curIdx = (idxY + y) * mCellCountX + (idxX + x);

			if (curIdx < 0 || idx >= mCellCount ||
				mVecCell[curIdx]->bOccupied)
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
			mVecCell[curIdx]->bOccupied = true;;
		}
	}

	return true;
}

bool CGrid::OccupyCell(const std::vector<int>& indexes)
{
	size_t size = indexes.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (indexes[i] < 0 || indexes[i] >= mCellCount)
		{
			return false;
		}
	}

	for (size_t i = 0; i < size; ++i)
	{
		mVecCell[indexes[i]]->bOccupied = true;;
	}

	return true;
}

bool CGrid::UnOccupyCell(const int idx)
{
	if (idx < 0 || idx >= mCellCount)
	{
		return false;
	}

	mVecCell[idx]->bOccupied = false;

	return true;
}

bool CGrid::UnOccupyCell(const std::vector<int>& indexes)
{
	size_t size = indexes.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (indexes[i] < 0 || indexes[i] >= mCellCount)
		{
			return false;
		}
	}

	for (size_t i = 0; i < size; ++i)
	{
		mVecCell[indexes[i]]->bOccupied = false;
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

void CGrid::SetTint(eButtonState state, const Vector4& color)
{
	mTint[(int)state] = color;
}

void CGrid::SetOccupiedTint(const Vector4& color)
{
	mOccupiedTint = color;
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

Vector2 CGrid::GetCellPos(const int idx)
{
	return Vector2(mVecCell[idx]->Position.x, mVecCell[idx]->Position.y);
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
			cell->State = eButtonState::Normal;
			cell->Position = Vector3(x * mCellSize.x,
				mSize.y - ((y + 1) * mCellSize.y), 0.f);

			mVecCell.push_back(cell);
		}
	}

	makeCellInfo();
}

void CGrid::makeCellInfo()
{
	if (mCellCount <= 0)
	{
		return;
	}

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
