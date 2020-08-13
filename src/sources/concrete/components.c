#include "components.h"

uint component_get_size(ComponentType type)
{
	switch(type)
	{
		case CMP_TRANSFORM:
			return sizeof(TransformComponent);
		break;

		case CMP_STATIC_SPRITE:
			return sizeof(StaticSpriteComponent);
		break;

		case CMP_DYNAMIC_SPRITE:
			return sizeof(DynamicSpriteComponent);
		break;

		case CMP_RIGID_BODY:
			return sizeof(RigidBodyComponent);
		break;
		
		case CMP_MULTY_DYNAMIC_SPRITE:
			return sizeof(MultiDynamicSpriteComponent);
		break;
		
		case CMP_PLAYER:
			return sizeof(PlayerComponent);
		break;

	}
}