//#define DEBUG_MEMORY_MODE
#ifdef DEBUG_MEMORY_MODE
	#include "debug.h"
	#define free(ptr)    	  debug_free(ptr)
	#define malloc(size) 	  debug_malloc(size)
	#define realloc(ptr,size) debug_realloc(ptr,size)
#endif

#include <windows.h>
#include "entities.h"
#include "render_systems.h"
#include "renderer.h"

#define AXIOM_ADD_SYSTEM(type, name, num_of_comp, args...) system_container_allocate(type, name, num_of_comp, args);

static void axiom_init_systems(void);
static void axiom_load_sheets(void);
static void axiom_load_entities(void);

void axiom_init(void);
void axiom_loop(void);
void axiom_clean(void);
