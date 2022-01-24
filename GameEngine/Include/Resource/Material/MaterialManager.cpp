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

	// 기본 Material 생성
	CreateMaterial<CMaterial>("Color");

	CSharedPtr<CMaterial> mtrl = FindMaterial("Color");

	mtrl->SetShader("ColorMeshShader");

	// 텍스쳐 Material 생성
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
		// 자기 자신만이 가지고 있다면, 지운다.
		// CSharedPtr이 소멸되면서, Release()호출되며 RefCount 0이 된다.
		if (iter->second->GetRefCount() == 1)
		{
			mMapMaterial.erase(iter);
		}
	}
}
