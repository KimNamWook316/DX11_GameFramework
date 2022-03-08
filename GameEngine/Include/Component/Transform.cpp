#include "Transform.h"
#include "../Resource/Shader/TransformConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"
#include "../Engine.h"

CTransform::CTransform()	:
	mParentTransform(nullptr),
	mScene(nullptr),
	mObject(nullptr),
	mOwnerComponent(nullptr),
	mbInheritScale(false),
	mbInheritRotX(false),
	mbInheritRotY(false),
	mbInheritRotZ(false),
	// 포지션은 상대적 움직임 적용됨에 유의
	mbInheritPosX(true),
	mbInheritPosY(true),
	mbInheritPosZ(true),
	// 시작 떄 한 번은 업데이트 해 주어야 한다.
	mbUpdateScale(true),
	mbUpdateRot(true),
	mbUpdatePos(true),
	mCBuffer(nullptr)
{
	// 기본 회전축 설정
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		mRelativeAxis[i] = Vector3::Axis[i];
		mWorldAxis[i] = Vector3::Axis[i];
	}
}

CTransform::CTransform(const CTransform& trans)
{
	*this = trans;

	mCBuffer = trans.mCBuffer->Clone();
}

CTransform::~CTransform()
{
	SAFE_DELETE(mCBuffer);
}

void CTransform::Start()
{
	// 시작 시 한 번 위치값 갱신해주어야 한다.
	SetInheritScaleValue();
	SetInheritRotValue(true);
	SetInheritPosValue();
}

void CTransform::Init()
{
	// 상수 버퍼 생성
	mCBuffer = new CTransformConstantBuffer;

	mCBuffer->Init();
}

CTransform* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Save(FILE* fp)
{
	fwrite(&mbInheritScale, sizeof(bool), 1, fp);
	fwrite(&mbInheritRotX, sizeof(bool), 1, fp);
	fwrite(&mbInheritRotY, sizeof(bool), 1, fp);
	fwrite(&mbInheritRotZ, sizeof(bool), 1, fp);
	fwrite(&mbInheritPosX, sizeof(bool), 1, fp);
	fwrite(&mbInheritPosY, sizeof(bool), 1, fp);
	fwrite(&mbInheritPosZ, sizeof(bool), 1, fp);

	fwrite(&mRelativeScale, sizeof(Vector3), 1, fp);
	fwrite(&mRelativePos, sizeof(Vector3), 1, fp);
	fwrite(&mRelativeRot, sizeof(Vector3), 1, fp);
	fwrite(&mRelativeAxis, sizeof(Vector3), AXIS_MAX, fp);

	fwrite(&mWorldScale, sizeof(Vector3), 1, fp);
	fwrite(&mWorldPos, sizeof(Vector3), 1, fp);
	fwrite(&mWorldRot, sizeof(Vector3), 1, fp);
	fwrite(&mWorldAxis, sizeof(Vector3), AXIS_MAX, fp);
	
	fwrite(&mPivot, sizeof(Vector3), 1, fp);
	fwrite(&mMeshSize, sizeof(Vector3), 1, fp);
}

void CTransform::Load(FILE* fp)
{
	fread(&mbInheritScale, sizeof(bool), 1, fp);
	fread(&mbInheritRotX, sizeof(bool), 1, fp);
	fread(&mbInheritRotY, sizeof(bool), 1, fp);
	fread(&mbInheritRotZ, sizeof(bool), 1, fp);
	fread(&mbInheritPosX, sizeof(bool), 1, fp);
	fread(&mbInheritPosY, sizeof(bool), 1, fp);
	fread(&mbInheritPosZ, sizeof(bool), 1, fp);

	fread(&mRelativeScale, sizeof(Vector3), 1, fp);
	fread(&mRelativePos, sizeof(Vector3), 1, fp);
	fread(&mRelativeRot, sizeof(Vector3), 1, fp);
	fread(&mRelativeAxis, sizeof(Vector3), AXIS_MAX, fp);

	fread(&mWorldScale, sizeof(Vector3), 1, fp);
	fread(&mWorldPos, sizeof(Vector3), 1, fp);
	fread(&mWorldRot, sizeof(Vector3), 1, fp);
	fread(&mWorldAxis, sizeof(Vector3), AXIS_MAX, fp);
	
	fread(&mPivot, sizeof(Vector3), 1, fp);
	fread(&mMeshSize, sizeof(Vector3), 1, fp);
}

