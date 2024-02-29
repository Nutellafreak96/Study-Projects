#include "display.h"

uint8_t display_array[4]={0,0,6,3};


int main()
{   
    printf("hello world\n");
    sleep(1);
    display_on();
    sleep(1);
    display_handle(display_array);
    sleep(1);
    int cnt=1;
    while (cnt<=5)
    {
        
        decr_display(display_array,3);
        display_handle(display_array);
        sleep(1);
        cnt++;

    }
    display_off();
    
    


    return 0;
}