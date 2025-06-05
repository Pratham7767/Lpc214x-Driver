# LPC214x GPIO Utility Library

Welcome to the **LPC214x GPIO Helper Functions** — a lightweight, efficient, and clean abstraction to control GPIO pins and ports on the **LPC214x** (ARM7TDMI) microcontroller series using Embedded C.

---

## Features

✅ Easy-to-use `pinWrite`, `pinRead`, `portWrite`, and `portRead` functions

✅ **NEW:** `pinSelect` function for configuring pin special functions

✅ **NEW:** `digitalWrite` function for DAC analog output control

✅ Pure Embedded C, compatible with older compilers

✅ Direct manipulation of IODIR, IOSET, IOCLR, IOPIN, PINSEL, and DACR registers

✅ Uses simple indexing: 0–31 for Port 0 and 100+offset for Port 1

✅ No use of C99 features like `bool`, ensuring backward compatibility

---

## Pin Mapping Convention

| PORT   | Pin Range    | Usage Convention |
| ------ | ------------ | ---------------- |
| Port 0 | P0.0 – P0.31 | `0` to `31`      |
| Port 1 | P1.0 – P1.31 | `100` to `131`   |

🔸 For example, `pinWrite(115, 1);` refers to `P1.15` (100 + 15).

---

## 🔧 Available Functions

### 1️⃣ `void pinWrite(int pinnum, int pinval)`

Set a specific GPIO pin to HIGH (1) or LOW (0). Automatically configures the pin as output.

```c
pinWrite(0, 1);      // Set P0.0 HIGH
pinWrite(100 + 16, 0); // Set P1.16 LOW
```

---

### 2️⃣ `int pinRead(int pinnum)`

Reads the logic level of a pin. Automatically configures the pin as input.

```c
int val = pinRead(5);      // Read value at P0.5
int val2 = pinRead(116);   // Read value at P1.16
```

---

### 3️⃣ `void portWrite(int portnum, unsigned int portval)`

Writes a byte (8 bits) of data to a group of GPIO pins based on the port number.

| portnum | Affected Pins |
| ------- | ------------- |
| 0       | P0.0 – P0.7   |
| 1       | P0.8 – P0.15  |
| 2       | P0.16 – P0.23 |
| 3       | P0.24 – P0.31 |
| 12      | P1.16 – P1.23 |
| 13      | P1.24 – P1.31 |

```c
portWrite(0, 0xAA);  // Write 10101010 to P0.0 – P0.7
portWrite(12, 0x0F); // Write 00001111 to P1.16 – P1.23
```

---

### 4️⃣ `unsigned int portRead(int portnum)`

Reads a group of 8 pins as a byte (0x00–0xFF). Automatically sets pins as input.

| portnum | Read Pins     |
| ------- | ------------- |
| 0       | P0.0 – P0.7   |
| 1       | P0.8 – P0.15  |
| 2       | P0.16 – P0.23 |
| 3       | P0.24 – P0.31 |
| 9       | Full Port 0   |
| 12      | P1.16 – P1.23 |
| 13      | P1.24 – P1.31 |
| 19      | Full Port 1   |

```c
unsigned int data = portRead(1);   // Read P0.8 – P0.15
unsigned int p1_full = portRead(19); // Read all of Port 1
```

---

### 5️⃣ `void pinSelect(int pinnum, int splFunction)`

Configures the special function of a pin by setting the appropriate PINSEL register bits. This allows pins to be used for alternate functions like UART, SPI, PWM, etc.

| splFunction | Pin Function    |
| ----------- | --------------- |
| 0           | GPIO (default)  |
| 1           | Special Func 1  |
| 2           | Special Func 2  |
| 3           | Special Func 3  |

**Supported Pin Ranges:**
- P0.0 – P0.15 (pins 0-15) → Uses PINSEL0 register
- P0.16 – P0.31 (pins 16-31) → Uses PINSEL1 register  
- P1.16 – P1.31 (pins 116-131) → Uses PINSEL2 register

```c
pinSelect(0, 1);    // Configure P0.0 for special function 1
pinSelect(16, 2);   // Configure P0.16 for special function 2
pinSelect(116, 0);  // Configure P1.16 as GPIO
```

---

### 6️⃣ `void digitalWrite(unsigned int value, unsigned int modesel)`

Controls the **Digital-to-Analog Converter (DAC)** output on pin **P0.25**. The LPC2144 has only **one DAC channel** available at P0.25, which converts digital values to analog voltage output.

**Parameters:**
- `value`: 10-bit digital value (0–1023) to be converted to analog output
- `modesel`: Bias mode selection
  - `0` = Normal mode (BIAS = 0)
  - `1` = High-speed mode (BIAS = 1, faster settling time)

**DAC Output Voltage Formula:**
```
VOUT = (value / 1023) × VREF
```
Where VREF is typically 3.3V on LPC2144.

```c
digitalWrite(512, 0);   // Output ~1.65V in normal mode (50% of 3.3V)
digitalWrite(1023, 1);  // Output ~3.3V in high-speed mode (100% of 3.3V)
digitalWrite(0, 0);     // Output 0V in normal mode
digitalWrite(256, 1);   // Output ~0.825V in high-speed mode (25% of 3.3V)
```

**Key Features:**
- ✅ 10-bit resolution (1024 discrete levels: 0–1023)
- ✅ Only available on **P0.25** (single DAC channel)
- ✅ Automatic range validation (values > 1023 are ignored)
- ✅ Configurable bias mode for speed vs. power optimization

**⚠️ Important Notes:**
- The LPC2144 has **only ONE DAC** located at pin **P0.25**
- Pin P0.25 must be configured for DAC function using `pinSelect(25, 2)` before use
- Values exceeding 1023 will be ignored (function does nothing)
- High-speed mode (BIAS=1) offers faster settling but higher power consumption

---

## ⚠️ General Notes

* Bit shifts are exclusively left shift operations to avoid confusion.
* All function logic is designed to work within Embedded C (pre-C99).
* `IODIR` registers are used to set pin direction on-the-fly.
* `pinSelect` function automatically clears existing bits before setting new function.
* DAC functionality is only available on LPC2144 and similar variants with built-in DAC.

---

## 📁 File Structure

```plaintext
├── headers_include/
│   ├── GPIO.h       // GPIO utility functions header
│   └── lpc214x.h    // LPC214x register definitions and macros
└── README.md        // This documentation file
```

---

## 🚀 Quick Start Example

```c
#include "headers_include/GPIO.h"
#include "headers_include/lpc214x.h"

int main() {
    // Configure P0.25 for DAC function
    pinSelect(25, 2);
    
    // Set P0.0 as output and turn on LED
    pinWrite(0, 1);
    
    // Read switch connected to P0.1
    int switch_state = pinRead(1);
    
    // Write pattern to Port 0 lower byte
    portWrite(0, 0xAA);
    
    // Generate 1.65V analog output
    digitalWrite(512, 0);
    
    return 0;
}
```

---

**Happy Coding with LPC214x! 🎯**