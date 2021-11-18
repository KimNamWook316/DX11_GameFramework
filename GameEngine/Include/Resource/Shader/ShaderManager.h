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

public:
	// Client�� ����ϴ� ������ ���̴��� ������ �� �ִ�.
	// � Shader Class�� ���� �� �𸣹Ƿ�, ���ø����� �����Ѵ�.
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

		mMapShader.insert(std::make_pair(name, shader));
		return true;
	}

private:
	std::unordered_map<std::string, CSharedPtr<class CShader>> mMapShader;
};

