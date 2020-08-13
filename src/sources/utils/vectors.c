#include "vectors.h"

#define CREATE_VEC_STATIC(HEADER, T_MACRO, T, TYPE_VAR) 			\
	if(TYPE_VAR == T_MACRO)											\
	{																\
		T* vector = HEADER; 										\
		vector->data = malloc(sizeof(*(vector->data)) * capacity);  \
		HEADER->length    = 0;										\
		HEADER->capacity  = capacity; 								\
		HEADER->increment = increment;								\
		HEADER->type      = type;									\
		return;														\
	}								

#define CREATE_VEC_DYNAMIC(T_MACRO, T, TYPE_VAR )					\
	if(TYPE_VAR == T_MACRO)											\
	{																\
		Vector_header* ret_val;										\
		T* vector;													\
		vector       = malloc(sizeof *vector);					    \
		vector->data = malloc(sizeof(*(vector->data)) * capacity);  \
		ret_val      = (Vector_header*)vector;						\
		ret_val->length    = 0;			 							\
		ret_val->capacity  = capacity;   							\
		ret_val->increment = increment;  							\
		ret_val->type      = type;									\
		return ret_val;												\
	}																

#define VECTOR_ADD_FUNCT(FUNCTION_TYPE)								\
	FUNCTION_TYPE 													\
	{																\
		Vector_header* header = &vector->header;					\
		if((header->length + 1) >= header->capacity)				\
		{															\
			header->capacity += header->increment;					\
			vector->data = realloc(vector->data, header->capacity * \
								   sizeof(*(vector->data)));        \
		}															\
		vector->data[header->length++] = value;						\
	}

#define VECTOR_SWAP_IF_TYPE(HEADER, T_MACRO, T, TYPE)				\
	if(HEADER->type == T_MACRO)										\
	{																\
		T* vector = HEADER;											\
		TYPE temp = vector->data[HEADER->length-1];					\
		vector->data[HEADER->length-1] = vector->data[index];		\
		vector->data[index]            = temp;						\
		vector_remove_last(HEADER);									\
		return;														\
	}

#define VECTOR_PRINT_ALL_FOR_TYPE(HEAD, T_MACRO, T, FROM, TO, STR)  \
	if(HEAD->type == T_MACRO)										\
	{																\
		int i;														\
		T* vector = header;											\
		for (i = FROM; i < TO; ++i)									\
			printf(STR,vector->data[i]);							\
		printf("\n");												\
		return;														\
	}


VECTOR_ADD_FUNCT(VECTOR_ADD_FUNCT_TYPE(ui, uint));
VECTOR_ADD_FUNCT(VECTOR_ADD_FUNCT_TYPE(f, float));
VECTOR_ADD_FUNCT(VECTOR_ADD_FUNCT_TYPE(p, void*));

void vector_add_array_f(Vector_f* vector, uint data_length, float* data)
{
	Vector_header* header = &vector->header;
	uint floats_needed = data_length / 4;

	if((header->length + floats_needed/4) >= header->capacity)
	{
		header->capacity += floats_needed;
		vector->data = realloc(vector->data, header->capacity * sizeof(float));	
	}

	memcpy(&(vector->data[header->length]),data,data_length);
	header->length += floats_needed;
}

inline void vector_p_add_free_func(Vector_p* vector, void(*delete)(void*))
{
	vector->delete = delete;
}

void vector_create_static(Vector_header* dest,uint capacity, uint increment, VectorType type)
{
	CREATE_VEC_STATIC(dest, VCT_UINT , Vector_ui, type);
	CREATE_VEC_STATIC(dest, VCT_FLOAT, Vector_f , type);
	CREATE_VEC_STATIC(dest, VCT_PTR  , Vector_p , type);
}

Vector_header* vector_create(uint capacity, uint increment, VectorType type)
{
	CREATE_VEC_DYNAMIC(VCT_UINT , Vector_ui, type);
	CREATE_VEC_DYNAMIC(VCT_FLOAT, Vector_f , type);
	CREATE_VEC_DYNAMIC(VCT_PTR  , Vector_p , type);
}

void vector_destroy_static(Vector_header* header)
{
	//it should be possible to free the data this way as it is not dependant on the pointer type
	Vector_p* vector = (Vector_p*)header;
	
	if(header->type == VCT_PTR && vector->delete != NULL)
	{
		int i;
		for(i = 0; i < vector->header.length; i++)
			vector->delete(vector->data[i]);
	}
	free(vector->data);
}

void vector_destroy(Vector_header* header)
{
	Vector_p* vector = (Vector_p*)header;
	if(header->type == VCT_PTR && vector->delete != NULL)
	{
		int i;
		for(i = 0; i < vector->header.length; i++)
			vector->delete(vector->data[i]);
	}
	free(vector->data);
	free(vector);
}

inline void vector_remove_last(Vector_header* header)
{
	if(header->length == 0)
	{
		ERROR_LOG("ERROR: trying to pop an element from an empty vector.");
		return;
	}
	if(header->type == VCT_PTR)
	{
		Vector_p* vector = header;
		if(vector->delete != NULL)
			vector->delete(vector->data[header->length-1]);
	}

	header->length--;
}

void vector_remove_at(Vector_header* header, uint index)
{
	if(header->length <= index)
	{
		ERROR_LOG("ERROR: trying to remove an element from an invalid index.");
		return;
	}

	if(index == header->length-1)
	{
		vector_remove_last(header);
		return;
	}

	VECTOR_SWAP_IF_TYPE(header, VCT_UINT , Vector_ui, uint);
	VECTOR_SWAP_IF_TYPE(header, VCT_FLOAT, Vector_f , float);
	VECTOR_SWAP_IF_TYPE(header, VCT_PTR  , Vector_p , void*);
}

void vector_print_all(Vector_header* header)
{
	vector_print_range(header, 0, header->length);
}

void vector_print_range(Vector_header* header, uint from, uint to)
{

	if(from >= header->length || to > header->length)
		return;

	VECTOR_PRINT_ALL_FOR_TYPE(header, VCT_UINT , Vector_ui, from, to, "%d|");
	VECTOR_PRINT_ALL_FOR_TYPE(header, VCT_FLOAT, Vector_f , from, to, "%f|");
	VECTOR_PRINT_ALL_FOR_TYPE(header, VCT_PTR  , Vector_p , from, to, "%p|");

}