void CTransform::Update(const float deltaTime)
{
}

void CTransform::PostUpdate(const float deltaTime)
{
	Vector3 worldPos = mWorldPos;

	if (eEngineSpace::Space2D == CEngine::GetInst()->GetEngineSpace())
	{
		worldPos.z = worldPos.y / 30000.f * 1000.f;
	}

	if (mbUpdateScale)
	{
		mMatScale.Scaling(mWorldScale);
	}

	if (mbUpdateRot)
	{
		mMatRot.Rotation(mWorldRot);
	}

	if (mbUpdatePos)
	{
		mMatPos.Translation(worldPos);
	}

	if (mbUpdateScale || mbUpdateRot || mbUpdatePos)
	{
		mMatWorld = mMatScale * mMatRot * mMatPos;
	}
}

// 상수 버퍼 갱신
void CTransform::SetTransformBuffer()
{
	mCBuffer->SetWorldMatrix(mMatWorld);
	
	CCameraComponent* cam = mScene->GetCameraManager()->GetCurrentCamera();

	mCBuffer->SetViewMatrix(cam->GetViewMatrix());
	mCBuffer->SetProjMatrix(cam->GetProjMatrix());

	mCBuffer->SetPivot(mPivot);
	mCBuffer->SetMeshSize(mMeshSize);

	// 월드 행렬 계산하고, 쉐이더로 보냄
	mCBuffer->UpdateCBuffer();
}

// 월드 행렬을 밖에서 갱신해야 할 때 호출
void CTransform::ComputeWolrdMatrix()
{
	mMatWorld = mMatScale * mMatRot * mMatPos;
}

void CTransform::SetRelativeScale(const Vector3& scale)
{
	mRelativeScale = scale;
	mWorldScale = scale;

	SetInheritScaleValue();
}

void CTransform::SetRelativeScale(const float& x, const float& y, const float& z)
{
	SetRelativeScale(Vector3(x, y, z));
}

void CTransform::SetRelativeRot(const Vector3& rot)
{
	mRelativeRot = rot;
	mWorldRot = rot;

	// 첫 회전일 경우 true
	SetInheritRotValue(true);
}

void CTransform::SetRelativeRot(const float& x, const float& y, const float& z)
{
	SetRelativeRot(Vector3(x, y, z));
}

void CTransform::SetRelativeRotX(const float& x)
{
	Vector3 rot = Vector3(x, mRelativeRot.y, mRelativeRot.z);
	SetRelativeRot(rot);
}

void CTransform::SetRelativeRotY(const float& y)
{
	Vector3 rot = Vector3(mRelativeRot.x, y, mRelativeRot.z);
	SetRelativeRot(rot);
}

void CTransform::SetRelativeRotZ(const float& z)
{
	Vector3 rot = Vector3(mRelativeRot.x, mRelativeRot.y, z);
	SetRelativeRot(rot);
}

void CTransform::SetRelativePos(const Vector3& pos)
{
	mRelativePos = pos;
	mWorldPos = pos;

	SetInheritPosValue();
}

void CTransform::SetRelativePos(const float& x, const float& y, const float& z)
{
	Vector3 pos(x, y, z);
	SetRelativePos(pos);
}

void CTransform::SetWorldScale(const Vector3& scale)
{
	mWorldScale = scale;
	mRelativeScale = scale;
	SetInheritWorldScaleValue();
}

void CTransform::SetWorldScale(const float& x, const float& y, const float& z)
{
	Vector3 scale(x, y, z);
	SetWorldScale(scale);
}

void CTransform::SetWorldRot(const Vector3& rot)
{
	mWorldRot = rot;
	mRelativeRot = rot;
	SetInheritWorldRotValue(true);
}

void CTransform::SetWorldRot(const float& x, const float& y, const float& z)
{
	Vector3 rot(x, y, z);
	SetWorldRot(rot);
}

