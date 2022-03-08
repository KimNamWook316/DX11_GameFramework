#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"
#include "../Resource/TileSet/TileSet.h"
#include "WallComponent.h"
#include "Tile.h"

struct PathFindTileInfo
{
	eTileType Type;
	Vector3 Pos;
	Vector2 Size;
	Vector3 Center;
	int IndexX;
	int IndexY;
	int Index;
};

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
	int GetTileIndexX(const Vector3& pos);
	int GetTileIndexY(const Vector3& pos);
	int GetTileIndex(const Vector3& pos);
	CTile* GetTile(const Vector3& pos);
	CTile* GetTile(const int x, const int y);
	CTile* GetTile(const int idx);

public:
	int GetPathFindTileIndexX(const Vector3& pos);
	int GetPathFindTileIndexY(const Vector3& pos);
	int GetPathFindTileIndex(const Vector3& pos);
	PathFindTileInfo* GetPathFindTile(const Vector3& pos);
	PathFindTileInfo* GetPathFindTile(const int x, const int y);
	PathFindTileInfo* GetPathFindTile(const int idx);

public:
	bool CreateTile(CTileSet* tileSet, const int countX, const int countY, const Vector2& size);
	bool CreateTile(const int countX, const int countY, const Vector2& size);
	bool CreateTileProcedual(const ProcedualMapData& mapData);
	void ClearTile();
	bool CreateWall();
	bool CreateWallComponent();
	void SetNavigationData();

	void SetWorldInfo();
	void SetTileDefaultInfo(const std::string& tileName);
	void SetTileInfo(const int idxX, const int idxY, const std::string& tileName);
	void SetTileInfo(const int idx, const std::string& tileName);
	void SetTileInfo(const Vector3& pos, const std::string& tileName);
	void SetTileOpacity(const int idxX, const int idxY, const float opacity);
	void SetTileOpacity(const int idx, const float opacity);
	void SetTileOpacity(const Vector3& pos, const float opacity);
	void SetTileColor(eTileType type, const float r, const float g, const float b, const float a);
	void SetTileColor(eTileType type, const Vector4& color);

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
	void EnableEditMode(bool bEnable)
	{
		mbEditMode = bEnable;
	}

public:
    CMaterial* GetBackMaterial() const
    {
        return mBackMaterial;
    }

	CTileSet* GetTileSet() const
	{
		return mTileSet;
	}

	CWallComponent* GetWallComponent() const
	{
		return mWallComponent;
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

	float GetTileDiagonal() const
	{
		return mTileDiagonal;
	}

	int GetPathFindTileCountX() const
	{
		return mPathFindTileCountX;
	}

	int GetPathFindTileCountY() const
	{
		return mPathFindTileCountY;
	}

	const Vector2& GetTileSize() const
	{
		return mTileSize;
	}

	const Vector2& GetPathFindTileSize() const
	{
		return mPathFindTileSize;
	}

	const Matrix& GetMatIsoToWorld() const
	{
		return mMatIsoToWorld;
	}

	const Matrix& GetMatWorldToIso() const
	{
		return mMatWorldToIso;
	}

public:
    void SetBackMaterial(CMaterial* material);
	void SetTileSet(CTileSet* tileSet);
	void SetWallTileSet(CTileSet* tileSet);

private:
	int getTileRenderIndexX(const Vector3& pos);
	int getTileRenderIndexY(const Vector3& pos);
	void setPathFindTileType(const int renderTileIdx);
	bool createPathFindTileInfo();
	void setPathFindRenderInfo();

protected:
    CSharedPtr<CSpriteMesh> mBackMesh;
    CSharedPtr<CMaterial> mBackMaterial;
	CSharedPtr<CTileSet> mTileSet;
    std::vector<CTile*> mVecTile;
	std::vector<PathFindTileInfo*> mVecPathFindTile;
	std::vector<TileInfo> mVecTileInfo;
	class CTileMapConstantBuffer* mCBuffer;
	class CStructuredBuffer* mTileInfoBuffer;
	eTileShape meTileShape;
	int mCountX;
	int mCountY;
	int mCount;
	int mPathFindTileCountX;
	int mPathFindTileCountY;
	int mPathFindTileCount;
	int mRenderCount;
	Vector2 mTileSize;
	Vector2 mPathFindTileSize;
	Vector4 mTileColor[(int)eTileType::Max];
	bool mbEditMode;
	float mTileDiagonal;
	float mPathFindTileDiagonal;
	Matrix mMatWorldToIso;
	Matrix mMatIsoToWorld;
	float mDetlaTime;

	// Debug
	CSharedPtr<class CStaticMesh> mPathFindTileMesh;
	CSharedPtr<class CShader> mPathFindTileShader;
	std::vector<PathFindTileRenderInfo> mVecPathFindTileRenderInfo;
	class CStructuredBuffer* mPathFindTileRenderInfoBuffer;
	int mPathFindTileRenderCount;
	int mSelectPathTileIdx;

	// For Random Map
	TileSetInfo* mArrTileSetInfo[(int)eTileType::Max];
	
	CSharedPtr<CWallComponent> mWallComponent;
};

