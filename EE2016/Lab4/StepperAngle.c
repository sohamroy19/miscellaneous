#include "LPC23xx.h"

void delay() {
    int i, j;

    for (i = 0; i < 0xFF; i++)
        for (j = 0; j < 0xFF; j++)
            ;
}

int main() {
    IODIR0 = 0xFFFFFFFF;

    int limit = 80;
    for (int i = 0; i < limit; i += 8) {
        IOPIN0 = 0x00000280; // 1
        delay();
        IOPIN0 = 0x00000180; // 2
        delay();
        IOPIN0 = 0x00000140; // 3
        delay();
        IOPIN0 = 0x00000240; // 4
        delay();
    }

    return 0;
}
