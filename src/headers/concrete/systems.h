#include "system_container.h"

SYSTEM_FUNCT_TYPE(system_player_update);

SYSTEM_FUNCT_TYPE(system_render_static_trans); 	//CMP_STATIC_SPRITE CMP_TRANSFORM
SYSTEM_FUNCT_TYPE(system_render_dynamic_trans); //CMP_DYNAMIC_SPRITE CMP_TRANSFORM

SYSTEM_FUNCT_TYPE(system_render_muldy_rb); 		//CMP_MULTY_SPRITE CMP_RIGID_BODY
SYSTEM_FUNCT_TYPE(system_render_static_rb);		//CMP_STATIC_SPRITE CMP_RIGID_BODY


void prepare_and_add_to_render(uint sheet_index, uint texture_index, mat4 trans);