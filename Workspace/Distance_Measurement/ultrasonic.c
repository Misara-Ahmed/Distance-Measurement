/******************************************************************************
 *
 * Module: ULTRASONIC SENSOR
 *
 * File Name: ultrasonic.c
 *
 * Description: Source file for the ATmega32 ULTRASONIC SENSOR driver
 *
 * Author: Misara Ahmed
 *
 *******************************************************************************/

#include "ultrasonic.h"
#include <util/delay.h>
#include <avr/io.h>

uint8 g_edgeCount = 0;
uint16 g_time = 0;

/* The Configurations for the icu */
Icu_ConfigType icu_Config = { F_CPU_8 , RISING };

/*
 * Description :
 * Function responsible for :
 * -initializing the icu driver.
 * -setup the icu call back function.
 * -Setup the direction for the trigger pin as output pin.
 */
void Ultrasonic_init(void)
{
	/* initializing the icu with the required configurations */
	Icu_init(&icu_Config);

	 /* Setting the callback function. */
	Icu_setCallBack( Ultrasonic_edgeProcessing );

	 /*  Configure the direction for Trigger pin as output pin */
	GPIO_setupPinDirection(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, PIN_OUTPUT);
}
/*
 * Description :
 * Function responsible for sending the trigger pulse to the ultrasonic
 */
void Ultrasonic_Trigger(void)
{
	/* Sending the trigger pulse to the ultrasonic */
	GPIO_writePin(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for calling the Ultrasonic_Trigger function and starting the icu measurement.
 */
uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();

	/*
	 * waiting the icu to give me the time value.
	 */
	while( g_edgeCount != 2 );

	/*
	 *clearing the counter of the edge to start from the beginning again.
	 */
	g_edgeCount = 0;

	/*
	 *calculating the time and returning it.
	 */
	return ( (g_time)*0.017 ) ;
}

/*
 * Description :
 * Function responsible for calculating the high time generated by the ultrasonic sensor.
 */
void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;

	if ( g_edgeCount == 1 )
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if ( g_edgeCount == 2 )
	{
		/* Store the time value */
		g_time = Icu_getInputCaptureValue();
		/*
		 * Clear the timer counter register
		 */
		Icu_clearTimerValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
		}
}
