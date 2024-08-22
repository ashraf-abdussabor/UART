/*
 * ashraf_mohamed.c
 *	receiving and sending character from PC to microcontroller
 *  Created on: Aug 22, 2024
 *      Author: Ashraf mohamed abdelsabor
 */

#include "tm4c123gh6pm.h"


void _exit(int status) {
    while (1);  // Infinite loop to halt execution
}


void UART0_Init(void);
void UART0_Handler(void);
void UART0_TransmitChar(char data);
char UART0_ReceiveChar(void);

int main(void) {
    UART0_Init();  // Initialize UART0

    while(1) {
        // The main loop does nothing, everything is handled in the UART interrupt
    }
}

void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;   // Enable clock for UART0
    SYSCTL_RCGCGPIO_R |= 0x01;   // Enable clock for PORTA

    // UART0 initialization
    UART0_CTL_R &= ~0x01;        // Disable UART0
    UART0_IBRD_R = 104;          // Set integer part of baud rate (assuming 16MHz clock, 9600 baud)
    UART0_FBRD_R = 11;           // Set fractional part of baud rate
    UART0_LCRH_R = 0x70;         // 8-bit, no parity, 1-stop bit, FIFOs enabled
    UART0_CTL_R |= 0x301;        // Enable UART0, TX, RX

    // Configure PA0 and PA1 for UART0
    GPIO_PORTA_AFSEL_R |= 0x03;  // Enable alt function on PA0, PA1
    GPIO_PORTA_PCTL_R |= 0x11;   // Configure PA0, PA1 as UART
    GPIO_PORTA_DEN_R |= 0x03;    // Enable digital on PA0, PA1

    // Enable UART0 interrupt
    UART0_IM_R |= 0x10;          // Enable RX interrupt
    NVIC_EN0_R |= 0x20;          // Enable interrupt 5 in NVIC (UART0)
}

void UART0_Handler(void) {
    char receivedChar = UART0_ReceiveChar();
    char transmitChar;

    if(receivedChar >= 'a' && receivedChar <= 'z') {
        transmitChar = receivedChar - ('a' - 'A');  // Convert to uppercase
    } else {
        transmitChar = receivedChar + 1;            // Send next character
    }

    UART0_TransmitChar(transmitChar);
}

char UART0_ReceiveChar(void) {
    while((UART0_FR_R & 0x10) != 0); // Wait until the RX buffer is not empty
    return UART0_DR_R & 0xFF;        // Read received character
}

void UART0_TransmitChar(char data) {
    while((UART0_FR_R & 0x20) != 0); // Wait until the TX buffer is not full
    UART0_DR_R = data;               // Transmit character
}



