#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)


//adc.c
void 		ft_adc();
uint8_t 	ft_adc_read();
uint16_t 	ft_adc_read_10bit();

//timer.c
void		setup_timer();


//uart.c
void		uart_init(unsigned int ubrr);
void 		uart_printstr(const char *str);
void 		uart_tx(unsigned char data);
char 		uart_rx(void);
uint8_t 	ft_hex(uint8_t val);
void		ft_uart_print_adc(uint8_t c);
void 		ft_uart_print_adc_10bit(uint16_t c);

#endif