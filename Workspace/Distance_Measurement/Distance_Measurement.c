/******************************************************************************
 *
 * Project Name: Fan Controller
 *
 * File Name: Distance_Measurement.c
 *
 * Description: Source file for the ATmega32 Distance Measurement project
 *
 * Author: Misara Ahmed
 *
 *******************************************************************************/
#include <avr/io.h>
#include "ultrasonic.h"
#include "lcd.h"
#include <util/delay.h>

int main(void)
{
	uint16 distance = 0;

	/* Initializing the LCD */
	LCD_init();

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Initializing the Ultrasonic */
	Ultrasonic_init();

	/*
	 * Setup the LCD cursor to display where you want.
	 */
	LCD_moveCursor(0,0);
	LCD_displayString("Distance= ");
	LCD_moveCursor(0,14);
	LCD_displayString("cm");

	while(1)
	{
		/* Getting the distance */
		distance = Ultrasonic_readDistance();

		/* Display the distance value every time at same position */
		LCD_moveCursor(0,10);

		if(distance <= 100)
		{
			LCD_intgerToString(distance);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayString(" ");
		}
		else if( distance <= 10 )
		{
			LCD_intgerToString(distance);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayString("  ");
		}
		else
		{
			LCD_intgerToString(distance);
		}
	}
}
