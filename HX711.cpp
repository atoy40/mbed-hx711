/*
 *  HX711 Library for HX711 load cell amplifier
 *
 *
 *  Copyright (C) Anthony Hinsinger
 *                Inspired from https://github.com/bogde/HX711
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

#include "HX711.h"
#include "mbed.h"

HX711::HX711(PinName pinData, PinName pinSck, uint8_t gain) : _offset(0), _scale(1.0f)
{
    _pinData = pinData;
    _pinSck = pinSck;

    setGain(gain);

    DigitalOut sck(_pinSck);
    DigitalIn data(_pinData);
}

HX711::~HX711()
{
}

bool HX711::isReady()
{
    DigitalIn data(_pinData);

    return data == 0;
}

int HX711::readAverage(uint8_t times)
{
    int sum = 0;
    for (int i = 0; i < times; i++)
    {
        sum += read();
    }

    return sum / times;
}

int HX711::read()
{
    int buffer;
    buffer = 0;

    DigitalOut sck(_pinSck);
    DigitalIn data(_pinData);

    waitReady();

    // next steps are timing-dependents
    CriticalSectionLock::enable();

    for (uint8_t i = 24; i--;)
    {
        sck = 1;
        buffer = buffer << 1;

        if (data.read())
        {
            buffer++;
        }

        sck = 0;
    }

    // set gain for next read
    for (int i = 0; i < _gain; i++)
    {
        sck = 1;
        sck = 0;
    }

    // reading done
    CriticalSectionLock::disable();

    // set sign
    if (buffer & 0x800000)
    {
        buffer ^= 0xFF000000;
    }

    return buffer;
}

void HX711::waitReady(int delay_ms)
{
    while (!isReady())
    {
        wait_us(delay_ms*1000);
    }
}

bool HX711::waitReadyRetry(int retries, int delay_ms) {
    int count = 0;
        while (count < retries) {
            if (isReady()) {
                return true;
            }
            wait_us(delay_ms*1000);
            count++;
        }
    return false;
}

void HX711::setOffset(int offset)
{
    _offset = offset;
}

void HX711::setScale(float scale)
{
    _scale = scale;
}

float HX711::getUnits(uint8_t times)
{
    double val = readAverage(times) - _offset;
    return val / _scale;
}

void HX711::setGain(uint8_t gain)
{
    switch (gain)
    {
    case 128: // channel A, gain factor 128
        _gain = 1;
        break;

    case 64: // channel A, gain factor 64
        _gain = 3;
        break;

    case 32: // channel B, gain factor 32
        _gain = 2;
        break;
    }
    DigitalOut sck(_pinSck);
    sck = 0;
    read();
}

void HX711::powerDown()
{
    DigitalOut sck(_pinSck);
    sck = 0;
    sck = 1;
}

void HX711::powerUp()
{
    DigitalOut sck(_pinSck);
    sck = 0;
}

void HX711::tare(uint8_t times)
{
    int sum = readAverage(times);
    setOffset(sum);
}
