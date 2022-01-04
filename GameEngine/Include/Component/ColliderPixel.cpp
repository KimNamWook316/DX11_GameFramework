#include "ColliderPixel.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../PathManager.h"
#include "../Device.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "CameraComponent.h"
#include "../Collision/Collision.h"
#include "ColliderBox2D.h"
#include "ColliderCircle.h"

CColliderPixel::CColliderPixel()
{
	SetTypeID<CColliderPixel>();
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = true;
	meColliderType = eColliderType::Pixel;
}

CColliderPixel::CColliderPixel(const CColliderPixel& com)	:
	CColliderComponent(com)
{
	// Shallow Copy
	mInfo = com.mInfo;
	++mInfo.RefCount;
	
	mPixelMesh = com.mPixelMesh;
	mPixelShader= com.mPixelShader;
}

CColliderPixel::~CColliderPixel()
{
	--mInfo.RefCount;

	if (mInfo.RefCount == 0)
	{
		SAFE_RELEASE(mInfo.SRV);
		SAFE_DELETE_ARRAY(mInfo.Pixel);
	}
}

bool CColliderPixel::Init()
{
	if (!CColliderComponent::Init())
	{
		assert(false);
		return false;
	}
	
	// 회전 시키지 않을 것이기 때문에 축 고정한다.
	mInfo.Box.Axis[0] = Vector2(1.f, 0.f);
	mInfo.Box.Axis[1] = Vector2(0.f, 1.f);

	mMesh = mScene->GetResource()->FindMesh("Box2DMesh");
	mPixelMesh = mScene->GetResource()->FindMesh("SpriteMesh");
	mPixelShader = mScene->GetResource()->FindShader("ColliderPixelShader");

	SetWorldScale((float)mInfo.Width, (float)mInfo.Height, 1.f);
	return true;
}

void CColliderPixel::Start()
{
	CColliderComponent::Start();
}

void CColliderPixel::Update(float deltaTime)
{
	CColliderComponent::Update(deltaTime);
}

void CColliderPixel::PostUpdate(float deltaTime)
{
	CColliderComponent::PostUpdate(deltaTime);

	mInfo.Box.Center.x = GetWorldPos().x + mOffset.x;
	mInfo.Box.Center.y = GetWorldPos().y + mOffset.y;

	mMinPos.x = mInfo.Box.Center.x - mInfo.Box.Length.x;
	mMinPos.y = mInfo.Box.Center.y - mInfo.Box.Length.y;
	mMaxPos.x = mInfo.Box.Center.x + mInfo.Box.Length.x;
	mMaxPos.y = mInfo.Box.Center.y + mInfo.Box.Length.y;

	mInfo.Min.x = mMinPos.x;
	mInfo.Min.y = mMinPos.y;
	mInfo.Max.x = mMaxPos.x;
	mInfo.Max.y = mMaxPos.y;
}

void CColliderPixel::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderPixel::Render()
{
	CColliderComponent::Render();

	CCameraComponent* cam = mScene->GetCameraManager()->GetCurrentCamera();

	Matrix matWorld, matView, matProj, matWVP;
	matView = cam->GetViewMatrix();
	matProj = cam->GetProjMatrix();

	Matrix matScale, matTrans;
	matScale.Scaling((float)mInfo.Width, (float)mInfo.Height);
	matTrans.Translation(mInfo.Box.Center);

	matWorld = matScale * matTrans;
	matWVP = matWorld * matView * matProj;
	matWVP.Transpose();

	mCBuffer->SetColliderWVP(matWVP);
	
	if (mPrevCollisionList.empty())
	{
		mCBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));
	}
	else
	{
		mCBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));
	}
	
	if (mbMouseCollision)
	{
		mCBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));
	}
	
	mCBuffer->UpdateCBuffer();

	// Pixel 넘긴다.
	CDevice::GetInst()->GetContext()->PSSetShaderResources(0, 1, &mInfo.SRV);

	// 텍스쳐 먼저 렌더링
	mPixelShader->SetShader();
	mPixelMesh->Render();

	mShader->SetShader();
	mMesh->Render();
}

void CColliderPixel::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderPixel* CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

void CColliderPixel::Save(FILE* fp)
{
	CColliderComponent::Save(fp);
	fwrite(&mInfo, sizeof(CircleInfo), 1, fp);
}

void CColliderPixel::Load(FILE* fp)
{
	CColliderComponent::Load(fp);
	fread(&mInfo, sizeof(CircleInfo), 1, fp);
}

