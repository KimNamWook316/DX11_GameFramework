#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
	SetTypeID<CObjectComponent>();
	meComponentType = Component_Type::OBJ_COMP;
}

CObjectComponent::CObjectComponent(const CObjectComponent& com) :
	CComponent(com)
{
}

CObjectComponent::~CObjectComponent()
{
}
