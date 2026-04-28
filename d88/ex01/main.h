#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <avr/eeprom.h>


#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

#define MODE_INPUT 1
#define BUFFER_SIZE 6

#define EEPROM_SIZE 1024
#define EEPROM_LINE 16


//eeprom.c
void ft_print_eeprom(uint16_t address);
bool ft_replace_eeprom(uint16_t address, uint8_t value);

//timer.c
void 		setup_timer();

//uart.c
void		uart_init(unsigned int ubrr);
void 		uart_printstr(const char *str);
void 		uart_tx(unsigned char data);
char 		uart_rx(void);
uint8_t 	ft_hex(uint8_t val);
void		ft_uart_print_hex(uint8_t c);
void 		ft_uart_print_adc_10bit(uint32_t c);
void 		ft_uart_print_base(uint32_t c, uint8_t base, uint8_t *byte, uint8_t i);
void 		ft_hexdump_addr(uint32_t c, uint8_t base);
// void 		ft_get_input(char *value, int mode);
uint16_t 	ft_get_input(uint8_t max_digits, bool *is_valid);
#endif