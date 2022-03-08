
#include "Component/ObjectComponent.h"
#include "../D2Info.h"

class CD2Projectile :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2Projectile();
    CD2Projectile(const CD2Projectile& com);
    virtual ~CD2Projectile();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2Projectile* Clone() override;

public:
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

public:
    void SetInfo(const std::string& infoName);
    virtual void OnCollideEnter(const CollisionResult& result);

    const D2SkillInfo& GetInfo() const
    {
        return mInfo;
    }

protected:
    D2SkillInfo mInfo;
    CSharedPtr<class CSceneComponent> mRoot;
    CSharedPtr<class CColliderCircle> mCollider;
};
