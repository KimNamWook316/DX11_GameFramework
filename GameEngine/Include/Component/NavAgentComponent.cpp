#include "NavAgentComponent.h"
#include "SceneComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/NavigationManager.h"
#include "../GameObject/GameObject.h"

CNavAgentComponent::CNavAgentComponent()    :
    mMoveSpeed(0.f)
{
    SetTypeID<CNavAgentComponent>();
}

CNavAgentComponent::CNavAgentComponent(const CNavAgentComponent& com)   :
    CObjectComponent(com)
{
    mMoveSpeed = com.mMoveSpeed;
}

CNavAgentComponent::~CNavAgentComponent()
{
}

bool CNavAgentComponent::Init()
{
    return true;
}

void CNavAgentComponent::Start()
{
    if (!mUpdateComponent)
    {
        mUpdateComponent = mObject->GetRootSceneComponent();
    }
}

void CNavAgentComponent::Update(float deltaTime)
{
    if (mUpdateComponent)
    {
        if (!mPathList.empty())
        {
            Vector3 targetPos = mPathList.front();
            Vector3 pos = mUpdateComponent->GetWorldPos();

            float targetDist = pos.Distance(targetPos);

            Vector3 dir = targetPos - pos;
            dir.Normalize();

            float dist = mMoveSpeed * deltaTime;

            if (targetDist <= dist)
            {
                mPathList.pop_front();
                dist = targetDist;
            }
            
            mUpdateComponent->AddWorldPos(dir * dist);

            if (mPathList.empty())
            {
                if (mEndCallBack)
                {
                    mEndCallBack();
                    mEndCallBack = nullptr;
                }
            }
            else
            {
				if (mFrameCallBack)
				{
					mFrameCallBack();
				}
            }
        }
    }
}

void CNavAgentComponent::PostUpdate(float deltaTime)
{
}

void CNavAgentComponent::PrevRender()
{
}

void CNavAgentComponent::Render()
{
}

void CNavAgentComponent::PostRender()
{
}

CNavAgentComponent* CNavAgentComponent::Clone()
{
    return new CNavAgentComponent(*this);
}

void CNavAgentComponent::SetUpdateComponent(CSceneComponent* updateComponent)
{
    mUpdateComponent = updateComponent;
}

bool CNavAgentComponent::Move(const Vector3& endPos)
{
    if (!mUpdateComponent)
    {
        return false;
    }

    int endIdx = mScene->GetNavigationManager()->GetIndex(endPos);

    if (mPrevEndIdx == endIdx)
    {
        return false;
    }

    mPrevEndIdx = endIdx;

    return mScene->GetNavigationManager()->FindPath<CNavAgentComponent>(this, &CNavAgentComponent::onFindPathResult,
        mUpdateComponent->GetWorldPos(), endPos);
}

void CNavAgentComponent::onFindPathResult(const std::list<Vector3>& pathList)
{
    mPathList.clear();
    mPathList = pathList;
}
