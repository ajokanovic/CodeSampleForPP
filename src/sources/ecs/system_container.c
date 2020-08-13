#include "system_container.h"

inline void systems_container_init(void)
{
	uint i;
	for(i = 0; i < SYS_TYPE_CNT; ++i)
	{
		systems_containers[i].container = malloc(sizeof(*systems_containers[i].container) * SYS_INIT_CNT);
		systems_containers[i].size 		= 0;
		systems_containers[i].capacity  = SYS_INIT_CNT;
	}
}

inline void systems_container_cleanup(void)
{
	uint i,j;
	for (i = 0; i < SYS_TYPE_CNT; ++i)
	{
		for (j = 0; j < systems_containers[i].size; ++j)
			vector_destroy_static(&(systems_containers[i].container[j].components));

		free(systems_containers[i].container);
	}
}

System* system_container_allocate(SystemType type, SYSTEM_FUNCT_PTR_TYPE(update), uint number_of_components, ...)
{
	int     i;
	va_list ap;

	SystemsContainer* systems_container = &(systems_containers[type]);

	if((systems_container->size+1) >= systems_container->capacity)
	{
		systems_container->capacity += SYS_INC_AM; 
		systems_container->container = realloc(systems_container->container,sizeof(*systems_container->container) * systems_container->capacity);
	}

	System* ret = &(systems_container->container[systems_container->size]);
	systems_container->size++;

	vector_create_static(&(ret->components),2,1,VCT_UINT);
	ret->update = update;

    va_start(ap, number_of_components); 

    for (i = 0; i < number_of_components; ++i)
    	vector_add_ui(&(ret->components),va_arg(ap, uint)); 

    va_end(ap); 

    return ret;
}

int systems_sort_components(const void * a, const void * b) 
{
   return (component_container.sizes[*(int*)a] - component_container.sizes[*(int*)b]);
}


void update_all_systems(SystemType type, double delta)
{

	uint          	 i, j,k;
	uint          	 number_of_components;
	uint*         	 comp_types;
	char 			 valid;
	System*       	 system;
	ComponentType 	 comp_type;
	ComponentHeader* comp_args[SYS_MAX_COMP];

	SystemsContainer* systems_container = &(systems_containers[type]);

	for(k = 0; k < systems_container->size; ++k)
	{
		system               = &(systems_container->container[k]);
		number_of_components = system->components.header.length;
		comp_types 			 = system->components.data;

		if(number_of_components == 1)
		{
			comp_type = *comp_types;
			for (j = 0; j < component_container.sizes[comp_type]; ++j)
			{
				comp_args[0] = component_container_get(comp_type,j);
				system->update(delta, number_of_components, comp_args);
			}
				
			continue;
		}

		if(component_container.resort_sys_flag)
		{
			for (i = 0; i < number_of_components; ++i)
			{
				if(component_container.resort_systems[comp_types[i]])
				{
					qsort(comp_types, number_of_components, sizeof(*comp_types), systems_sort_components);	
					
					for(j = 0; j < number_of_components; ++j)	
						component_container.resort_systems[comp_types[j]] = 0;

					break;
				}
			}
		}

		comp_type = comp_types[0];
		for(i = 0; i < component_container.sizes[comp_type]; ++i)
		{
			
			valid 		 = 1;
			comp_args[0] = component_container_get(comp_type,i);
			void* entity = comp_args[0]->entity;

			if(entity == NULL)
				ERROR_LOG_FATAL("All Components must be attached to an entity!");

			if(((Entity*)entity)->number_of_components == 1)
				continue;

			for(j = 1; j < number_of_components; ++j)
			{
				comp_args[j] = entity_find_component(entity, comp_types[j]);
				if(comp_args[j] == NULL)
				{
					valid = 0;
					break;
				}
			}

			if(!valid)
				continue;

			system->update(delta, number_of_components, comp_args);
		}
	}
	component_container.resort_sys_flag = 0;
}

void* system_extract_component(uint number_of_components, ComponentHeader** components, ComponentType type)
{
	int i;
	for(i = 0; i < number_of_components; i++)
		if(components[i]->type == type)
			return components[i];

	return NULL;
}