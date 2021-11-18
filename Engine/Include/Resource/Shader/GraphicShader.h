#pragma once

#include "Shader.h"

class CGraphicShader : public CShader
{
	friend class CShaderManager;

protected:
	CGraphicShader();
	virtual ~CGraphicShader() = 0;

public:
	virtual bool Init() = 0;
	virtual void SetShader();

	// 정점 구조체의 성분을 DX에게 알려주기 위한 InputDesc 배열을 추가하기 위한 함수
	void AddInputDesc(const char* name, unsigned int index,
		DXGI_FORMAT fmt, unsigned int slot, unsigned int size,
		D3D11_INPUT_CLASSIFICATION slotClass,
		unsigned int instanceDataStep);
	// 실제 InputLayout 생성 함수
	bool CreateInputLayout();

	// 쉐이더 파일(.fx) 로부터 실제 쉐이더 객체 생성하는 함수
	bool LoadVertexShader(const char* entryName, const TCHAR* fileName,
		const std::string& pathName);
	bool LoadPixelShader(const char* entryName, const TCHAR* fileName,
		const std::string& pathName);
	bool LoadHullShader(const char* entryName, const TCHAR* fileName,
		const std::string& pathName);
	bool LoadDomainShader(const char* entryName, const TCHAR* fileName,
		const std::string& pathName);
	bool LoadGeometryShader(const char* entryName, const TCHAR* fileName,
		const std::string& pathName);

private:
	// InputLayout이란, Vertex의 각 성분이 어떤 용도인지 DirectX에게 알려주기 위한 객체이다.
	// InputDesc배열을 통해 구축된다.
	// 정점 성분이 두 개라면, inputDesc배열(여기서는 vector)의 원소가 두 개여야 한다.
	ID3D11InputLayout* mInputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> mVecInputDesc;
	unsigned int mInputSize;
	
	// ID3D11 Shader : 바이트코드를 통해 생성되는 실제 쉐이더 객체
	// ID3DBlob		 : fx파일 컴파일을 통해 생성되는 쉐이더 바이트 코드
	ID3D11VertexShader* mVS;
	ID3DBlob* mVSBlob;

	ID3D11PixelShader* mPS;
	ID3DBlob* mPSBlob;

	ID3D11HullShader* mHS;
	ID3DBlob* mHSBlob;

	ID3D11DomainShader* mDS;
	ID3DBlob* mDSBlob;

	ID3D11GeometryShader* mGS;
	ID3DBlob* mGSBlob;
};