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
	// �������� ����� ������ ����ʿ� ����
	mbInheritPosX(true),
	mbInheritPosY(true),
	mbInheritPosZ(true),
	// ���� �� �� ���� ������Ʈ �� �־�� �Ѵ�.
	mbUpdateScale(true),
	mbUpdateRot(true),
	mbUpdatePos(true)
{
	// �⺻ ȸ���� ����
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
	// �� Ʈ�������� �ֻ��� Ʈ�������̶�� ���⼭ worldScale��������� �Ѵ�.
	// TODO : ���� Ʈ������ Setter�� �ִ� ���� ���� ������?
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

	// ù ȸ���� ��� true
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
	// ���� Ʈ�������� �ְ�, ����� ������ ������� ���
	if (mParentTransform && mbInheritScale)
	{
		// ����� ������ * ���� Ʈ�������� ���� ������ == �� Ʈ�������� ���� ������
		// Vector3 ���Һ� ���� �����ӿ� ����
		mWorldScale = mRelativeScale * mParentTransform->GetWorldScale();
	}
	
	mbUpdateScale = true;

	size_t size = mVecChildTransform.size();

	// ���� ������Ʈ�鵵 ������ ����
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritScaleValue();
	}
}

void CTransform::SetInheritRotValue(bool bIsCurrent)
{
	if (mParentTransform)
	{
		// ���� ȸ���� ����
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

		// ���� Ʈ�������� Rotation���� �ϳ��� ����޴� ���
		// 
		if ((mbInheritRotX || mbInheritRotY || mbInheritRotZ) && (!bIsCurrent))
		{
			// ���� Ʈ�������� ȸ���� ����޴� ���, ��ġ�� �ٲ��� �� ( ���� )
			SetInheritPosValue();
		}
	}

	// ��� ȸ���� ��������
	Vector3 convertRot = mRelativeRot.ConvertAngle();

	// XMVECTOR4 ���ʹϾ� �� �޾ƿ�
	XMVECTOR quarternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	
	// ��� ȸ�������� ��ȯ�ϴ� ���
	Matrix matRot;
	matRot.RotationQuaternion(quarternion);

	// ���� Ʈ�������� ���� ��� �� ��ȯ
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		mRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		mRelativeAxis[i].Normalize();
	}

	// ���� ȸ������ ���� ��ȯ���
	convertRot = mWorldRot.ConvertAngle();
	quarternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	matRot.RotationQuaternion(quarternion);
	
	// ���� �� ��ȯ
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		mWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		mWorldAxis[i].Normalize();
	}

	mbUpdateRot = true;

	size_t size = mVecChildTransform.size();
	
	// ���� ������Ʈ�鿡�� ����
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
			// �θ� ��ŭ ȸ���� �ִ��� ȸ����� �����Ѵ�.
			Vector3 convertRot = parentRot.ConvertAngle();

			XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

			Matrix matRot;
			matRot.RotationQuaternion(quaternion);

			// �θ� ������Ʈ�� Position�� ����
			Vector3 parentPos = mParentTransform->GetWorldPos();
			
			/*
			������ ȸ����Ŀ� �θ� ������Ʈ�� ��ġ�� ���� �Ѵ�.
			11, 12, 13, 14
			21, 22, 23, 24
			31, 32, 33,	34
			x,   y,   z, 1
			
			_41, _42, _43�� �����Ѵ�.
			-> �θ� ȸ��, ��ġ�� ���� ��ȯ��� �ϼ�
			*/
			memcpy(&matRot._41, &parentPos, sizeof(Vector3));

			// �θ� ȸ����ŭ ȸ����Ű��, �θ��� ��ġ����ŭ �̵��Ѵ�.
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