void CTransform::SetWorldRotX(const float& x)
{
	Vector3 rot(x, mWorldRot.y, mWorldRot.z);
	SetWorldRot(rot);
}

void CTransform::SetWorldRotY(const float& y)
{
	Vector3 rot(mWorldRot.x, y, mWorldRot.z);
	SetWorldRot(rot);
}

void CTransform::SetWorldRotZ(const float& z)
{
	Vector3 rot(mWorldRot.x, mWorldRot.y, z);
	SetWorldRot(rot);
}

void CTransform::SetWorldPos(const Vector3& pos)
{
	mWorldPos = pos;
	mRelativePos = pos;
	SetInheritPosValue();
}

void CTransform::SetWorldPos(const float& x, const float& y, const float& z)
{
	Vector3 pos(x, y, z);
	SetWorldPos(pos);
}

void CTransform::AddRelativeScale(const Vector3& scale)
{
	mRelativeScale += scale;
	mWorldScale = mRelativeScale;
	SetInheritScaleValue();
}

void CTransform::AddRelativeScale(const float& x, const float& y, const float& z)
{
	Vector3 scale(x, y, z);
	AddRelativeScale(scale);
}

void CTransform::AddRelativeRot(const Vector3 rot)
{
	mRelativeRot += rot;
	mWorldRot = mRelativeRot;
	SetInheritRotValue(true);
}

void CTransform::AddRelativeRot(const float& x, const float& y, const float& z)
{
	Vector3 rot(x, y, z);
	AddRelativeRot(rot);
}

void CTransform::AddRelativeRotX(const float& x)
{
	Vector3 rot(x, 0, 0);
	AddRelativeRot(rot);
}

void CTransform::AddRelativeRotY(const float& y)
{
	Vector3 rot(0, y, 0);
	AddRelativeRot(rot);
}

void CTransform::AddRelativeRotZ(const float& z)
{
	Vector3 rot(0, 0, z);
	AddRelativeRot(rot);
}

void CTransform::AddRelativePos(const Vector3& pos)
{
	mRelativePos += pos;
	mWorldPos = mRelativePos;
	SetInheritPosValue();
}

void CTransform::AddRelativePos(const float& x, const float& y, const float& z)
{
	Vector3 pos(x, y, z);
	AddRelativePos(pos);
}

void CTransform::AddWorldScale(const Vector3& scale)
{
	mWorldScale += scale;
	mRelativeScale = mWorldScale;
	SetInheritWorldScaleValue();
}

void CTransform::AddWorldScale(const float& x, const float& y, const float& z)
{
	Vector3 scale(x, y, z);
	AddWorldScale(scale);
}

void CTransform::AddWorldRot(const Vector3 rot)
{
	mWorldRot += rot;
	mRelativeRot = mWorldRot;
	SetInheritWorldRotValue(true);
}

void CTransform::AddWorldRot(const float& x, const float& y, const float& z)
{
	Vector3 rot(x, y, z);
	AddWorldRot(rot);
}

void CTransform::AddWorldRotX(const float& x)
{
	Vector3 rot(x, 0, 0);
	AddWorldRot(rot);
}

void CTransform::AddWorldRotY(const float& y)
{
	Vector3 rot(0, y, 0);
	AddWorldRot(rot);
}

void CTransform::AddWorldRotZ(const float& z)
{
	Vector3 rot(0, 0, z);
	AddWorldRot(rot);
}

void CTransform::AddWorldPos(const Vector3& pos)
{
	mWorldPos += pos;
	mRelativePos = mWorldPos;
	SetInheritPosValue();
}

void CTransform::AddWorldPos(const float& x, const float& y, const float& z)
{
	Vector3 pos(x, y, z);
	AddWorldPos(pos);
}

void CTransform::SetInheritScaleValue()
{
	// 상위 트랜스폼이 있고, 상대적 스케일 적용받을 경우
	if (mParentTransform && mbInheritScale)
	{
		// 상대적 스케일 * 상위 트랜스폼의 월드 스케일 == 이 트랜스폼의 월드 스케일
		// Vector3 원소별 곱셈 연산임에 유의
		mWorldScale = mRelativeScale * mParentTransform->GetWorldScale();
	}

	mbUpdateScale = true;

	size_t size = mVecChildTransform.size();

	// 하위 오브젝트들도 스케일 조정
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritScaleValue();
	}
}

