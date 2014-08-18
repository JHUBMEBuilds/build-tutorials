/* mbed DHT22 Library
 * Copyright (c) 2011, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MBED_DHT22_H
#define MBED_DHT22_H

#include "mbed.h"

/**
 * Currently supports DHT22 (SparkFun Electronics)
 * Humidity and Temperature Sensor
 *
 * Features:
 *  >   3.3-6V Input
 *  >   1-1.5mA measuring current
 *  >   40-50uA standby current
 *  >   Humidity from 0-100% RH
 *  >   -40 - 80 degrees C  temperature range
 *  >   +-2% RH accuracy
 *  >   +-0.5 degrees C
 *
 * Usages
 * #include "DHT22.h"
 *
 * DTH22 dth22(PinName sda, PinName scl, int addr);  // pin
 *
 * int main() {
 *     err=dth22.readData();
 *     if (err==0) {
          printf("Temperature is %f.2 C\n",dht22.getTemperature());
          printf("Humidity is %f.2 \%\n",dht22.getHumidity());
       }
 *
 *
 */

#define DHT22_ERROR_VALUE -99.5

typedef enum {
    DHT_ERROR_NONE = 0,
    DHT_BUS_HUNG,
    DHT_ERROR_NOT_PRESENT,
    DHT_ERROR_ACK_TOO_LONG,
    DHT_ERROR_SYNC_TIMEOUT,
    DHT_ERROR_DATA_TIMEOUT,
    DHT_ERROR_CHECKSUM,
    DHT_ERROR_TOO_QUICK
} DHT22_ERROR;

class DHT22 {
private:
    time_t  _lastReadTime;
    PinName _data;
    float   _lastHumidity;
    float   _lastTemperature;
public:
    DHT22(PinName Data);
    ~DHT22();
    DHT22_ERROR readData(void);
    float getHumidity();
    float getTemperatureC();
    void clockReset();
};

#endif /*_DHT22_H_*/
