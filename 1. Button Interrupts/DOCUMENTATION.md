#**Lab 3 part1.c**
#### Keiane Balicanta
## Purpose
The purpose of this lab is to interrupt routines to change the color of the on-board LEDs of the MSP430.

## Main Function
* The purpose of the main function clears the interrupt flag on P2.3: `P2IFG &= ~BIT3;`
* The `while(1)` loop toggles the LED respective LEDs based on whether the button is pressed or not. This is determined by the `LED_Color` variable which is configured in the port2 vector in **lines 85-109**. An if-statement is implemented that polls to see if button 2.3 is pressed and if so, it will set the global LED_Color variable to 0 and clear both the edge direction of P2.3 and clear the red LED. If not, it will clear the green LED and change to the opposite edge direction of P2.3.


