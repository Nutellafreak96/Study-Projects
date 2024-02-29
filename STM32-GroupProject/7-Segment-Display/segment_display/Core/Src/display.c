/*
 * display.c
 *
 *  Created on: May 24, 2023
 *      Author: kevin
 */

/*
 * display.c
 *
 *  Created on: May 24, 2023
 *      Author: kevin
 */

// Code zu den Funktionen des Headers um die 7-Segment-Anzeige zu kontrollieren

#include "display.h"

//globale variablen

//zum speichern der daten (besseres darstellen)
uint8_t display_data[4];
union dd dd_data;

//Funktionen um die Flanke am CLK- bzw. DIO-Eingang auf high oder low zu schalten.
//Dient in späteren funktionen dazu Daten zu senden bzw. die Datenübertragung zu starten.
void display_clk_high() {
	HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET);
}

void display_clk_low() {
	HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET);
}

void display_dio_high() {
	HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_SET);
}

void display_dio_low() {
	HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_RESET);
}

void display_both_high() {
	HAL_GPIO_WritePin(GPIOA, DIO_Pin | CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DIO_Pin | CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DIO_Pin | CLK_Pin, GPIO_PIN_SET);
}

void display_both_low() {
	HAL_GPIO_WritePin(GPIOA, DIO_Pin | CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, DIO_Pin | CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, DIO_Pin | CLK_Pin, GPIO_PIN_RESET);
}

//Funktionen um die 3 commands zu starten bzw zu beenden.
//Dient grundsätzlich dazu um die Datenübertragung mit dem ersten Command zu starten und nach dem dritten zu beenden.

void display_start_cmd() {
	display_both_high();

	display_dio_low();

	display_clk_low();
}

void display_end_cmd() {
	display_both_low();

	display_clk_high();

	display_dio_high();

}

//Funktion um die Flanken je nach Bit zu ändern
//Ist wichtig um die Commands bzw. die Daten über die DIO Flane zu übertragen
//tx_data = array mit bits zum starten bzw. beschreiben des registers (commands)
//clk wird für jedes bit einmal auf low und danach wieder auf hihg gestellt um einen clk durchlauf darzustellen
void read_bit(int *tx_data) {

	for (uint8_t j = 0; j < 0x08; j++) //Array sollte nicht gößer sein als 8, da ein Command max 8 bits lang ist und auch die Einstellungen zur Anzeige max 8 bit lang ist.
			{
		display_clk_low();
		if (tx_data[j] == GPIO_PIN_SET)       //GPIO_PIN_SET = 1 laut Definition
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

	display_check_acknowledge(); //zum überprüfen ob der command funktioniert hat

	if (tx_data[0] == 0x40 || tx_data[0] == 0x80 || tx_data[0] == 0x88) {
		display_end_cmd(); //es gibt nur ein stop-signal nach dem ersten command zum entscheiden ob lesen oder schreiben
	}
}

//Funktion um die Daten auf das Display darstellen zu lassen
void transmit_data(uint8_t *tx_data, uint8_t data_length) {
	int tx_byte[8] = { 0 }; //zum speichern/aufspalten des bytes in einzelne bit

	for (uint8_t i = 0; i < data_length; i++) {
		for (int j = 0; j < 8; j++) {
			tx_byte[j] = (tx_data[i] & (0x01 << j)) && 1; //shifted das byte damit die einzelnen bits gespeichert werden können

		}

		read_bit(tx_byte);               //darstellen des bytes über die flanken

		display_clk_low();
		display_clk_high(); //CLK von high auf low und wieder auf high stellen die CLK für einen Acknowledge check vorzubereiten

		display_check_acknowledge(); //zum überprüfen ob der command funktioniert hat

	}
	display_end_cmd();

}

//funktion um den DIO Pin zwischen Inout und Output MOdus wechseln zu lassen
//dient dazu um dac ack bit empfangen zu können
void change_pin_mode(char change) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = CLK_Pin;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	switch (change) {
	case 'i':
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		break;

	case 'o':
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		break;
	}

}

