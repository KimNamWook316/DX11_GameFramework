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

	// ���� ����ü�� ������ DX���� �˷��ֱ� ���� InputDesc �迭�� �߰��ϱ� ���� �Լ�
	void AddInputDesc(const char* name, unsigned int index,
		DXGI_FORMAT fmt, unsigned int slot, unsigned int size,
		D3D11_INPUT_CLASSIFICATION slotClass,
		unsigned int instanceDataStep);
	// ���� InputLayout ���� �Լ�
	bool CreateInputLayout();

	// ���̴� ����(.fx) �κ��� ���� ���̴� ��ü �����ϴ� �Լ�
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
	// InputLayout�̶�, Vertex�� �� ������ � �뵵���� DirectX���� �˷��ֱ� ���� ��ü�̴�.
	// InputDesc�迭�� ���� ����ȴ�.
	// ���� ������ �� �����, inputDesc�迭(���⼭�� vector)�� ���Ұ� �� ������ �Ѵ�.
	ID3D11InputLayout* mInputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> mVecInputDesc;
	unsigned int mInputSize;
	
	// ID3D11 Shader : ����Ʈ�ڵ带 ���� �����Ǵ� ���� ���̴� ��ü
	// ID3DBlob		 : fx���� �������� ���� �����Ǵ� ���̴� ����Ʈ �ڵ�
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