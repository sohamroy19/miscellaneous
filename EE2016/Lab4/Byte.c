#include "LPC23xx.h"

int main() {
    FIO3DIR = 0xFF;
    FIO4DIR = 0x00;

    while (1) {
        int high = (FIO4PIN & 0xF0) >> 4;
        int low = (FIO4PIN & 0x0F);
        FIO3PIN = high * low;
    }

    return 0;
}
