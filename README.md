# mbed-hx711

Read HX711 load cell amplifier data.

## Usage

Add the library to your project using mbed CLI (or directly through online IDE):

```# mbed add https://github.com/atoy40/mbed-hx711 HX711```

## Example

```cpp
#include "mbed.h"
#include "HX711.h"

HX711 loadcell(D7, D6);

int main(void) {
    while(1) {
        loadcell.powerUp();
        printf("Weight = %.1f\n", loadcell.getUnits(10));
        loadcell.powerDown();

        wait(3);
    }
}
```