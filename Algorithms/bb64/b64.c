#include "encoder.h"



int main(void)
{
    char_Array ch;
    int_Array in;
    char file_name[50];


    init_c_Array(&ch,1);
    init_i_Array(&in,1);

    printf("Enter File name:\n");

    scanf("%[^\n]s", file_name);

    b64_encoder(&in,&ch,file_name);  

    
    for(int i = 0; i <= ch.c_size; i++)
    {
        printf(" %c ",ch.c_array[i]);
    };
 


    free_c_Array(&ch);
    free_i_Array(&in);   
    

    
    return 0;
}
