#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <string.h>

typedef struct
{
    char *c_array;
    size_t c_used;
    size_t c_size;
} char_Array;

// initialize array
void init_c_Array(char_Array *a, size_t initialSize);
void insert_c_Array(char_Array *a, char element);

void pop_last_c_Array(char_Array *a);



// free the allocated memory
void free_c_Array(char_Array *a);

//same for int Arrays

typedef struct
{
    int *i_array;
    size_t i_used;
    size_t i_size;
} int_Array;

// initialize array
void init_i_Array(int_Array *a, size_t initialSize);

// resize array
void insert_i_Array(int_Array *a, int element);

void pop_last_i_Array(int_Array *a);



// free the allocated memory
void free_i_Array(int_Array *a);


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
