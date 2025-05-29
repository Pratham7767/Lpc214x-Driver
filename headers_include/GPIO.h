#include "lpc214x.h"

// Write to a single pin
// PORT0: 0–31  |  PORT1: 100–131
void pinWrite(int pinnum, int pinval) {
    if (pinnum >= 0 && pinnum <= 31) {
        IODIR0 |= (1 << pinnum); // Set as output
        if (pinval)
            IOSET0 = (1 << pinnum);
        else
            IOCLR0 = (1 << pinnum);
    } else if (pinnum >= 100 && pinnum <= 131) {
        int pin = pinnum - 100;
        IODIR1 |= (1 << pin); // Set as output
        if (pinval)
            IOSET1 = (1 << pin);
        else
            IOCLR1 = (1 << pin);
    }
}

// Write 8 bits to a group of pins
void PortWrite(int portnum, unsigned int portval) {
    int i;
    int basePin;

    if (portval <= 0xFF) {
        // Set base pin based on port group
        if (portnum == 0) basePin = 0;
        else if (portnum == 1) basePin = 8;
        else if (portnum == 2) basePin = 16;
        else if (portnum == 3) basePin = 24;
        else if (portnum == 12) basePin = 100 + 16;
        else if (portnum == 13) basePin = 100 + 24;
        else return; // Invalid port number

        for (i = 0; i < 8; i++) {
            if (portval & (1 << i)) {
                pinWrite(basePin + i, 1); // Set pin HIGH
            } else {
                pinWrite(basePin + i, 0); // Set pin LOW
            }
        }
    }
}

int pinRead(int pinnum) {
    unsigned int myval;

    if (pinnum >= 0 && pinnum <= 31) {
        IODIR0 &= ~(1 << pinnum);        // Set as input
        myval = IOPIN0;                  // Read from PORT0
        if ((myval & (1 << pinnum)) == 0)
            return 0;
        else
            return 1;
    }

    else if (pinnum >= 100 && pinnum <= 131) {
        int pin = pinnum - 100;
        IODIR1 &= ~(1 << pin);           // Set as input
        myval = IOPIN1;                  // Read from PORT1
        if ((myval & (1 << pin)) == 0)
            return 0;
        else
            return 1;
    }

}

