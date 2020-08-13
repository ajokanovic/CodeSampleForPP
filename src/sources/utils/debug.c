#include "debug.h"

static size_t 		 counter;
static size_t 		 capacity;
static AllocEntry**  container;


void  debug_init_log(size_t initial_capacity)
{
	container = malloc(sizeof(*container) * initial_capacity);
	capacity  = initial_capacity; 
}

void  debug_flush_log()
{
	int i;
	if(!counter)
	{
		printf("No memory leaks were found.\n");
	}
	else
	{
		AllocEntry* ae;
		for (i = 0; i < counter; ++i)
		{
			ae = container[i];
			printf("\n------------------------------------------------------\n");
			printf(" LEAK (%d bytes) IN ---%s--- AT ---%s--- ", ae->size, ae->file_name, ae->line_numb);
			printf("\n------------------------------------------------------\n");

			free(ae);
		}	
	}
	
	free(container);
}

void* debug_malloc(size_t size, char* calling_file, char* calling_line)
{
	void* ret_ptr = malloc(size);

	if(counter + 1 >= capacity)
	{
		capacity *= 2;
		container = realloc(container,capacity);
	}

	AllocEntry* new_entry = malloc(sizeof(*container));
	new_entry->ptr  	  = ret_ptr;
	new_entry->size 	  = size;

	strcpy(new_entry->file_name,  calling_file);
	strcpy(new_entry->line_numb,  calling_line);

	container[counter++] = new_entry;


	return ret_ptr;
}

void* debug_realloc(void *ptr, size_t size)
{
	int   		i;
	void* 		ret_ptr;
	AllocEntry* ae;

	for (i = 0; i < counter; ++i)
	{
		ae = container[i];
		if(ae->ptr == ptr)
			break;
	}

	ret_ptr  = realloc(ptr,size);
	ae->size = size;
	ae->ptr  = ret_ptr;

	return ret_ptr;
}

void  debug_free(void* ptr)
{
	int i;
	AllocEntry* ae;

	for (i = 0; i < counter; ++i)
	{
		ae = container[i];
		if(ae->ptr == ptr)
			break;
	}
	free(ae->ptr);
	free(ae);

	container[i] = container[counter - 1];
	counter--;
}

