# LPC214x GPIO Utility Library

Welcome to the **LPC214x GPIO Helper Functions** — a lightweight, efficient, and clean abstraction to control GPIO pins and ports on the **LPC214x** (ARM7TDMI) microcontroller series using Embedded C.

---

## Features

✅ Easy-to-use `pinWrite`, `pinRead`, `portWrite`, and `portRead` functions

✅ **NEW:** `pinSelect` function for configuring pin special functions

✅ Pure Embedded C, compatible with older compilers

✅ Direct manipulation of IODIR, IOSET, IOCLR, IOPIN, and PINSEL registers

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

## ⚠️ Notes

* Bit shifts are exclusively left shift operations to avoid confusion.
* All function logic is designed to work within Embedded C (pre-C99).
* `IODIR` registers are used to set pin direction on-the-fly.
* `pinSelect` function automatically clears existing bits before setting new function.

---

## 📁 File Structure

```plaintext
├── gpio_utils.c    // Implementation of all helper functions
├── gpio_utils.h    // Header file with function prototypes
├── main.c          // Example usage
├── README.md       // This file
```

---

##