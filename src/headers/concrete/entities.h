#ifndef ENTITIES_H
#define ENTITIES_H

#include "entity_container.h"
void entities_add_static_body_sprite(uint sprite_sheet, uint texture_index, TransformParams transform, PhysicsTypeID type_id);
void entities_add_animated_object(uint sprite_sheet, uint texture_index, uint keyframe_count, float animation_speed, TransformParams transform);
void entities_add_player(TransformParams transform);
#endif