bool CColliderPixel::DoCollide(CColliderComponent* Dest)
{
	switch (Dest->GetColliderType())
	{
	case eColliderType::Box2D:
		return CCollision::CollisionBox2DToPixel((CColliderBox2D*)Dest, this);
	case eColliderType::Circle2D:
		return CCollision::CollisionCircleToPixel((CColliderCircle*)Dest, this);
	default:
		return false;
	}
}

bool CColliderPixel::DoCollideMouse(const Vector2& mousePos)
{
	CollisionResult result;
	mbMouseCollision = CCollision::CollisionPixelToPoint(mMouseResult, result, mInfo, mousePos);
	return mbMouseCollision;
}

void CColliderPixel::SetInfo(const TCHAR* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	TCHAR fullPath[MAX_PATH] = {};

	if (path)
	{
		lstrcpy(fullPath, path->Path);
	}
	lstrcat(fullPath, fileName);

	SetInfoFullPath(fullPath);
}

// 최종적으로 여기에서 mInfo 세팅
void CColliderPixel::SetInfoFullPath(const TCHAR* fullPath)
{
	TCHAR fileExt[_MAX_EXT] = {};

	_wsplitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, fileExt, _MAX_EXT);

	char ext[_MAX_EXT] = {};

#ifdef UNICODE
	int convertLength = WideCharToMultiByte(CP_ACP, 0, fileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, fileExt, -1, ext, convertLength, nullptr, nullptr);
#else
	strcpy_s(ext, fileExt);
#endif
	_strupr_s(ext);

	ScratchImage image = {};

	if (strcmp(ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, image)))
		{
			assert(false);
			return;
		}
	}

	else if (strcmp(ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(fullPath, nullptr, image)))
		{
			assert(false);
			return;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, image)))
		{
			assert(false);
			return;
		}
	}

	mInfo.Width = (unsigned int)image.GetImages()[0].width;
	mInfo.Height = (unsigned int)image.GetImages()[0].height;

	// Pixel 개수만큼 배열 할당
	mInfo.Pixel = new unsigned char[image.GetPixelsSize()];

	// 픽셀 데이터 복사
	memcpy(mInfo.Pixel, image.GetPixels(), image.GetPixelsSize());

	// bmp파일의 경우, 위아래가 반전된 형태로 픽셀 데이터가 구성되어 있다.
	// 다른 포맷과 똑같은 구조를 가지도록 반전시켜준다.
	if (strcmp(ext, ".BMP") == 0)
	{
		// rgba 이므로 한 줄 픽셀 수 * 4
		unsigned int lineSize = mInfo.Width * 4;
		unsigned char* line = new unsigned char[lineSize];
		
		int halfHeight = mInfo.Height / 2;

		for (int i = 0; i < halfHeight; ++i)
		{
			memcpy(line, &mInfo.Pixel[i * lineSize], lineSize);
			memcpy(&mInfo.Pixel[i * lineSize], &mInfo.Pixel[(mInfo.Height - 1 - i) * lineSize], lineSize);
			memcpy(&mInfo.Pixel[(mInfo.Height - 1 - i) * lineSize], line ,lineSize);
		}

		SAFE_DELETE_ARRAY(line);
	}

	SetWorldScale((float)mInfo.Width, (float)mInfo.Height, 1.f);

	mInfo.Box.Length.x = (float)mInfo.Width / 2.f;
	mInfo.Box.Length.y = (float)mInfo.Height / 2.f;

	// Resource View 생성
	if (FAILED(CreateShaderResourceView(CDevice::GetInst()->GetDevice(), image.GetImages(),
		image.GetImageCount(), image.GetMetadata(), &mInfo.SRV)))
	{
		assert(false);
		return;
	}
}

void CColliderPixel::SetInfoMultiByte(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char fullPath[MAX_PATH] = {};

	if (path)
	{
		strcpy_s(fullPath, path->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	SetInfoMultibyteFullPath(fullPath);
}

void CColliderPixel::SetInfoMultibyteFullPath(const char* fullPath)
{
	TCHAR convertedPath[MAX_PATH] = {};

	int length = MultiByteToWideChar(CP_ACP, 0, fullPath, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, fullPath, -1, convertedPath, length);
	
	return SetInfoFullPath(convertedPath);
}

void CColliderPixel::SetPixelCollisionType(ePixelCollisionType eType)
{
	mInfo.Type = eType;
}

void CColliderPixel::SetPixelColor(unsigned char r, unsigned char g, unsigned char b)
{
	mInfo.Color[0] = r;
	mInfo.Color[1] = g;
	mInfo.Color[2] = b;
}

void CColliderPixel::SetPixelAlpha(unsigned char a)
{
	mInfo.Color[3] = a;
}
