/*
 * Editor: Keiane Balicanta
 * Course (Section): Embedded Systems (Section 3)
 * Assignment: Lab 3 - Part 3
 * Date: 30 APR 2023
 * Version: 1.0
 * Description: This is part 3 of lab 3 which changes the blinking speed of an LED based on the length of time a button is pressed
 */

#include <msp430.h>

unsigned char high = 0x01;
unsigned char low = 0x00;
unsigned int count;
unsigned char counting = 0x00;

void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();
    count = 1000;
    counting = 0x00;

    // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);
    __enable_interrupt();

}

// ======= GPIO INIT ======= //
void gpioInit() {
    P1OUT &= ~BIT0;                         // Clear P6.6 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P6.6 to output direction

    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction

    P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
    P4REN |= BIT1;                          // P2.3 pull-up register enable
    P4IES |= BIT1;                         // P2.3 High --> Low edge
    P4IE |= BIT1;                           // P2.3 interrupt enabled

    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES |= BIT3;                         // P2.3 High --> Low edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled
}

// ======= TIMER INIT ======= //
void timerInit() {
    // Timer B0
    TB0CCTL0 = CCIE;                          // TB0CCR0 interrupt enable
    TB0CCR0 = 1;
    TB0CTL = TBSSEL_1 | MC_2 | ID_3;          // ACLK, continuous mode, divide clock by 8

    // Timer B1
    TB1CCTL0 = CCIE;                          // TB1CCR0 interrupt enable
    TB1CCR0 = 1000;
    TB1CTL = TBSSEL_1 | MC_2 | ID_3;          // ACLK, continuous mode, divide clock by 8
}


// ======= ISR ======= //

// Timer B0
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void) {
    if(counting) {              // If Pin 2.3 (button) is low, increment count by 1
        count++;
    }
    else {                      // If Pin 2.3 (button) is high, count is equal to count
        count = count;
    }
    TB0CCR0 += 1;               // Add Offset to TB0CCR0
}

// Timer B1
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
        P1OUT ^= BIT0;                      // Toggle red LED
        TB1CCR0 += count;                   // Change on/off speed ofLED
}

// Port 2
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;           // Clear P2.3 IFG

    if (high)                 // If button held down
    {
        high = 0x00;
        low = 0x01;
        P2IES &= ~BIT3;             // P2.3 Low -> High edge
        P6OUT |= BIT6;              // Set green LED
        P1OUT &= ~BIT0;             // Clear red LED
        counting = 0x01;
        count = 0;
    }

    else if (low)                   // If button released
    {
        high = 0x01;
        low = 0x00;
        P2IES |= BIT3;              // P2.3 High -> Low
        P6OUT &= ~BIT6;             // Clear green LED
        counting = 0x00;
    }
}

// Port 4
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                       // Clear P4.1 IFG
    count = 1000;                        // Set led blinking speed
    counting = 0x00;
}
