/**
 * @file display.h
 * 
 * @brief Header from Course CE31 Embedded System (modified) 
 * @version 0.1
 * @date 2023-12-04
 * 
 * 
 * 
 */

#include <stdint.h>
#include "gpio.h"

/**
 * @brief 
 * 
 */
#define CLK 13
#define DIO 12

/**
 * @brief Commands the Display needs to manipulate the Display
 * 
 */
#define Command_1			    0x40
#define Command_2               0xc0
#define Command_3_on			0x88
#define Command_3_off			0x80


/**
 * @brief Hexdata for the representation of the needed Numbers/Characters
 * 
 */

#define zero                0x3f
#define one                 0x06
#define two                 0x5b
#define three               0x4f
#define four                0x66
#define five                0x6d
#define six                 0x7d
#define seven               0x07
#define eight               0x7f
#define nine                0x6f
#define minus               0x40
#define E                   0x79
#define r                   0x50


/**
 * @brief Functions to activate/deactivate the needed GPIO Pins 
 * 
 */
void activate_gpio_pin();
void deactivate_gpio_pin();


/**
 * @brief Functions to change the GPIO Pin mode (High|Low)
 * 
 */
void display_clk_high();
void display_clk_low();
void display_dio_high();
void display_dio_low();


//Funktionen zum starten und stoppen der Commands
void display_start_cmd();
void display_end_cmd();


//Funktionen um Daten/Commands über die Flanken zu realisieren
void read_bit(int *tx_data);

//Funktion um die Commands/Daten an den Chip über die DIO Flanke zu senden
void transmit_command(uint8_t *tx_data);
void transmit_data(uint8_t *tx_data, uint8_t data_length);


//Testfunktionen zum an und ausstellen des Displays
void display_on();
void display_off();

//Funktion Zahlen umzuwandeln damit sie auf dem Display korrekt angezeigt werden
uint8_t display_segment(uint8_t number);

//Funktion um das Display zu manipulieren
void display_handle();

//Funktionen um das Array für das Display
void reset_display(uint8_t array[4]);
void incr_display(uint8_t array[4],int num);
void decr_display(uint8_t array[4],int num);
