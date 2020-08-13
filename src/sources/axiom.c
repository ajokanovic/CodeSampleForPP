#include "axiom.h"

static void axiom_init_systems()
{
	AXIOM_ADD_SYSTEM(SYS_RENDER, system_render_muldy_rb, 2, CMP_RIGID_BODY, CMP_MULTY_DYNAMIC_SPRITE);
	AXIOM_ADD_SYSTEM(SYS_RENDER, system_render_dynamic_trans, 2, CMP_TRANSFORM, CMP_DYNAMIC_SPRITE);
	AXIOM_ADD_SYSTEM(SYS_RENDER, system_render_static_rb, 2, CMP_STATIC_SPRITE, CMP_RIGID_BODY);
	AXIOM_ADD_SYSTEM(SYS_MAIN, system_player_update, 3, CMP_MULTY_DYNAMIC_SPRITE, CMP_PLAYER, CMP_RIGID_BODY);
}

static void axiom_load_sheets()
{
	renderer_load_spritesheet(SPRITE_PATH("Spritesheet"), 5, 6); //0
}

static void axiom_load_entities()
{
	TransformParams transform = {0,0,-1,50,50,0};
	entities_add_player(transform);
	//entities_add_animated_object(0,0,20,0.7,transform);
}

void axiom_init()
{
	//must come first so it can track initialisations
	#ifdef DEBUG_MEMORY_MODE
		debug_init_log(1000);
	#endif
		
	component_container_init();
	systems_container_init();
	entity_container_init();

	RendererParams params = {INIT_NEAR,INIT_FAR,INIT_FOV,MSAA};
	renderer_initialise(params);
	physics_init();

	axiom_load_sheets();
	axiom_load_entities();
	axiom_init_systems();
}

void axiom_loop()
{
	uint   fps 			  = 0;
	double lastTime  	  = glfwGetTime();
	double fpsTimeCounter = 0.0;
	double updateTimer 	  = 1.0;
	double frameTime 	  = 1.0/INIT_FPS;

	double currentTime;
	double passedTime;

	while(!callback_events_global.keys[GLFW_KEY_ESCAPE]) 
	{
		currentTime =  glfwGetTime();
		passedTime  = currentTime - lastTime;
		lastTime    = currentTime;

		fpsTimeCounter += passedTime;
		updateTimer    += passedTime;

		if(fpsTimeCounter >= 1.0) 
		{
			double msPerFrame = 1000.0/(double)fps;
			printf("FPS: %f ms (%d fps)\n", msPerFrame, fps);
			fpsTimeCounter = 0;
			fps = 0;
		}
		
		char shouldRender = 0;
		while(updateTimer >= frameTime) 
		{
			glfwPollEvents();
			update_all_systems(SYS_MAIN,frameTime);			

			updateTimer -= frameTime;
			shouldRender = 1;
		}
		
		if(shouldRender) 
		{
			if(callback_events_global.resize_event)
				renderer_resize_window();
			
			update_all_systems(SYS_RENDER, frameTime);
			renderer_draw_all();	
			
			fps++;
		} 
		else 
		{
			Sleep(1);
		}
	}
}

void axiom_clean()
{
	physics_cleanup();
	renderer_cleanup();
	entity_container_cleanup();
	systems_container_cleanup();
	component_container_cleanup();

	//this must be last so it can check if there are any leaks
	#ifdef DEBUG_MEMORY_MODE
		debug_flush_log();
	#endif 
}
