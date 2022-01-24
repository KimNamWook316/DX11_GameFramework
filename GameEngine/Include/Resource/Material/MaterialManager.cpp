#include "MaterialManager.h"
#include "../ResourceManager.h"
#include "../Shader/MaterialConstantBuffer.h"

CMaterialManager::CMaterialManager()	:
	mCBuffer(nullptr)
{
}

CMaterialManager::~CMaterialManager()
{
	SAFE_DELETE(mCBuffer);
}

bool CMaterialManager::Init()
{
	mCBuffer = new CMaterialConstantBuffer;
	mCBuffer->Init();

	// �⺻ Material ����
	CreateMaterial<CMaterial>("Color");

	CSharedPtr<CMaterial> mtrl = FindMaterial("Color");

	mtrl->SetShader("ColorMeshShader");

	// �ؽ��� Material ����
	CreateMaterial<CMaterial>("BaseTexture");
	mtrl = FindMaterial("BaseTexture");
	mtrl->SetShader("Mesh2DShader");
	CTexture* tex = CResourceManager::GetInst()->FindTexture("EngineDefaultTexture");
	mtrl->AddTexture(0, (int)eBufferShaderTypeFlags::Pixel, "EngineDefaultTexture", tex);
	mtrl->SetBaseColor(1.f, 1.f, 1.f, 1.f);

	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& name)
{
	auto iter = mMapMaterial.find(name);

	if (iter == mMapMaterial.end())
	{
		return nullptr;
	}
	
	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& name)
{
	auto iter = mMapMaterial.find(name);

	if (iter != mMapMaterial.end())
	{
		// �ڱ� �ڽŸ��� ������ �ִٸ�, �����.
		// CSharedPtr�� �Ҹ�Ǹ鼭, Release()ȣ��Ǹ� RefCount 0�� �ȴ�.
		if (iter->second->GetRefCount() == 1)
		{
			mMapMaterial.erase(iter);
		}
	}
}
