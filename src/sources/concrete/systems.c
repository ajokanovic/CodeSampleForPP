#include "systems.h"

SYSTEM_FUNCT_TYPE(system_player_update)
{

	MultiDynamicSpriteComponent* state_sprite = SYSTEM_EXTRACT_COMPONENT(CMP_MULTY_DYNAMIC_SPRITE);
	PlayerComponent*  		 	 control      = SYSTEM_EXTRACT_COMPONENT(CMP_PLAYER);
	RigidBodyComponent*    		 rigid_body   = SYSTEM_EXTRACT_COMPONENT(CMP_RIGID_BODY);

    if(!rigid_body->body_wrap->control_body)
        ERROR_LOG_FATAL("Can not move the body without its constraints set. Try calling \"physics_make_body_movable\"");
    
	cpBody* body = rigid_body->body_wrap->control_body;
	cpVect velocity = cpvzero;
	// if(callback_events_global.mouse_update)
	// {
	// 	callback_events_global.mouse_update =0;
	// }
   	uint animation_index = 0;
  	if(callback_events_global.keys[GLFW_KEY_X])
  		global_render_info.xview = !global_render_info.xview;

    if(callback_events_global.keys[GLFW_KEY_W])
    {
    	velocity.y = PLAYER_SPEED;
    	animation_index = 1;
    }
    else if(callback_events_global.keys[GLFW_KEY_S])
    {
    	velocity.y = -PLAYER_SPEED;	
    	animation_index = 2;
    }

    if(callback_events_global.keys[GLFW_KEY_A])
    {
    	velocity.x = -PLAYER_SPEED;
    	animation_index = 3;
    }
    else if(callback_events_global.keys[GLFW_KEY_D])
    {
    	velocity.x = PLAYER_SPEED;
    	animation_index = 4;
    }

    if(state_sprite->animation_index != animation_index)
    {
    	state_sprite->animation_index = animation_index;
    	state_sprite->fps_count = 0;
    	state_sprite->curr_anim_frame = 0;
    }
    
    cpBodySetVelocity(body, velocity);
}

SYSTEM_FUNCT_TYPE(system_render_static_rb)
{
    mat4 tmp_matrix;
    RigidBodyComponent*    rigid_body = SYSTEM_EXTRACT_COMPONENT(CMP_RIGID_BODY);
    StaticSpriteComponent* sprite     = SYSTEM_EXTRACT_COMPONENT(CMP_STATIC_SPRITE);

    if(!sprite->should_render) return;

    cpVect position = cpBodyGetPosition(rigid_body->body_wrap->body);
    RENDER_SYS_TRANSFORM_SPRITE(tmp_matrix, position.x, position.y, -0.3,rigid_body->scale[0], rigid_body->scale[1], 0);
    prepare_and_add_to_render(sprite->sheet_index, sprite->texture_index, tmp_matrix);
}

SYSTEM_FUNCT_TYPE(system_render_muldy_rb)
{
    uint index;
    mat4 tmp_matrix;
    RigidBodyComponent*          rigid_body = SYSTEM_EXTRACT_COMPONENT(CMP_RIGID_BODY);
    MultiDynamicSpriteComponent* sprite     = SYSTEM_EXTRACT_COMPONENT(CMP_MULTY_DYNAMIC_SPRITE);

    if(!sprite->should_render) return;

    index = sprite->animation_index;
    RENDER_SYS_PROCEED_ANIMATION(sprite,sprite->num_of_frames[index],delta)
    
    cpVect position = cpBodyGetPosition(rigid_body->body_wrap->body);
    RENDER_SYS_TRANSFORM_SPRITE(tmp_matrix, position.x, position.y, -0.3,
        rigid_body->scale[0], rigid_body->scale[1], 0
    );
    prepare_and_add_to_render(sprite->sheet_index, sprite->texture_index[index] + sprite->curr_anim_frame, tmp_matrix);
}

SYSTEM_FUNCT_TYPE(system_render_dynamic_trans)
{
    mat4 tmp_matrix;
    DynamicSpriteComponent* sprite = SYSTEM_EXTRACT_COMPONENT(CMP_DYNAMIC_SPRITE);
    TransformComponent* transform  = SYSTEM_EXTRACT_COMPONENT(CMP_TRANSFORM);

    if(!sprite->should_render) return;

    RENDER_SYS_PROCEED_ANIMATION(sprite,sprite->num_of_frames,delta)
    RENDER_SYS_TRANSFORM_SPRITE(tmp_matrix, transform->position[0], transform->position[1], transform->position[2],
        transform->scale[0], transform->scale[1], transform->rotation
    );
    prepare_and_add_to_render(sprite->sheet_index, sprite->texture_index + sprite->curr_anim_frame, tmp_matrix);
}

SYSTEM_FUNCT_TYPE(system_render_static_trans)
{
    mat4 tmp_matrix;
    StaticSpriteComponent* sprite = SYSTEM_EXTRACT_COMPONENT(CMP_STATIC_SPRITE);
    TransformComponent* transform = SYSTEM_EXTRACT_COMPONENT(CMP_TRANSFORM);

    if(!sprite->should_render)return;

    RENDER_SYS_TRANSFORM_SPRITE(tmp_matrix, transform->position[0], transform->position[1], transform->position[2],
        transform->scale[0], transform->scale[1], transform->rotation
    );
    prepare_and_add_to_render(sprite->sheet_index, sprite->texture_index, tmp_matrix);
}


void prepare_and_add_to_render(uint sheet_index, uint texture_index, mat4 trans)
{
    int i, j;
    float data[SPRITE_INSTANCE_LEN] = {0};
    data[0] = texture_index;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            data[4+(i*4+j)] = trans[i][j];

    renderer_add_to_batch(sheet_index, sizeof(data), data);
}