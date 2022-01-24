#pragma once

#include "../../GameInfo.h"

class CShaderManager
{
	friend class CResourceManager;

private:
	CShaderManager();
	~CShaderManager();

public:
	bool Init();
	class CShader* FindShader(const std::string& name);
	class CConstantBuffer* FindConstantBuffer(const std::string& name);
	void ReleaseShader(const std::string& name);

public:
	bool CreateConstantBuffer(const std::string& name, const int size, const int reg,
		const int eConstantBufferShaderType = (int)eBufferShaderTypeFlags::All);

	// Client를 사용하는 유저가 쉐이더를 정의할 수 있다.
	// 어떤 Shader Class가 들어올 지 모르므로, 템플릿으로 정의한다.
	template <typename T>
	bool CreateShader(const std::string& name)
	{
		T* shader = (T*)FindShader(name);

		if (shader)
		{
			return false;
		}

		shader = new T;

		if (!shader->Init())
		{
			return false;
		}
		shader->SetName(name);

		mMapShader.insert(std::make_pair(name, shader));
		return true;
	}

private:
	std::unordered_map<std::string, CSharedPtr<class CShader>> mMapShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>> mMapConstantBuffer;
};

