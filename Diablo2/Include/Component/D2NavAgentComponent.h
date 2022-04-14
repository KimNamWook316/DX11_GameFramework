#pragma once

#include "Component/NavAgentComponent.h"

class CD2NavAgentComponent :
    public CNavAgentComponent
{
    friend class CGameObject;

protected:
    CD2NavAgentComponent();
    CD2NavAgentComponent(const CD2NavAgentComponent& com);
    virtual ~CD2NavAgentComponent();

public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual CD2NavAgentComponent* Clone() override;
    virtual bool Move(const Vector3& endPos) override;
    void CancleMove();

protected:
	void onFindPathResult(const std::list<Vector3>& pathList) override;

private:
    bool getIsBlockedFront();
    bool findPath();

protected:
    class CD2CharacterInfoComponent* mCharInfo;
    bool mbMoveLinear;
    int mCurOnIdx;
    int mPrevOnIdx;
    eTileType ePrevOnTileType;
    Vector3 mEndPos;
};

