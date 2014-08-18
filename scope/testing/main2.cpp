/*
 *  ===========================================================================
 *  Processing Scope Demo
 *  
 *  main2.cpp
 *  
 *  Demo main file for mbed microcontroller. Generates three independent random
 *  walks, and transmits them over the serial port.
 *  
 *  Copyright (C) 2014 JHU BME Builds
 *  ===========================================================================
 */

// Include for general mbed library for this microprocessor.
#include "mbed.h"
// Include to gain access to system timer functions.
#include "time.h"

#include "DHT22.h"

/* mbed implements digital output pins as a class. The LED_GREEN flag tells the
    controller which digital output pin to use. */
DigitalOut myLED( LED_GREEN );
/* mbed also defines a class for input/output over the USB port, which will
    allow us to talk to the computer. */
Serial pc( USBTX, USBRX );

DHT22 dht22( D0 );

// Convenience function that sends an array over the serial connection.
void printArray( float array[], int length )
{
    // Print out each data point with a space after it, except the last one
    for ( int i = 0; i < ( length - 1 ); i ++ )
        pc.printf( "%2.4f ", array[ i ] );
    // The last one has no space, but has a newline.
    pc.printf( "%2.4f\n", array[ length - 1 ] );
}

// The main entry point of our program.
int main ()
{

    // -- Parameter definitions -- //

    const int   kNChannels  = 2;            // Number of channels to generate
    const float kFs         = 250.0f;       // Desired step frequency, Hz

    // -- Initialization -- //

    /* The Serial.baud function sets the "baud rate" of the serial connection--
        that is, how many bytes per second travel over the line. Here we've set
        it to the maximum value to make sure we don't run into any lag. */
    pc.baud( 115200 );

    while (true)
    {

    	myLED = !myLED;
	    //pc.printf("Waiting ...\n");
	    wait(2.1f);

	    myLED = !myLED;
	    DHT22_ERROR err = dht22.readData();
	    if ( err == DHT_ERROR_NONE )
	    {
	    	pc.printf("%4.2f %4.2f\n", dht22.getTemperatureC(), dht22.getHumidity());

	        //pc.printf("Temperature is %f.2 C\n", dht22.getTemperatureC());
	        //pc.printf("Humidity is %f.2 pct.\n", dht22.getHumidity());
	    }

	}

    /* main() returning 0 is the computer way of signaling "SUCCESS". The
        program will never get here, but we'll throw it in anyway. */
    return 0;

}




















