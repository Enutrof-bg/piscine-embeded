#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

//timer.c
void setup_timer();


//uart.c
void uart_init(unsigned int ubrr);
void uart_printstr(const char *str);
void uart_tx(unsigned char data);
char uart_rx(void);


#endif