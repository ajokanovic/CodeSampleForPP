#ifndef COMPONENTS_H
#define COMPONENTS_H

typedef enum
{
	CMP_TRANSFORM,
	CMP_STATIC_SPRITE,
	CMP_DYNAMIC_SPRITE,
	CMP_MULTY_DYNAMIC_SPRITE,
	CMP_RIGID_BODY,
	CMP_PLAYER,
	CMP_CNT

}ComponentType;

typedef struct
{
	ComponentType type;
	void* entity;

}ComponentHeader;


//Only use one of these on an entity, never both!
//-----------------------------------------------
typedef struct
{
	ComponentHeader header;
	vec3  			position;
	vec2  			scale;
	float  			rotation;

}TransformComponent;

typedef struct
{
	ComponentHeader  header;
	PhysicsBodyWrap* body_wrap;
	vec2 			 scale;
	
}RigidBodyComponent;

//---------------------------------------------

typedef struct
{
	ComponentHeader header;
	uint sheet_index;
	uint texture_index;
	uint should_render;

}StaticSpriteComponent;

typedef struct
{
	ComponentHeader header;
	uint sheet_index;
	uint texture_index;
	uint should_render;

	uint num_of_frames;
	uint curr_anim_frame;
	float fps_count;
	float animation_speed;

}DynamicSpriteComponent;

typedef struct
{
	ComponentHeader header;
	uint sheet_index;
	uint should_render;
	uint curr_anim_frame;
	uint animation_index;

	uint texture_index[ANIM_STATE_CNT];
	uint num_of_frames[ANIM_STATE_CNT];

	float fps_count;
	float animation_speed;

}MultiDynamicSpriteComponent;

typedef struct
{
	ComponentHeader header;
	vec2 front_vector;

}PlayerComponent;


uint component_get_size(ComponentType type);

#endif