// Header für die funktionen um die 7-Segment-Anzeige zu kontrollieren
// Hier werden auch einige define variablen erzeugt damit das arbeiten leichter fällt und es übersichtlicher wird

#include "main.h"
#include "stm32f0xx_hal_gpio.h"
#include "math.h"

//Defines um die verschiedenen Commands direkt zur hand zu haben
#define Command_1			    0x40
#define Command_2               0xc0
#define Command_3_on			0x88
#define Command_3_off			0x80

//Die Adressen für das Display um die verschiedenen Segmente zu manipulieren
#define C0H					0xc0
#define C1H					0xc1
#define C2H					0xc2
#define C3H					0xc3
#define C4H					0xc4
#define C5H					0xc5

//Display Darstellungen 0-9 und die Buchstaben H und C



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
#define H                   0x76
#define C                   0x39
#define minus               0x40
#define E                   0x79
#define r                   0x50



//
#define input               'c'
#define output              'o'


//

//für das bessere darstellen der einzelnen zehner potenzen auf dem display
struct bitfield
{
    uint8_t input_data:8;
    uint8_t einer:4;
    uint8_t zehner:4;

};

//hilfs konstrukt für die double_dabble funktion
union dd
{
  uint16_t num;
  struct bitfield raw;

};

//Funktionen um die Flanken zu steuern
void display_clk_high();
void display_clk_low();
void display_dio_high();
void display_dio_low();
void display_both_high();
void display_both_low();

//Funktionen zum starten und stoppen der Commands
void display_start_cmd();
void display_end_cmd();


//Funktionen um Daten/Commands über die Flanken zu realisieren
void read_bit(int *tx_data);

//Funktion um die Commands/Daten an den Chip über die DIO Flanke zu senden
void transmit_command(uint8_t *tx_data);
void transmit_data(uint8_t *tx_data, uint8_t data_length);

//damit der acknowledge empfangen werden kann
void change_pin_mode(char change);

//Check ob es einen Acknowledge vom Chip gibt und somit alles korrekt gesendet wurde
void display_check_acknowledge();


//Testfunktionen zum an und ausstellen des Displays
void display_on();
void display_off();

//Funktion um die empfangenen Daten zu interprettieren
void receive_data(uint16_t data);


//Funktion um die Erhaltenen Daten in ein passendes Format zu bringen damit diese auf dem Display angezeigt werden können
void double_dabble(uint8_t);
uint8_t manipulate_data(uint16_t data);

//Funktion um die Segment-Anzeigen des Displays zu verändern
void display_handle();

//Funktion um die Segmente entsprechend der Zahl zu aktivieren/deaktivieren
uint8_t display_segment(uint8_t data);
