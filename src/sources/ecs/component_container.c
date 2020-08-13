#include "component_container.h"

void component_container_init(void)
{
	uint i;
	for (i = 0; i < CMP_CNT; ++i)
	{
		component_container.components[i] 	  = malloc(component_get_size((ComponentType)i) * COMP_INIT_CNT);
		component_container.capacities[i] 	  = COMP_INIT_CNT; 
		component_container.sizes[i] 	  	  = 0;
		component_container.resort_systems[i] = 0;
		component_container.resort_sys_flag   = 0;
	}
}

inline void component_container_cleanup(void)
{
	uint i;
	for (i = 0; i < CMP_CNT; ++i)
		free(component_container.components[i]);
}

ComponentHeader* component_container_allocate(ComponentType type)
{
	uint component_size = component_get_size(type);
	if(component_container.sizes[type]+1 >= component_container.capacities[type])
	{
		component_container.capacities[type] += COMP_INC_AM;
		component_container.components[type]  = realloc(component_container.components[type], component_container.capacities[type] * component_size);
	}
	
	component_container.sizes[type]++;

	char* ret = component_container.components[type];
	ret      += component_size * (component_container.sizes[type]-1);

	((ComponentHeader*)ret)->type 	         = type;
	((ComponentHeader*)ret)->entity          = NULL;

	component_container.resort_systems[type] = 1;
	component_container.resort_sys_flag      = 1;
	return ret;
}

inline ComponentHeader* component_container_get(ComponentType type, uint index)
{
	return ((char*)&(component_container.components[type][0])) + (component_get_size(type) * index);
}

void component_container_remove(ComponentHeader* header)
{
	ComponentType type    = header->type;
	uint component_size   = component_get_size(type);

	void *p    = component_container.components[type];
	void *end  = p + component_size * (component_container.sizes[type]-1);

	if(!component_container.sizes[type])
		return;
	
	if(component_container.sizes[type] == 1)
	{
		component_container.sizes[type] = 0;
		return;
	}

	if(header == end)
	{
		component_container.sizes[type]--;
		return;
	}

	for (; p <= end; p += component_size )
	{
		if(p == header)
		{
			void* temp_memory = malloc(component_size);

			memcpy(temp_memory,end,component_size);
			memcpy(end,p,component_size);
			memcpy(p,temp_memory,component_size);

			free(temp_memory);

			component_container.sizes[type]--;

			break;
		}
	}
	component_container.resort_systems[type] = 1;
	component_container.resort_sys_flag      = 1;
}