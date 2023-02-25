/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Russell Trafford
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

/*
 * Editor: Keiane Balicanta
 * Course (Section): Embedded Systems (Section 3)
 * Assignment: Lab 3 - Part 1
 * Date: 22 FEB 2023
 * Version: 1.0
 * Description: This is part 2 of the lab 3 changes the speed blinking LED based on an ISR button press.
 */

#include <msp430.h>

void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}


void gpioInit(){
    // @TODO Initialize the Green LED
        P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
        P6DIR |= BIT6;                          // Set P6.6 to output direction

    // @TODO Initialize Button 2.3
        P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
        P2REN |= BIT3;                          // P2.3 pull-up register enable
        P2IES |= BIT3;                         // P2.3 High --> Low edge
        P2IE |= BIT3;                           // P2.3 interrupt enabled

}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB1CTL = TBSSEL_1 | MC_1;      // SMCLK, continuous mode, clear TBR, enable interrupt
    TB1CCTL0 |= CCIE;                             // Enable TB1 CCR0 Interrupt
    TB1CCR0 = 50000;                          // Set CCR1 to the value to set the duty cycle
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
        P2IFG &= ~BIT3;

    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.
        // Mode 1 is 50000 counts
        // Mode 2 is 35000 counts
        // Mode 3 is 20000 counts
        // Reset to 50000
        if (P2IES & BIT3){
            if(TB1CCR0 <= 20000){
                TB1CCR0 = 50000;
            }
            else {
                TB1CCR0 -= 15000;
            }
        }
}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
        P6OUT ^= BIT6;
}


