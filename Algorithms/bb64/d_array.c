#include "d_array.h"
/*

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
*/

/*
typedef struct
{
    char *c_array;
    size_t c_used;
    size_t c_size;
} char_Array;*/

// initialize array
void init_c_Array(char_Array *a, size_t initialSize)
{
    a->c_array = malloc(initialSize * sizeof(char));
    a->c_used = 0;
    a->c_size = initialSize;
}

// resize array
void insert_c_Array(char_Array *a, char element)
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->c_used == a->c_size)
    {     
        a->c_size += 1;
        a->c_array = realloc(a->c_array, a->c_size * sizeof(char));
    }
    a->c_array[a->c_used++] = element;
}

void pop_last_c_Array(char_Array *a)
{
    if(a->c_size != 0)
    {
        a->c_size -=1;        
        a->c_used -=1;
        a->c_array = realloc(a->c_array, a->c_size * sizeof(char));

    }
    else
    {
        a->c_array = realloc(a->c_array, a->c_size * sizeof(char));
        a->c_used = a->c_size = 0;        
    }
}


// free the allocated memory
void free_c_Array(char_Array *a)
{
    free(a->c_array);
    a->c_array = NULL;
    a->c_used = a->c_size = 0;
}

//same for int Arrays

/*typedef struct
{
    int *i_array;
    size_t i_used;
    size_t i_size;
} int_Array;*/

// initialize array
void init_i_Array(int_Array *a, size_t initialSize)
{
    a->i_array = malloc(initialSize * sizeof(int));
    a->i_used = 0;
    a->i_size = initialSize;
}

// resize array
void insert_i_Array(int_Array *a, int element)
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->i_used == a->i_size)
    {     
        a->i_size += 1;
        a->i_array = realloc(a->i_array, a->i_size * sizeof(int));
    }
    a->i_array[a->i_used++] = element;
}

void pop_last_i_Array(int_Array *a)
{
    if(a->i_size != 0)
    {
        a->i_size -=1;        
        a->i_used -=1;
        a->i_array = realloc(a->i_array, a->i_size * sizeof(int));

    }
    else
    {
        a->i_array = realloc(a->i_array, a->i_size * sizeof(int));
        a->i_used = a->i_size = 0;
    }
}


// free the allocated memory
void free_i_Array(int_Array *a)
{
    free(a->i_array);
    a->i_array = NULL;
    a->i_used = a->i_size = 0;
}

// https://stackoverflow.com/questions/3536153/c-dynamically-growing-array


//Example of use
/*Array a;
int i;

initArray(&a, 5); // initially 5 elements
for (i = 0; i < 100; i++)
    insertArray(&a, i);     // automatically resizes as necessary
printf("%d\n", a.array[9]); // print 10th element
printf("%d\n", a.size);     // print number of elements
freeArray(&a);*/