//Funktion zum auf die Antword des Chips vom Display zu warten
//Dafür muss der DIO Pin auf Input umgestellt werden
//und danach wieder initialisiert werden
//dafür dient der generierte code als hilfestellung
void display_check_acknowledge() {
	change_pin_mode(input);
	display_clk_low();

	while(HAL_GPIO_ReadPin(CLK_GPIO_Port, CLK_Pin))
	{
		HAL_GPIO_WritePin(led_GPIO_Port, led_Pin,SET);
		
	}
	HAL_GPIO_WritePin(led_GPIO_Port, led_Pin,RESET);
	change_pin_mode(output);
}

//Funktionen zum an des Displays
void display_on() {
	uint8_t buffer[4] = { 0 };
	uint8_t command[1] = { 0 };

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

}

//Funktion welche die Anzeige je nach Daten ändert
void display_handle() {
	uint8_t command[1] = { 0 };
	uint8_t on = 0x88;

	command[0] = Command_1;
	transmit_command(command);
	command[0] = Command_2;
	transmit_command(command);
	transmit_data(display_data, 4);
	//command[0]=0x88;
	transmit_command(&on);

}

//Funktion welche den gemessenen Wert rundet
uint8_t manipulate_data(uint16_t data) {
	
    
    uint8_t num;
	data = (data & 0b0011111111111111);

    if(data > 994)
    {
        return 99;
    }
    
	float transform = round(((float) data / 10));

	num = (uint16_t) transform;

	return num;

}

//Funktion welche die Daten verändert um eine korrekte Darstellung der Anzeige zu gewährleisten
void double_dabble(uint8_t data) {
	union dd wandler;
	wandler.num = 0x00;

	wandler.raw.input_data = data;

	for (int i = 0; i < 7; i++) {
		wandler.num = wandler.num << 1;
		if (wandler.raw.zehner >= 5) {
			wandler.raw.zehner += 3;

		}
		if (wandler.raw.einer >= 5) {
			wandler.raw.einer += 3;

		}
	}
	wandler.num = wandler.num << 1;

	dd_data = wandler;
}

//Funktion um die Daten in das Vorhandene Array zu speichern damit diese Dargestellt werden können
void receive_data(uint16_t data) {

	if ((0b1100000000000000 & data) == 0b0100000000000000) {
		display_data[0] = H;
		display_data[1] = 0;
		display_data[2] = display_segment(dd_data.raw.zehner);
		display_data[3] = display_segment(dd_data.raw.einer);
		return;
	} else if ((0b1100000000000000 & data) == 0b1000000000000000) {
		display_data[0] = C;
		display_data[1] = minus;
		display_data[2] = display_segment(dd_data.raw.zehner);
		display_data[3] = display_segment(dd_data.raw.einer);
		return;
	} else if ((0b1100000000000000 & data) == 0b0000000000000000) {
		display_data[0] = C;
		display_data[1] = 0;
		display_data[2] = display_segment(dd_data.raw.zehner);
		display_data[3] = display_segment(dd_data.raw.einer);
		return;
	}	
	else if((0b1100000000000011 & data) == 0b1100000000000001)
	{
		display_data[0] = E;
		display_data[1] = r;
		display_data[2] = r;
		display_data[3] = two;
		return;
	}
	else if((0b1100000000000011 & data) == 0b110000000000000010)
	{
		display_data[0] = E;
		display_data[1] = r;
		display_data[2] = r;
		display_data[3] = three;
		return;
	}
	else if((0b1100000000000011 & data) == 0b1100000000000000)
	{
		display_data[0] = E;
		display_data[1] = r;
		display_data[2] = r;
		display_data[3] = one;
		return;
	}

		display_data[0] = E;
		display_data[1] = r;
		display_data[2] = r;
		display_data[3] = 0;



}

//Funktion um die Segmente je nach gesendeter Daten zu aktivieren
uint8_t display_segment(uint8_t data) {
	switch (data) {
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
}

