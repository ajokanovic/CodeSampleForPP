#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct 
{
	void*  ptr;
	size_t size;
	char   file_name[128];
	char   line_numb[128];

}AllocEntry;

void  debug_flush_log(void);
void  debug_init_log(size_t initial_capacity);
void* debug_malloc(size_t size, char* calling_file, char* calling_line);
void* debug_realloc(void *ptr, size_t size);
void  debug_free(void* ptr);

#endif