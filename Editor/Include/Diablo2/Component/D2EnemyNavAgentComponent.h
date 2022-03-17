#pragma once

#include "Component/NavAgentComponent.h"

class CD2EnemyNavAgentComponent :
    public CNavAgentComponent
{
    friend class CGameObject;

protected:
    CD2EnemyNavAgentComponent();
    CD2EnemyNavAgentComponent(const CD2EnemyNavAgentComponent& com);
    virtual ~CD2EnemyNavAgentComponent();

public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual CD2EnemyNavAgentComponent* Clone() override;
    virtual bool Move(const Vector3& endPos = Vector3(0.f, 0.f, 0.f)) override;
    bool MoveBlockedFront();
    void CancleMove();
    void OnDie();

public:
    void SetWait(bool bWait)
    {
        mbWait = bWait;
    }

public:
    bool IsWaiting() const
    {
        return mbWait;
    }

protected:
	void onFindPathResult(const std::list<Vector3>& pathList) override;

private:
    bool getIsBlockedFront();
    int getAroundPlayerTileIndex();

protected:
    class CD2CharacterInfoComponent* mCharInfo;
    int mCurOnIdx;
    int mPrevOnIdx;
    int mCurReserveIdx;
    int mPlayerIdx;
    bool mbWait;
    Vector3 mEndPos;
};

