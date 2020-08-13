#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <stdarg.h>
#include "vectors.h"
#include "entity_container.h"


#define SYS_INIT_CNT 20
#define SYS_INC_AM   5
#define SYS_MAX_COMP 5
#define SYSTEM_FUNCT_PTR_TYPE(name) void(*name)(double delta, uint number_of_components, ComponentHeader** components)
#define SYSTEM_FUNCT_TYPE(name) void name (double delta, uint number_of_components, ComponentHeader** components)
#define SYSTEM_EXTRACT_COMPONENT(type) system_extract_component(number_of_components, components, type)

typedef enum
{
	SYS_MAIN,
	SYS_RENDER,
	SYS_INTERACT,
	SYS_TYPE_CNT

}SystemType;

typedef struct
{
	Vector_ui components; // types
	SYSTEM_FUNCT_PTR_TYPE(update);

}System;

typedef struct
{
	System* container;
	uint 	size;
	uint 	capacity;

}SystemsContainer;

SystemsContainer systems_containers[SYS_TYPE_CNT];

void systems_container_init(void);
void systems_container_cleanup(void);

System* system_container_allocate(SystemType type, SYSTEM_FUNCT_PTR_TYPE(update), uint number_of_components, ...);

void update_all_systems(SystemType type, double delta);
int  systems_sort_components (const void* a, const void* b);
void* system_extract_component(uint number_of_components, ComponentHeader** components, ComponentType type);

#endif