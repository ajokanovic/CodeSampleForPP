#ifndef COMPONENT_CONTAINER_H
#define COMPONENT_CONTAINER_H
#include <string.h> 
#include <stdlib.h>
#include "components.h"


#define COMP_INIT_CNT 100
#define COMP_INC_AM     5

typedef struct 
{
	ComponentHeader* components[CMP_CNT];
	uint sizes[CMP_CNT];
	uint capacities[CMP_CNT];
	char resort_systems[CMP_CNT];
	char resort_sys_flag;
}ComponentContainer;

ComponentContainer component_container;

void component_container_init(void);
void component_container_cleanup(void);
ComponentHeader* component_container_allocate(ComponentType type);
void component_container_remove(ComponentHeader* header);
ComponentHeader* component_container_get(ComponentType type, uint index);

#endif