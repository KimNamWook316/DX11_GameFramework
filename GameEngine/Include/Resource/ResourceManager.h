#pragma once

#include "../GameInfo.h"

// 다양한 리소스들을 모두 관리할 매니저 (메쉬, 오디오 등등 모든 리소스는 이 객체에서만 관리한다.)
class CResourceManager
{
public:
	bool Init();

	DECLARE_SINGLE(CResourceManager)
};

