#include "d_array.h"


//create container for binary info

void create_info_container(char_Array *container,char name[]);

//encode binary information

void b64_encoder(int_Array *container1,char_Array *container2, char file_name[]);

//functions to help realise the coder

void binary_to_deci(int_Array *container1,char_Array *container2);
int char_to_int(char c);
char code_mechanic(int number);
void fillchar(char_Array *container,int number);
void grow_int_array(int_Array *container, int number);
