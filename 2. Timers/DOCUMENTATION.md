#**Lab 3 part2.c**
#### Keiane Balicanta
## Purpose
The purpose of this lab is to utilize interrupts to change the blinking speed of the MSPs on-board LEDs.

## Main Function
* Here the `main()` function serves to only call the `init` functions.

## GPIO and Timers
The only initilizations here are being used for P6.6 (the green LED) and P2.3 (the on-board button) using bit set and bit clear instructions. The `timerInit()` function utilizes TB1 with the ACLK (32.768 kHz) in continuous mode. Additionally, I gave the `CCR0` value an initial value of 50000. Every time P2.3 is pressed, it is decreased by 15000 until it reaches past 20000 where it is then reset to 50000. Therefore, there are 3 modes being addressed:
`1. 50000 (bright) -> 2. 35000 (middle) -> 3. 20000 (low) -> 1. 50000 (bright)`

Finally, the last few lines of code, **88-94** toggle the green LED so that when the timer overflows past 50000, it goes to the opposite state it was in. For example, if it was on, it would turn off and vice versa.