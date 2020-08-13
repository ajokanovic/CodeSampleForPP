#ifndef ENTITY_CONTAINER_H
#define ENTITY_CONTAINER_H

#include <stdarg.h>
#include "component_container.h"

#define ENT_INIT_CNT 100
#define ENT_INC_AM   10

typedef struct
{
	uint number_of_components;
	ComponentHeader** components;

}Entity;

typedef struct
{
	Entity* container;
	uint 	size;
	uint 	capacity;

}EntityContainer;

EntityContainer entity_container;

Entity* entity_container_allocate(uint number_of_components,...);

void entity_container_remove(Entity* entity_to_remove);
void entity_container_init(void);
void entity_container_cleanup(void);

ComponentHeader* entity_find_component(Entity* p, ComponentType type);

#endif