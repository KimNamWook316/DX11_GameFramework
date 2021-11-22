#include "Transform.h"

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
	mbUpdatePos(true)
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
}

CTransform::~CTransform()
{
}

void CTransform::Init()
{
}

CTransform* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::SetRelativeScale(const Vector3& scale)
{
	mRelativeScale = scale;
	// 이 트랜스폼이 최상위 트랜스폼이라면 여기서 worldScale변경해줘야 한다.
	// TODO : 월드 트랜스폼 Setter가 있는 편이 낫지 않은가?
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
}

void CTransform::SetRelativePos(const float& x, const float& y, const float& z)
{
}

void CTransform::AddRelativeScale(const Vector3& scale)
{
}

void CTransform::AddRelativeScale(const float& x, const float& y, const float& z)
{
}

void CTransform::AddRelativeRot(const Vector3 rot)
{
}

void CTransform::AddRelativeRot(const float& x, const float& y, const float& z)
{
}

void CTransform::AddRelativeRotX(const float& x)
{
}

void CTransform::AddRelativeRotY(const float& y)
{
}

void CTransform::AddRelativeRotZ(const float& z)
{
}

void CTransform::AddRelativePos(const Vector3& pos)
{
}

void CTransform::AddRelativePos(const float& x, const float& y, const float& z)
{
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
		// 
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
		SetInheritRotValue(false);
	}
}

void CTransform::SetInheritPosValue()
{
	if (mParentTransform)
	{
		Vector3 parentRot;

		if (mbInheritPosX)
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

			// 부모 회전만큼 회전시키고, 부모의 위치값만큼 이동한다.
			mWorldPos = mRelativePos.TransformCoord(matRot);
		}
	}

	mbUpdatePos = true;

	size_t size = mVecChildTransform.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritPosValue();
	}
}
