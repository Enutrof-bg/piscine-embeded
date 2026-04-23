#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/twi.h>

#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

//i2c.c
void		i2c_init(void);
void		i2c_start(void);
void 		i2c_stop(void) ;
void 		i2c_write(unsigned char data);
void 		i2c_read(void);
void 		print_hex_value(char c);

//timer.c
void 		setup_timer();

//uart.c
void		uart_init(unsigned int ubrr);
void 		uart_printstr(const char *str);
void 		uart_tx(unsigned char data);
char 		uart_rx(void);
uint8_t 	ft_hex(uint8_t val);
void		ft_uart_print_adc(uint8_t c);
void 		ft_uart_print_adc_10bit(uint16_t c);

#endif