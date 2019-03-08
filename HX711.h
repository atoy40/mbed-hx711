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

#ifndef _HX711_H_
#define _HX711_H_

#include "mbed.h"

/** Read HX711 load cell amplifier data
 * 
 * Example:
 * @code
 * #include "mbed.h"
 * #include "HX711.h"
 *
 * HX711 loadcell(D7, D6);
 *
 * int main(void) {
 *   while(1) {
 *       loadcell.powerUp();
 *       printf("Weight = %.1f\r\n", loadcell.getUnits(10));
 *       loadcell.powerDown();
 *
 *       wait(3);
 *   }
 * }
 * @endcode
 */
class HX711
{
  public:
    HX711(PinName pinData, PinName pinSck, uint8_t gain = 128);
    ~HX711();
    bool isReady();
    int read(void);
    void waitReady(int delay_ms = 0);
    int readAverage(uint8_t times = 10);
    void setOffset(int offset);
    void setScale(float scale);
    float getUnits(uint8_t times = 1);
    void setGain(uint8_t gain);
    void powerDown();
    void powerUp();
    void tare(uint8_t times = 10);

  private:
    PinName _pinData;
    PinName _pinSck;
    int _offset;
    float _scale;
    uint8_t _gain;
};

#endif
