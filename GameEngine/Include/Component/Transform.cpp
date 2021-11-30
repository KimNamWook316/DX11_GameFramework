#include "Transform.h"
#include "../Resource/Shader/TransformConstantBuffer.h"

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
	mbUpdatePos(true),
	mCBuffer(nullptr)
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

	mCBuffer = trans.mCBuffer->Clone();
}

CTransform::~CTransform()
{
	SAFE_DELETE(mCBuffer);
}

void CTransform::Start()
{
	// ���� �� �� �� ��ġ�� �������־�� �Ѵ�.
	SetInheritScaleValue();
	SetInheritRotValue(true);
	SetInheritPosValue();
}

void CTransform::Init()
{
	// ��� ���� ����
	mCBuffer = new CTransformConstantBuffer;

	mCBuffer->Init();
}

CTransform* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Update(const float deltaTime)
{
}

void CTransform::PostUpdate(const float deltaTime)
{
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
		mMatPos.Translation(mWorldPos);
	}

	if (mbUpdateScale || mbUpdateRot || mbUpdatePos)
	{
		mMatWorld = mMatScale * mMatRot * mMatPos;
	}

	if (mbUpdateScale)
	{
		mbUpdateScale = false;
	}

	if (mbUpdateRot)
	{
		mbUpdateRot = false;
	}

	if (mbUpdatePos)
	{
		mbUpdatePos = false;
	}
}

// ��� ���� ����
void CTransform::SetTransformBuffer()
{
	mCBuffer->SetWorldMatrix(mMatWorld);
	
	// ���� ���� ����� �����., y�� �Ʒ����� ������ �����Ѵ�.
	Matrix matProj;
	matProj = XMMatrixOrthographicOffCenterLH(0.f, 1280.f, 0.f, 720.f, 0.f, 1000.f);

	mCBuffer->SetProjMatrix(matProj);

	mCBuffer->SetPivot(mPivot);
	mCBuffer->SetMeshSize(mMeshSize);

	// ���� ��� ����ϰ�, ���̴��� ����
	mCBuffer->UpdateCBuffer();
}

// ���� ����� �ۿ��� �����ؾ� �� �� ȣ��
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
		mVecChildTransform[i]->SetInheritRotValue(false);
	}
}

// ȸ�� ��� ���� ������� relative position�� �����Ǿ�� �� ��쿡��??
// -> ��� ��ġ ���� �θ��� ���� ��ġ�� �ٲ���� �ٲ��� �ʴ´�.
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
			
			mWorldPos = mRelativePos.TransformCoord(matRot);
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

void CTransform::SetInheritWorldScaleValue()
{
	// ���� Ʈ�������� �ְ�, ����� ������ ������� ���
	if (mParentTransform && mbInheritScale)
	{
		// ��� ������ = �θ� ���彺���� ��� �ڽ��� ���彺���� ��
		mRelativeScale = mWorldScale / mParentTransform->GetWorldScale();
	}

	mbUpdateScale = true;

	size_t size = mVecChildTransform.size();

	// ���� ������Ʈ�鵵 ������ ����
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildTransform[i]->SetInheritWorldScaleValue();
	}
}

void CTransform::SetInheritWorldRotValue(bool bIsCurrent)
{
	if (mParentTransform)
	{
		// ���� ȸ���� ����
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

		// ���� Ʈ�������� Rotation���� �ϳ��� ����޴� ��� and ù ��°�� ȣ��� ���� �ƴ� ��� ��ġ�� �����Ѵ�.
		if ((mbInheritRotX || mbInheritRotY || mbInheritRotZ) && !bIsCurrent)
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

			// ���� ��ġ = ��� ��ġ * �θ� ���� ��ȯ ����̸�,
			// ��� ��ġ = ���� ��ġ * �θ� ���� ��ȯ�� ������̴�.
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