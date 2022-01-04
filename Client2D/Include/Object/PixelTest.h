#pragma once

#include "GameObject/GameObject.h"

class CPixelTest :
    public CGameObject
{
    friend class CScene;

protected:
    CPixelTest();
    CPixelTest(const CPixelTest& obj);
    virtual ~CPixelTest();
    
public:
	virtual bool Init();
	virtual CPixelTest* Clone();

private:
    CSharedPtr<class CColliderPixel> mBody;
};

