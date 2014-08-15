/*
 *  ===========================================================================
 *  Processing Scope Demo
 *  
 *  main.cpp
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

/* mbed implements digital output pins as a class. The LED_GREEN flag tells the
    controller which digital output pin to use. */
DigitalOut myLED( LED_GREEN );
/* mbed also defines a class for input/output over the USB port, which will
    allow us to talk to the computer. */
Serial pc( USBTX, USBRX );

// Convenience function that returns a random float between 0 and 1.
float randFloat ()
{
    /* rand() generates a random integer. To convert that to a decimal number,
        we have to divide it by the maximum integer rand() can produce, which
        is stored in the system constant RAND_MAX. Both of these values have to
        be cast to floats before the division, because in C++ dividing two ints
        results in another int--hence, this function would only return 0 or 1,
        and never anything in between. */
    return ( (float)rand() / (float)RAND_MAX );
}

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

    const int   kNChannels  = 3;            // Number of channels to generate
    const float kRandScale  = 0.05f;        // Maximum step size
    const float kFs         = 100.0f;       // Desired step frequency, Hz

    // -- Initialization -- //

    /* The Serial.baud function sets the "baud rate" of the serial connection--
        that is, how many bytes per second travel over the line. Here we've set
        it to the maximum value to make sure we don't run into any lag. */
    pc.baud( 115200 );

    /* The random number generator has to be initialized with a "seed". To make
        sure our random numbers are as random as possible, we use the system
        timer as a seed. */
    srand( time( NULL ) );

    // Compute the desiredd time difference between steps.
    const float kDt = 1.0f / kFs;

    // Declare and initialize our data array.
    float data [kNChannels];
    for ( int i = 0; i < kNChannels; i ++ )
        data[ i ] = 0.0f;

    // Start our serial communication by printing our initialized data
    printArray( data, kNChannels );

    // Initialize the timers.
    Timer appClock, cycleClock;
    appClock.start();
    cycleClock.start();

    // Keep track of the number of seconds since we started.
    int nSeconds = 0;

    // Main program loop; go forever.
    while ( true )
    {

        // Has enough time passed? If not, keep waiting.
        if ( cycleClock.read() < kDt )
            continue;
        // Restart the clock for the next cycle.
        cycleClock.stop(); cycleClock.reset(); cycleClock.start();

        // For each data point, take a random step.
        for ( int i = 0; i < kNChannels; i ++ )
            data[ i ] = data[ i ] + kRandScale * ( -1.0f + 2.0f * randFloat() );

        // Check and see if we've added another second.
        int curNSeconds = (int)appClock.read();
        if ( curNSeconds > nSeconds )
        {
            // If so, update the second counter and toggle the LED.
            nSeconds = curNSeconds;
            myLED = !myLED;
        }

        // Finally, print the data over the serial port.
        printArray( data, kNChannels );

    }

    /* main() returning 0 is the computer way of signaling "SUCCESS". The
        program will never get here, but we'll throw it in anyway. */
    return 0;

}




















