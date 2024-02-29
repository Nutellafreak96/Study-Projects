/**
 * @file gpio.h
 * 
 * @brief Copied from the Website which was given in the Exercise from  Prof. Bauer 
 * 
 * https://buyzero.de/blogs/news/alles-uber-gpio-pins
 * 
 * @version 0.1
 * @date 2023-12-04
 * 
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Directories for the Files to controll the GPIOs of the Pi 
 */
#define GPIO_ROOT "/sys/class/gpio"
#define EXPORT "/sys/class/gpio/export"
#define UNEXPORT "/sys/class/gpio/unexport"

/**
 * @brief Pin Modes
 * 
 */
 #define in "in"
 #define out "out"

/**
 * Pin Voltage Modes 
 * 0 -> Low (no Voltage)
 * 1 -> High (Voltage)
 */
#define LOW 0
#define HIGH 1



/**
 * @brief Create Directory to manipulate the GPIO Pin
 * 
 * @param num: Number of the GPIO Pin 
 * @return int: Returns 0 if succeeded
 */
int export_pin(int num);

/**
 * @brief  Delete Directory to manipulate the GPIO Pin
 * 
 * @param num: Number of the GPIO Pin 
 * @return int: Returns 0 if succeeded
 */
int unexport_pin(int num);


/**
 * @brief Set the direction of the GPIO Pin (Read/Input | Write/Output)
 * 
 * @param mode: value can be in|out
 * @param num: Number of the GPIO Pin 
 * @return int: Returns 0 if succeeded
 */
int set_direction(char *mode, int num);


/**
 * @brief Set the value object
 * 
 * @param val: value can be 0 or 1 | see defines at the top 
 * @param num: Number of the GPIO Pin 
 * @return int: Returns 0 if succeeded
 */
int set_value(int val, int num);