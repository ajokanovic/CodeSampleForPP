#ifndef VECTORS_H
#define VECTORS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VECTOR_ADD_FUNCT_TYPE(SUFIX, DATA_TYPE) void vector_add_##SUFIX(Vector_##SUFIX* vector, DATA_TYPE value)

typedef enum 
{
	VCT_UINT,
	VCT_FLOAT,
	VCT_PTR,
	VCT_CNT

}VectorType;

typedef struct
{
	uint length;
	uint capacity;
	uint increment;

	VectorType type;

}Vector_header;

//if adding new things to individual vectors MODIFY THE DESTROY FUNCTIONS!
typedef struct
{
	Vector_header header;
	float* data;	

} Vector_f;

typedef struct
{
	Vector_header header;
	uint*  data;

} Vector_ui;

typedef struct
{
	Vector_header header;
	void** data;
	void(*delete)(void*);

} Vector_p;


void vector_create_static(Vector_header* dest,uint capacity, uint increment, VectorType type);
void vector_destroy_static(Vector_header* header);

Vector_header* vector_create(uint capacity, uint increment, VectorType type);

void vector_destroy(Vector_header* header);

//remove is not used as much so it stays generic
void vector_remove_last(Vector_header* header);
void vector_remove_at(Vector_header* header, uint index);

void vector_print_all(Vector_header* header);
void vector_print_range(Vector_header* header, uint from, uint to);

void vector_add_array_f(Vector_f* vector, uint data_length, float* data);

VECTOR_ADD_FUNCT_TYPE(ui, uint);
VECTOR_ADD_FUNCT_TYPE(f, float);
VECTOR_ADD_FUNCT_TYPE(p, void*);

void vector_p_add_free_func(Vector_p* vector, void(*delete)(void*));

#endif	//VECTORS_H