void CTransform::SetInheritRotValue(bool bIsCurrent)
{
	if (mParentTransform)
	{
		// 월드 회전값 변경
		if (mbInheritRotX)
		{
			mWorldRot.x = mRelativeRot.x + mParentTransform->GetWorldRot().x;
		}

		if (mbInheritRotY)
		{
			mWorldRot.y = mRelativeRot.y + mParentTransform->GetWorldRot().y;
		}

		if (mbInheritRotZ)
		{
			mWorldRot.z = mRelativeRot.z + mParentTransform->GetWorldRot().z;
		}

		// 상위 트랜스폼의 Rotation값을 하나라도 적용받는 경우
		if ((mbInheritRotX || mbInheritRotY || mbInheritRotZ) && (!bIsCurrent))
		{
			// 상위 트랜스폼의 회전을 적용받는 경우, 위치도 바뀌어야 함 ( 공전 )
			SetInheritPosValue();
		}
	}

	// 상대 회전값 라디안으로
	Vector3 convertRot = mRelativeRot.ConvertAngle();

	// XMVECTOR4 쿼터니언 값 받아옴
	XMVECTOR quarternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

	// 상대 회전값으로 변환하는 행렬
	Matrix matRot;
	matRot.RotationQuaternion(quarternion);

	// 상위 트랜스폼에 대한 상대 축 변환
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		mRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		mRelativeAxis[i].Normalize();
	}

	// 월드 회전값에 따른 변환행렬
	convertRot = mWorldRot.ConvertAngle();
	quarternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	matRot.RotationQuaternion(quarternion);

	// 월드 축 변환
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		mWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		mWorldAxis[i].Normalize();
	}

	mbUpdateRot = true;

	size_t size = mVecChildTransform.size();

	// 하위 오브젝트들에도 적용
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritRotValue(false);
	}
}

// 회전 상속 여부 상관없이 relative position이 수정되어야 할 경우에는??
// -> 상대 위치 값은 부모의 월드 위치가 바뀌더라도 바뀌지 않는다.
void CTransform::SetInheritPosValue()
{
	if (mParentTransform)
	{
		Vector3 parentRot;

		if (mbInheritRotX)
		{
			parentRot.x = mParentTransform->GetWorldRot().x;
		}

		if (mbInheritRotY)
		{
			parentRot.y = mParentTransform->GetWorldRot().y;
		}

		if (mbInheritRotZ)
		{
			parentRot.z = mParentTransform->GetWorldRot().z;
		}

		if (mbInheritRotX || mbInheritRotY || mbInheritRotZ)
		{
			// 부모가 얼만큼 회전해 있는지 회전행렬 생성한다.
			Vector3 convertRot = parentRot.ConvertAngle();

			XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

			Matrix matRot;
			matRot.RotationQuaternion(quaternion);

			// 부모 오브젝트의 Position값 얻어옴
			Vector3 parentPos = mParentTransform->GetWorldPos();

			/*
			생성된 회전행렬에 부모 오브젝트의 위치값 복사 한다.
			11, 12, 13, 14
			21, 22, 23, 24
			31, 32, 33,	34
			x,   y,   z, 1

			_41, _42, _43에 복사한다.
			-> 부모 회전, 위치값 가진 변환행렬 완성
			*/
			memcpy(&matRot._41, &parentPos, sizeof(Vector3));
			
			mWorldPos = mRelativePos.TransformCoord(matRot);
		}
		else
		{
			mWorldPos = mRelativePos + mParentTransform->GetWorldPos();
			//mRelativePos = mWorldPos - mParentTransform->GetWorldPos();
		}
	}

	mbUpdatePos = true;

	size_t size = mVecChildTransform.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritPosValue();
	}
}

