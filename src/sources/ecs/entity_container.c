#include "entity_container.h"


void entity_container_init(void)
{
	entity_container.container = malloc(sizeof(*entity_container.container) * ENT_INIT_CNT);
	entity_container.size 	   = 0;
	entity_container.capacity  = ENT_INIT_CNT;
}

void entity_container_cleanup(void)
{
	uint i;
	for (i = 0; i < entity_container.size; ++i)
		free(entity_container.container[i].components);

	free(entity_container.container);
}

Entity* entity_container_allocate(uint number_of_components,...)
{
	uint   			 i;
	va_list			 ap;
	Entity*			 new_entity;
	ComponentHeader* component_to_add;

	if((entity_container.size+1)  >= entity_container.capacity)
	{
		entity_container.capacity += ENT_INC_AM; 
		entity_container.container = realloc(entity_container.container,sizeof(*entity_container.container) * entity_container.capacity);
	}

	new_entity = &(entity_container.container[entity_container.size]);
	entity_container.size++;

	new_entity->number_of_components = number_of_components;
	new_entity->components 			 = malloc(sizeof(*(new_entity->components)) * number_of_components);

    va_start(ap, number_of_components); 

    for (i = 0; i < number_of_components; ++i)
    {
    	component_to_add 		  = va_arg(ap, ComponentHeader*);
    	component_to_add->entity  = new_entity;
    	new_entity->components[i] = component_to_add;
    }

    va_end(ap); 
    return new_entity;
}


ComponentHeader* entity_find_component(Entity* p, ComponentType type)
{
	uint    i;
	for (i = 0; i < p->number_of_components; ++i)
	{
		if(p->components[i]->type == type)
			return p->components[i];
	}
	return NULL;
	
}

void entity_container_remove(Entity* entity_to_remove)
{
	uint    i;
	uint    entity_size;
	Entity* p   = entity_container.container;
	Entity* end = &(entity_container.container[entity_container.size - 1]);


	for (i = 0; i < entity_to_remove->number_of_components; ++i)
		component_container_remove(entity_to_remove->components[i]);
	

	if(end == entity_to_remove)
	{
		entity_container.size--;
		return;
	}

	entity_size = sizeof(*p);
	for(; p <= end; p++)
	{
		void* temp_memory = malloc(entity_size);

		memcpy(temp_memory,end,entity_size);
		memcpy(end,p,entity_size);
		memcpy(p,temp_memory,entity_size);

		free(temp_memory);

		entity_container.size--;
	}
}
