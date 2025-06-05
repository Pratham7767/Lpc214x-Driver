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
void portWrite(int portnum, unsigned int portval) {
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


unsigned int portRead(int portnum) {
    unsigned int mydata;

    if (portnum == 0) {
        // P0.0–P0.7
        IODIR0 &= ~(0x000000FF);  // Set bits 0–7 as input
        mydata = IOPIN0;
        return (mydata & 0x000000FF);
    }

    else if (portnum == 1) {
        // P0.8–P0.15
        IODIR0 &= ~(0x0000FF00);
        mydata = IOPIN0;
        return ((mydata & 0x0000FF00) >> 8);
    }

    else if (portnum == 2) {
        // P0.16–P0.23
        IODIR0 &= ~(0x00FF0000);
        mydata = IOPIN0;
        return ((mydata & 0x00FF0000) >> 16);
    }

    else if (portnum == 3) {
        // P0.24–P0.31
        IODIR0 &= ~(0xFF000000);
        mydata = IOPIN0;
        return ((mydata & 0xFF000000) >> 24);
    }

    else if (portnum == 9) {
        // Full P0
        IODIR0 = 0x00000000;
        mydata = IOPIN0;
        return mydata;
    }

    else if (portnum == 12) {
        // P1.16–P1.23
        IODIR1 &= ~(0x00FF0000);
        mydata = IOPIN1;
        return ((mydata & 0x00FF0000) >> 16);
    }

    else if (portnum == 13) {
        // P1.24–P1.31
        IODIR1 &= ~(0xFF000000);
        mydata = IOPIN1;
        return ((mydata & 0xFF000000) >> 24);
    }

    else if (portnum == 19) {
        // Full P1
        IODIR1 = 0x00000000;
        mydata = IOPIN1;
        return mydata;
    }


}


void pinSelect(int pinnum, int splFunction) {
    int bit0 = 0, bit1 = 0;

    // Set bit0 and bit1 based on splFunction (0 to 3)
    if (splFunction == 0) {
        bit0 = 0; bit1 = 0;
    }
    else if (splFunction == 1) {
        bit0 = 1; bit1 = 0;
    }
    else if (splFunction == 2) {
        bit0 = 0; bit1 = 1;
    }
    else if (splFunction == 3) {
        bit0 = 1; bit1 = 1;
    }
    else {
        return; // invalid splFunction, exit
    }

    // Port 0.0 to 0.15 → PINSEL0
    if (pinnum >= 0 && pinnum <= 15) {
        int offset = pinnum * 2;

        // Clear both bits first
        PINSEL0 &= ~(1 << offset);
        PINSEL0 &= ~(1 << (offset + 1));

        // Set bits
        if (bit0 == 1) {
            PINSEL0 |= (1 << offset);
        }
        if (bit1 == 1) {
            PINSEL0 |= (1 << (offset + 1));
        }
    }

    // Port 0.16 to 0.31 → PINSEL1
    else if (pinnum >= 16 && pinnum <= 31) {
        int offset = (pinnum - 16) * 2;

        // Clear both bits first
        PINSEL1 &= ~(1 << offset);
        PINSEL1 &= ~(1 << (offset + 1));

        // Set bits
        if (bit0 == 1) {
            PINSEL1 |= (1 << offset);
        }
        if (bit1 == 1) {
            PINSEL1 |= (1 << (offset + 1));
        }
    }

    // Port 1.16 to 1.31 → PINSEL2 (represented as 116 to 131)
    else if (pinnum >= 116 && pinnum <= 131) {
        int actualPin = pinnum - 100;
        int offset = (actualPin) * 2;

        // Clear both bits first
        PINSEL2 &= ~(1 << offset);
        PINSEL2 &= ~(1 << (offset + 1));

        // Set bits
        if (bit0 == 1) {
            PINSEL2 |= (1 << offset);
        }
        if (bit1 == 1) {
            PINSEL2 |= (1 << (offset + 1));
        }
    }

    // Other pins not supported
    else {
        return;
    }
}

void digitalWrite(unsigned int value, unsigned int modesel) {
    if (value <= 1023) {  // No need to check value >= 0 for unsigned
        if (modesel == 1) {
            DACR = (1 << 16) | (value << 6);  // Set BIAS = 1
        } else {
            DACR = (0 << 16) | (value << 6);  // BIAS = 0 (optional, since 0 << 16 = 0)
        }
    }
}
