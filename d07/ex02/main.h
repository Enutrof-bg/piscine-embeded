#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdlib.h> //dtostrf

#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

#define T_SENSOR_ADDR 0x38

#define STATE_START 0x08
#define MT_SLA_ACK 0x18 //SLA+W has been transmitted; ACK has been received
#define MT_DATA_ACK 0x28 //  Data byte has been transmitted; ACK has been received

#define DIVIDER 1048576UL

//i2c.c
void		i2c_init(void);
void		i2c_start(void);
void 		i2c_stop(void) ;
void 		i2c_write(unsigned char data);
void 		i2c_read(void);
void 		print_hex_value(char c);

void ft_protocole(void);

//timer.c
void 		setup_timer();

//uart.c
void		uart_init(unsigned int ubrr);
void 		uart_printstr(const char *str);
void 		uart_tx(unsigned char data);
char 		uart_rx(void);
uint8_t 	ft_hex(uint8_t val);
void		ft_uart_print_hex(uint8_t c);
void 		ft_uart_print_adc_10bit(uint16_t c);

#endif