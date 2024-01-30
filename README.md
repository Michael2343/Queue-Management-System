## Queue-Management-System

# Summary
Programmed on AT8051 microprocessor using C, the project features interactive self-developed mini-OS, SIM900 Module with a custom-built interface between the microprocessor and sim900 module (The SMS messages arrive in Hebrew)

# Electrical Diagram
![Electrical Diagram A3-1](https://github.com/Michael2343/Queue-Management-System/assets/100785699/ff20d293-3a6d-4750-a76d-3503b84eb0e3)

#Technical Specification
1. AT89C5131 Microcontroller - The AT89C5131 microcontroller from ATMAIL is based on the Intel 8051 processor, with unique enhancements. The microcontroller includes counters, timers, an interrupt system, serial communication, and more.
2. GSM SIM900 Module - Easily and quickly installed onto the main controller, enabling the sending of SMS messages to any phone number according to a specific protocol (explained later).
3. Keypad - 4x4 keypad with phone digits (1-9, *, #) and letters (A, B, C, D). Connects to socket x18 (8-pin socket).
4. Liquid Crystal Display (LCD) - The LCD component displays messages and data. Unlike LEDs, LCDs do not emit light. Connects to socket 7x2 (14-pin socket).
5. Binary Code to BCD Decoder - This component decodes binary numbers (ranging from 0 to 9) to a specific BCD code, which activates a seven-segment display. This display has seven segments, each working with 8 switches to show the desired numerical value.
6. Four 7-Segment Displays + Enable Unit - Four 7-segment displays work in a sequential manner, with each digit displaying consecutively. An enable unit controls the digits, allowing only one digit to be displayed at a time, creating the illusion that all digits are lit simultaneously.
7.Buttons (Service Stations) - The project includes 2 buttons representing service stations. Pressing one of them advances the queue. Additionally, the next queue number and the user's referral to the selected station are displayed on the screen.
8.Buzzer - In addition to queue advancement, a buzzer sounds periodically as the queue progresses and a station becomes available.
