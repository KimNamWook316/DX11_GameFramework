#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"
#include "Tile.h"

class CTileMapComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CTileMapComponent();
    CTileMapComponent(const CTileMapComponent& comp);
    virtual ~CTileMapComponent();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CTileMapComponent* Clone() override;

public:
	void CreateTile(eTileShape eShape, const int countX, const int countY, const Vector2& size);

public:
	void SetBackBaseColor(const Vector4& color);
	void SetBackBaseColor(const float r, const float g, const float b, const float a);
	void SetBackRenderState(class CRenderState* state);
	void SetBackRenderState(const std::string& name);
	void SetBackTransparency(bool bEnable);
	void SetBackOpacity(const float val);
	void AddBackOpacity(const float val);

public:
	void AddBackTexture(const int reg, const int shaderType, const std::string& name,
		class CTexture* texture);
	void AddBackTexture(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddBackTextureFullPath(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fullPath);
	void AddBackTexture(const int reg, const int shaderType, const std::string& name,
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);

	void SetBackTexture(const int index, const int reg, const int shaderType,
		const std::string& name, class CTexture* texture);
	void SetBackTexture(const int index, const int reg, const int shaderType,
		const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetBackTextureFullPath(const int index, const int reg, const int shaderType,
		const std::string& name, const TCHAR* fullPath);
	void SetBackTexture(const int index, const int reg, const int shaderType,
		const std::string& name, const std::vector<TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);

public:
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
    CMaterial* GetMaterial() const
    {
        return mBackMaterial;
    }

	eTileShape GetTileShape() const
	{
		return meTileShape;
	}

	int GetTileCountX() const
	{
		return mCountX;
	}

	int GetTileCountY() const
	{
		return mCountY;
	}

	const Vector2& GetTileSize() const
	{
		return mTileSize;
	}

public:
    void SetBackMaterial(CMaterial* material);
	void SetTileMaterial(CMaterial* material);

private:
	int getTileRenderIndexX(const Vector3& pos);
	int getTileRenderIndexY(const Vector3& pos);

protected:
    CSharedPtr<CSpriteMesh> mBackMesh;
    CSharedPtr<CMaterial> mBackMaterial;
	CSharedPtr<CMaterial> mTileMaterial;
    std::vector<CTile*> mVecTile;
	class CTileMapConstantBuffer* mCBuffer;
	eTileShape meTileShape;
	int mCountX;
	int mCountY;
	int mRenderCount;
	Vector2 mTileSize;
};

