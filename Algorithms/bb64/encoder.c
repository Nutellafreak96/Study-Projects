#include "encoder.h"

#define RED "\e[0;31m"
#define NC "\e[0m"

void create_info_container(char_Array *container,char name[])
{
    FILE* ptr;
    char ch;
    
 
    // Opening file in reading mode
    ptr = fopen(name, "r");
   

    if (NULL == ptr) {
        fprintf(stderr,RED "[ERROR]"
               NC  "file can't be opened \n");
        exit(0);
    }

    do{
        ch = fgetc(ptr);
        insert_c_Array(container,ch);     
        
        
 
        // Checking if character is not EOF.
        // If it is EOF stop eading.
    }while (ch != EOF);
    pop_last_c_Array(container);
    

     // Closing the file
    fclose(ptr);

}

int char_to_int(char c)
{
    if(c=='0')
    {
        return 0;
    }
    else{
        return 1;
    }

}

char code_mechanic(int number)
{
    switch(number)
    {
        case 0: return '0'; break;
        case 1: return '1'; break;
        case 2: return '2'; break;
        case 3: return '3'; break;
        case 4: return '4'; break;
        case 5: return '5'; break;
        case 6: return '6'; break;
        case 7: return '7'; break;
        case 8: return '8'; break;
        case 9: return '9'; break;

        case 10: return 'A'; break;
        case 11: return 'B'; break;
        case 12: return 'C'; break;
        case 13: return 'D'; break;
        case 14: return 'E'; break;
        case 15: return 'F'; break;
        case 16: return 'G'; break;
        case 17: return 'H'; break;
        case 18: return 'I'; break;
        case 19: return 'J'; break;
        case 20: return 'K'; break;
        case 21: return 'L'; break;
        case 22: return 'M'; break;
        case 23: return 'N'; break;
        case 24: return 'O'; break;
        case 25: return 'P'; break;
        case 26: return 'Q'; break;
        case 27: return 'R'; break;
        case 28: return 'S'; break;
        case 29: return 'T'; break;
        case 30: return 'U'; break;
        case 31: return 'V'; break;
        case 32: return 'W'; break;
        case 33: return 'X'; break;
        case 34: return 'Y'; break;
        case 35: return 'Z'; break;

        case 36: return 'a'; break;
        case 37: return 'b'; break;
        case 38: return 'c'; break;
        case 39: return 'd'; break;
        case 40: return 'e'; break;
        case 41: return 'f'; break;
        case 42: return 'g'; break;
        case 43: return 'h'; break;
        case 44: return 'i'; break;
        case 45: return 'j'; break;
        case 46: return 'k'; break;
        case 47: return 'l'; break;
        case 48: return 'm'; break;
        case 49: return 'n'; break;
        case 50: return 'o'; break;
        case 51: return 'p'; break;
        case 52: return 'a'; break;
        case 53: return 'r'; break;
        case 54: return 's'; break;
        case 55: return 't'; break;
        case 56: return 'u'; break;
        case 57: return 'v'; break;
        case 58: return 'w'; break;
        case 59: return 'x'; break;
        case 60: return 'y'; break;
        case 61: return 'z'; break;

        case 62: return '-'; break;

        case 63: return '_'; break;

        default: return '='; break;
    };
}

void fillchar(char_Array *container,int number)
{
    int cnt=0;

    while(cnt<number)
    {
        insert_c_Array(container,'0');
        cnt++;
    }
    pop_last_c_Array(container);

}

void grow_int_array(int_Array *container, int number)
{
        int cnt=0;

    while(cnt<=number)
    {
        insert_i_Array(container,0);
        cnt++;
    }
    pop_last_i_Array(container);
    

}

//binary to deci for encoder
void binary_to_deci(int_Array *container1,char_Array *container2)
{   
    int cnt=0;
    int number=0;
    int j=container1->i_size;
    

    

    for(int i=container2->c_size; i >= 0;--i)
    {
        

        if(cnt == 6 || i==0) 
        {
            
            container1->i_array[j]=number;
            
            

            cnt=0;
            number=0;
            j--;

        }

        
        //printf("number= %d + %c * %.2lf\n",number,container2->c_array[i],pow(2,(cnt+1)));
        number= number + (char_to_int(container2->c_array[i])*pow(2,(cnt)));
        
        

        //pop_last_c_Array(container2);
        
        cnt++;
        //printf("%d\tcnt=%d\ti=%i\n",number,cnt,i); 
        

    }   
    


}

//encode deci/binary information
void b64_encoder(int_Array *container1,char_Array *container2, char file_name[])
{

    int filler=0, size=0;

    create_info_container(container2,"test.txt");
    filler=container2->c_size%3;

    fillchar(container2,(filler*8));

    size=container2->c_size/6;  


    grow_int_array(container1,size);

    
    binary_to_deci(container1,container2);
    
    if(container2->c_size > container1->i_size)
    {
        while(container2->c_size != container1->i_size)
        {
            pop_last_c_Array(container2);
        };
    }
    else
    {
        fprintf(stderr,RED "[ERROR]"
            NC  "Something impossible happend \n");
        exit(0);

    };

    for(int i = 0; i <= container2->c_size; i++)
    {
        container2->c_array[i]=code_mechanic(container1->i_array[i]);
    };

    if(filler != 0)
    {
        for(int fill=container2->c_size; fill > (container2->c_size- filler) ;fill--)
        {
            container2->c_array[fill]='=';
        }
    }
}

