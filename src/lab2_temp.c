/**
  ******************************************************************************
  * @file    lab2_temp.c
  * @author  Group 6
  * @version V1.0.0
  * @date    18-October-2013
  * @brief   This defines a temperature reading function using the built-in
	           temperature sensor and produces a display using the four
						 on-board LEDs
  */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include "lab2_filter.h"
#include "lab2_temp.h"

/* Defines ------------------------------------------------------------------*/

#define V25 .760f
#define AVSLOPE .0025f
#define VDD 2.938f

/* Temperature Reader Private Functions ---------------------------------------------------------*/

/** @defgroup Temperature_Reader_Private_Functions
  * @{
  */

/**
  * @brief  Converts ADC value to temperature (in degrees C).
	* @param  raw_value: Raw ADC temperature sensor reading.
  * @retval float: Converted temperature in degrees C.
  */

static float get_temperature(uint16_t raw_value) {
	//Take raw ADC value, scale it from 0-4095 to 0-3V
	//Return this scaled value after sensor calibration function
	return((((VDD*((float)raw_value)/4096 - V25)/AVSLOPE)+25));
}

/**
  * @brief  Turns on the selected LED (modulo 8)
	* @note   This function takes an LED number (from 0-7) and maps it to
            a base-4 system in the following manner:
            led_number -> LED
                 0     ->  0
                 1     ->  0
                 2     ->  1
                 3     ->  1
                 4     ->  2
                 5     ->  2
                 6     ->  3
                 7     ->  3
	* @param  led_number: LED number to turn on.
  * @retval float: Converted temperature in degrees C.
  */
void rotate_led(uint32_t led_number) {
	GPIO_Write(GPIOD, 0x0); //Reset all LEDs
	switch (led_number) { //Only turn the desired LED on
		case 0:
			GPIO_SetBits(GPIOD, GPIO_Pin_12);
			break;
		case 1:
			GPIO_SetBits(GPIOD, GPIO_Pin_12);
			break;
		case 2:
			GPIO_SetBits(GPIOD, GPIO_Pin_13);
			break;
		case 3:
			GPIO_SetBits(GPIOD, GPIO_Pin_13);
			break;
		case 4:
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
			break;
		case 5:
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
			break;
		case 6:
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
			break;
		default:
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
			break;
	}
}

/**
  * @}
  */

/* Temperature Reader Public Functions ---------------------------------------------------------*/

/** @defgroup Temperature_Reader_Public_Functions
  * @{
  */

/**
  * @brief  Initializes Temperature_Reader struct. Should be called before use.
	* @param  *moving_average: Pointer to a Moving_Average filter struct
  * @retval None
  */

void init_temp_reader(struct Temperature_Reader *temperature_reader, uint32_t size) {
	struct Moving_Average moving_average;
	temperature_reader->moving_average = moving_average;
	init_moving_average(&temperature_reader->moving_average, size);
}

/**
  * @brief  Reads the on-board temperature and updated built-in LEDs
	* @param  *moving_average: Pointer to a Moving_Average filter struct
  * @retval None
  */
void read_temp(struct Temperature_Reader *temperature_reader) {
	float current = get_temperature(ADC_GetConversionValue(ADC1)); //Get current temperature value
	insert_value(&temperature_reader->moving_average, current); //Insert newest temperature reading into filter
	calculate_average(&temperature_reader->moving_average); //Calculate averaged temperature
	uint32_t led_number = ((uint32_t)(temperature_reader->moving_average.average) % 8); //Determine which LED should be on depending on temperature
	rotate_led(led_number); //Turn on correct (rotating) LED
}

/**
  * @}
  */
