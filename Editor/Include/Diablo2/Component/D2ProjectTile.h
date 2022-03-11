
#include "Component/ObjectComponent.h"
#include "D2SkillObject.h"
#include "../D2Info.h"

class CD2Projectile :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2Projectile();
    CD2Projectile(const CD2Projectile& com);
    virtual ~CD2Projectile();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual CD2Projectile* Clone() override;

public:
    virtual void SetDir(const Vector2& dir);
    
public:
    virtual void OnCollideEnter(const CollisionResult& result) override;

public:
    CSharedPtr<class CColliderCircle> mCollider;
};