void CTransform::SetInheritWorldScaleValue()
{
	// 상위 트랜스폼이 있고, 상대적 스케일 적용받을 경우
	if (mParentTransform && mbInheritScale)
	{
		// 상대 스케일 = 부모 월드스케일 대비 자신의 월드스케일 값
		mRelativeScale = mWorldScale / mParentTransform->GetWorldScale();
	}

	mbUpdateScale = true;

	size_t size = mVecChildTransform.size();

	// 하위 오브젝트들도 스케일 조정
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritWorldScaleValue();
	}
}

void CTransform::SetInheritWorldRotValue(bool bIsCurrent)
{
	if (mParentTransform)
	{
		// 월드 회전값 변경
		if (mbInheritRotX)
		{
			mRelativeRot.x = mWorldRot.x - mParentTransform->GetWorldRot().x;
		}

		if (mbInheritRotY)
		{
			mRelativeRot.y = mWorldRot.y - mParentTransform->GetWorldRot().y;
		}

		if (mbInheritRotZ)
		{
			mRelativeRot.z = mWorldRot.z - mParentTransform->GetWorldRot().z;
		}

		// 상위 트랜스폼의 Rotation값을 하나라도 적용받는 경우 and 첫 번째로 호출된 것이 아닐 경우 위치도 조정한다.
		if ((mbInheritRotX || mbInheritRotY || mbInheritRotZ) && !bIsCurrent)
		{
			// 상위 트랜스폼의 회전을 적용받는 경우, 위치도 바뀌어야 함 ( 공전 )
			SetInheritPosValue();
		}
	}

	// 상대 회전값 라디안으로
	Vector3 convertRot = mRelativeRot.ConvertAngle();

	// XMVECTOR4 쿼터니언 값 받아옴
	XMVECTOR quarternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

	// 상대 회전값으로 변환하는 행렬
	Matrix matRot;
	matRot.RotationQuaternion(quarternion);

	// 상위 트랜스폼에 대한 상대 축 변환
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		mRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		mRelativeAxis[i].Normalize();
	}

	// 월드 회전값에 따른 변환행렬
	convertRot = mWorldRot.ConvertAngle();
	quarternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	matRot.RotationQuaternion(quarternion);

	// 월드 축 변환
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		mWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		mWorldAxis[i].Normalize();
	}

	mbUpdateRot = true;

	size_t size = mVecChildTransform.size();

	// 하위 오브젝트들에도 적용
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritWorldRotValue(false);
	}
}

void CTransform::SetInheritWorldPosValue()
{
	if (mParentTransform)
	{
		Vector3 parentRot;

		if (mbInheritRotX)
		{
			parentRot.x = mParentTransform->GetWorldRot().x;
		}

		if (mbInheritRotY)
		{
			parentRot.y = mParentTransform->GetWorldRot().y;
		}

		if (mbInheritRotZ)
		{
			parentRot.z = mParentTransform->GetWorldRot().z;
		}

		if (mbInheritRotX || mbInheritRotY || mbInheritRotZ)
		{
			// 부모가 얼만큼 회전해 있는지 회전행렬 생성한다.
			Vector3 convertRot = parentRot.ConvertAngle();

			XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

			Matrix matRot;
			matRot.RotationQuaternion(quaternion);

			// 부모 오브젝트의 Position값 얻어옴
			Vector3 parentPos = mParentTransform->GetWorldPos();

			/*
			생성된 회전행렬에 부모 오브젝트의 위치값 복사 한다.
			11, 12, 13, 14
			21, 22, 23, 24
			31, 32, 33,	34
			x,   y,   z, 1

			_41, _42, _43에 복사한다.
			-> 부모 회전, 위치값 가진 변환행렬 완성
			*/
			memcpy(&matRot._41, &parentPos, sizeof(Vector3));

			// 월드 위치 = 상대 위치 * 부모 월드 변환 행렬이면,
			// 상대 위치 = 월드 위치 * 부모 월드 변환의 역행렬이다.
			matRot.Inverse();
			mRelativePos = mWorldPos.TransformCoord(matRot);
		}
		else
		{
			mWorldPos = mRelativePos + mParentTransform->GetWorldPos();
		}
	}

	mbUpdatePos = true;

	size_t size = mVecChildTransform.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritPosValue();
	}
}
