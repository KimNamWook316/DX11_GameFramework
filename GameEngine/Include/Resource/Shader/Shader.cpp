#include "Shader.h"

CShader::CShader()
{
}

CShader::CShader(const CShader& shader)
{
	*this = shader;
	mRefCount = 0;
}

CShader::~CShader()
{
}
