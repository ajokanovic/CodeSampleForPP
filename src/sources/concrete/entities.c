#include "entities.h"


void entities_add_static_body_sprite(uint sprite_sheet, uint texture_index, TransformParams transform, PhysicsTypeID type_id)
{
	StaticSpriteComponent*  static_sprite_comp  = component_container_allocate(CMP_STATIC_SPRITE);      
	RigidBodyComponent*     body_comp    		= component_container_allocate(CMP_RIGID_BODY);
	entity_container_allocate(2, static_sprite_comp, body_comp);
	
	static_sprite_comp->sheet_index    = sprite_sheet;
	static_sprite_comp->texture_index  = texture_index;
	static_sprite_comp->should_render  = 1;
									
	PhysicsBodyParams body_params = { 
		transform.x, 
		transform.y, 
		transform.sx, 
		transform.sy, 
		PLAYER_MASS, 
		PLAYER_ELASTIC, 
		PLEYR_FRICTION, 
		type_id
	}	;

	body_comp->body_wrap = physics_create_aabb_body(body_params);
	V2_FILL(body_comp->scale,transform.sx, transform.sy);
}

void entities_add_animated_object(uint sprite_sheet, uint texture_index, uint keyframe_count, float animation_speed, TransformParams transform)
{
	DynamicSpriteComponent*  dynamic_sprite_comp  = component_container_allocate(CMP_DYNAMIC_SPRITE);
	TransformComponent*      transform_comp       = component_container_allocate(CMP_TRANSFORM);
	entity_container_allocate(2, dynamic_sprite_comp, transform_comp);
	
	dynamic_sprite_comp->sheet_index     = sprite_sheet;
	dynamic_sprite_comp->texture_index   = texture_index;
	dynamic_sprite_comp->should_render   = 1;
	dynamic_sprite_comp->num_of_frames   = keyframe_count;
	dynamic_sprite_comp->fps_count       = 0.0f;
	dynamic_sprite_comp->animation_speed = animation_speed;
	dynamic_sprite_comp->curr_anim_frame = 0;
									
	V3_FILL(transform_comp->position, transform.x, transform.y, transform.z);
	V2_FILL(transform_comp->scale, transform.sx, transform.sy);
	transform_comp->rotation = transform.rot;
}

void entities_add_player(TransformParams transform)
{	
	uint i;
	float animation_speed 				     = 0.9f;
	uint16 sheet 		  				     = 0;
	uint16 animations[ANIM_STATE_CNT] 		 = {0 , 6  , 12 , 24 , 18};
	uint16 animation_lengths[ANIM_STATE_CNT] = {6 , 6  , 6  , 6  , 6 };

	PhysicsBodyParams body_params = { 
		transform.x, 
		transform.y, 
		transform.sx, 
		transform.sy, 
		PLAYER_MASS, 
		PLAYER_ELASTIC, 
		PLEYR_FRICTION, 
		PHYSICS_PLAYER
	};

	PhysicsControlParams control_params = {
		1,
		120000.0f,
		0,
		1.2f,
		50000.0f
	};

	MultiDynamicSpriteComponent* state_sprite_sprite_comp = component_container_allocate(CMP_MULTY_DYNAMIC_SPRITE);
	PlayerComponent*   	 		 control_comp 			  = component_container_allocate(CMP_PLAYER);
	RigidBodyComponent*    		 body_comp    			  = component_container_allocate(CMP_RIGID_BODY);

	entity_container_allocate(3, state_sprite_sprite_comp, control_comp, body_comp);
	
	state_sprite_sprite_comp->sheet_index = sheet;
	for(i = 0; i<ANIM_STATE_CNT; i++)
	{
		state_sprite_sprite_comp->texture_index[i] = animations[i];
		state_sprite_sprite_comp->num_of_frames[i] = animation_lengths[i];
	}

	state_sprite_sprite_comp->should_render   = 1;
	state_sprite_sprite_comp->fps_count 	  = 0.0f;
	state_sprite_sprite_comp->animation_speed = animation_speed;
	state_sprite_sprite_comp->curr_anim_frame = 0;
	state_sprite_sprite_comp->animation_index = 0;

	body_comp->body_wrap = physics_create_aabb_body(body_params); 
	physics_set_control_body(body_comp->body_wrap, control_params);

	V2_FILL(body_comp->scale,transform.sx, transform.sy);
	V2_FILL(control_comp->front_vector,0.0f,1.0f);						
}

void entities_add_animated_object(uint sprite_sheet, uint texture_index, uint keyframe_count, float animation_speed, TransformParams transform)
{
	DynamicSpriteComponent*  dynamic_sprite_comp  = component_container_allocate(CMP_DYNAMIC_SPRITE);
	TransformComponent*      transform_comp       = component_container_allocate(CMP_TRANSFORM);
	entity_container_allocate(2, dynamic_sprite_comp, transform_comp);
	
	dynamic_sprite_comp->sheet_index     = sprite_sheet;
	dynamic_sprite_comp->texture_index   = texture_index;
	dynamic_sprite_comp->should_render   = 1;
	dynamic_sprite_comp->num_of_frames   = keyframe_count;
	dynamic_sprite_comp->fps_count       = 0.0f;
	dynamic_sprite_comp->animation_speed = animation_speed;
	dynamic_sprite_comp->curr_anim_frame = 0;
									
	V3_FILL(transform_comp->position, transform.x, transform.y, transform.z);
	V2_FILL(transform_comp->scale, transform.sx, transform.sy);
	transform_comp->rotation = transform.rot;
}