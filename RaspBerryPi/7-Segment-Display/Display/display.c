#include "display.h"


void activate_gpio_pin()
{
    //CLK PIN
    export_pin(CLK);
    set_direction(out,CLK);
    set_value(LOW,CLK);

    //DIO PIN
    export_pin(DIO);
    set_direction(out,DIO);
    set_value(LOW,DIO);
    printf("activate pin\n");
}
void deactivate_gpio_pin()
{
    //CLK PIN
    set_value(LOW,CLK);
    set_direction(in,CLK);
    unexport_pin(CLK);

    //DIO PIN
    set_value(LOW,DIO);
    set_direction(in,DIO);
    unexport_pin(DIO);
    printf("deactivate pin\n");
}

void display_clk_high() {
    set_value(HIGH,CLK);
    set_value(HIGH,CLK);
    set_value(HIGH,CLK);
}

void display_clk_low() {
    set_value(LOW,CLK);
    set_value(LOW,CLK);
    set_value(LOW,CLK);
}

void display_dio_high() {
    set_value(HIGH,DIO);
    set_value(HIGH,DIO);
    set_value(HIGH,DIO);
}

void display_dio_low() {
    set_value(LOW,DIO);
    set_value(LOW,DIO);
    set_value(LOW,DIO);
}


//Funktionen um die 3 commands zu starten bzw zu beenden.
//Dient grundsätzlich dazu um die Datenübertragung mit dem ersten Command zu starten und nach dem dritten zu beenden.

void display_start_cmd() {
	display_clk_high();
    display_dio_high();

    

	display_dio_low();

	display_clk_low();
}

void display_end_cmd() {
	display_clk_low();
    display_dio_low();


	display_clk_high();

	display_dio_high();
}

//Funktion um die Flanken je nach Bit zu ändern
//Ist wichtig um die Commands bzw. die Daten über die DIO Flane zu übertragen
//tx_data = array mit bits zum starten bzw. beschreiben des registers (commands)
//clk wird für jedes bit einmal auf low und danach wieder auf hihg gestellt um einen clk durchlauf darzustellen
void read_bit(int *tx_data) {

	for (uint8_t j = 0; j < 0x08; j++) //Array shouldnt be extend the size of 8 bits (size of a command)
			{
		display_clk_low();
		if (tx_data[j] == 1)       
				{
			display_dio_high();
		} else {
			display_dio_low();
		}
		display_clk_high();
	}

}

//Funktion um die einzelnen COmmands and das Register des Displays zu senden
void transmit_command(uint8_t *tx_data) {
	int tx_byte[8] = { 0 }; //zum speichern/aufspalten des bytes in einzelne bit

	for (uint8_t i = 0; i < 0x08; i++) {
		tx_byte[i] = (tx_data[0] & (0x01 << i)) && 1; //shifted das byte damit die einzelnen bits gespeichert werden können
	}

	display_start_cmd();                    //started die datenübertragung
	read_bit(tx_byte);                   //darstellen des bytes über die flanken

	display_clk_low();
	display_clk_high(); //CLK von high auf low und wieder auf high stellen die CLK für einen Acknowledge check vorzubereiten


	if (tx_data[0] == 0x40 || tx_data[0] == 0x80 || tx_data[0] == 0x88) {
		display_end_cmd(); //es gibt nur ein stop-signal nach dem ersten command zum entscheiden ob lesen oder schreiben
	}
}

//Funktion um die Daten auf das Display darstellen zu lassen
void transmit_data(uint8_t *tx_data, uint8_t data_length)
{
    int tx_byte[8]={0};     //zum speichern/aufspalten des bytes in einzelne bit

    for(uint8_t i = 0 ; i < data_length ; i++)
    {
        for(int j = 0 ; j < 8 ; j++)
        {
            tx_byte[j] = (tx_data[i]& (0x01 << j)) && 1;          //shifted das byte damit die einzelnen bits gespeichert werden können

        }



        read_bit(tx_byte);                      //darstellen des bytes über die flanken

        display_clk_low();
        display_clk_high();                     //CLK von high auf low und wieder auf high stellen die CLK für einen Acknowledge check vorzubereiten


    }
    display_end_cmd();


}


//Funktionen zum an des Displays
void display_on() {
	uint8_t buffer[4] = { 0};
	uint8_t command[1] = { 0 };

    activate_gpio_pin();

	command[0] = Command_1;
	transmit_command(command);
	command[0] = Command_2;
	transmit_command(command);
	transmit_data(buffer, 4);
	command[0] = Command_3_on;
	transmit_command(command);

}

//Funktion zum abschalten des Displays
void display_off() {
	uint8_t buffer[4] = { 0 };
	uint8_t command[1] = { 0 };

	command[0] = Command_1;
	transmit_command(command);
	command[0] = Command_2;
	transmit_command(command);
	transmit_data(buffer, 4);
	command[0] = Command_3_off;
	transmit_command(command);

    deactivate_gpio_pin();

}
//Funktion zum manipulieren des Displays
void display_handle(uint8_t array[4])
{
    uint8_t local_arr[4]={0};
    uint8_t command[1]={0};
    uint8_t on=0x88;
    local_arr[3]=display_segment(array[3]);
    if(array[2]>0 || array[1]>0)
    {
        local_arr[2]=display_segment(array[2]);
    }
    if(array[1]>0 || array[0]>0)
    {
        local_arr[1]=display_segment(array[1]);        
    }
    if(array[0]>0)
    {
        local_arr[0]=display_segment(array[0]);
    }

    command[0]=Command_1;
    transmit_command(command);
    command[0]=Command_2;
    transmit_command(command);
    transmit_data(local_arr,4);
    //command[0]=0x88;
    transmit_command(&on);

}



//Funktion zum umwandeln der Zahlen zur korrekten Anzeigeform
uint8_t display_segment(uint8_t data)
{
    switch (data)
    {
    case 0x00:
            return zero;
            break;
    case 0x01:
            return one;
            break;
    case 0x02:
            return two;
            break;
    case 0x03:
            return three;
            break;
    case 0x04:
            return four;
            break;
    case 0x05:
            return five;
            break;
    case 0x06:
            return six;
            break;
    case 0x07:
            return seven;
            break;
    case 0x08:
            return eight;
            break;
    case 0x09:
            return nine;
            break;
    }
    return zero;
}

void reset_display(uint8_t array[4])
{
    for(int i=0;i<3;++i)
    {
        array[i]=0;
    }
}

void incr_display(uint8_t array[4],int num)
{
    
    if(num>3 || num<0)
    {
        return;
    }

    //check if highest possible number is shown on display
    if(array[0]>=9 && array[1]>=9  && array[2]>=9 && array[3]>=9)
    {
        reset_display(array);
        return;
    }

    //increment number shown on display
    if(array[num]<9 && array[num]>=0)
    {
        array[num]++;
    }
    else if(array[num]==9)
    {
        array[num]=0;
        incr_display(array,num-1);
    }
    return;
}
void decr_display(uint8_t array[4],int num)
{
    
    if(num>3 || num<0)
    {
        return;
    }

    //check if lowest possible number is shown on display
    if(array[0]==0 && array[1]==0  && array[2]==0 && array[3]==0)
    {
        return;
    }

    //decrement number shown on display
    if(array[num]==0)
    {
        array[num]=9;
        decr_display(array,num-1);
    }
    else
    {
        array[num]--;
    }
    
    return;
}
