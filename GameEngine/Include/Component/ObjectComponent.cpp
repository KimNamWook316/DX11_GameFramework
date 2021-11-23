#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
	SetTypeID<CObjectComponent>();
	meComponentType = eComponentType::OBJ_COMP;
}

CObjectComponent::CObjectComponent(const CObjectComponent& com) :
	CComponent(com)
{
}

CObjectComponent::~CObjectComponent()
{
}
