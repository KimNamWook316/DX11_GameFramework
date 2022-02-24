#pragma once

#include "SceneComponent.h"
#include "Wall.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/TileSet/TileSet.h"

class CWallComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CWallComponent();
    CWallComponent(const CWallComponent& com);
    virtual ~CWallComponent();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CWallComponent* Clone() override;

public:
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
    void CreateWall(CTile* tile);
    void ChangeWall(CTile* tile);
    void SetWorldInfo(const int mTileCount);

public:
    void SetTileSet(CTileSet* tileSet);

    void SetShape(eTileShape shape)
    {
        meTileShape = shape;
    }

private:
    CSharedPtr<CSpriteMesh> mMesh;
    CSharedPtr<CTileSet> mTileSet;
    std::vector<CWall*> mVecWall;
    std::vector<TileInfo*> mVecWallInfo;
    class CTileMapConstantBuffer* mCBuffer;
    class CStructuredBuffer* mWallInfoBuffer;
    eTileShape meTileShape;
    Vector2 mWallSize;
    int mWallCount;
    int mRenderCount;
    float mDeltaTime;
};

