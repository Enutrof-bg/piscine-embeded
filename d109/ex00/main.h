#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)


//rgb.c
void		ft_init_led();
void		ft_led(uint8_t c);
void		set_rgb(uint8_t r, uint8_t g, uint8_t b);
void		wheel(uint8_t pos);
void		init_rgb();

//timer.c
void		setup_timer();

//spi.c
void		ft_spi_init();
void 		SPI_tx(uint8_t data);

//uart.c
void		uart_init(unsigned int ubrr);
void		uart_printstr(const char *str);
void		uart_tx(unsigned char data);
char		uart_rx(void);
uint8_t		ft_hex(uint8_t val);
void		ft_uart_print_adc(uint8_t c);
void		uart_putnbr(int16_t n);
void		ft_uart_print_base(uint32_t c, uint8_t base, uint8_t *byte, uint8_t i);
void		ft_hexdump_addr(uint32_t c, uint8_t base);


//utils.c
bool		ft_compare(uint8_t *s1, uint8_t *s2);
uint8_t		ft_hex(uint8_t val);
bool		ft_find(char c, char *str1);
bool		ft_check_dec(char c);
bool		ft_check_hex(char c);
bool		ft_check_tag(char c);
uint16_t	ft_get_value(char c);
uint8_t		ft_strlen(char *input);
uint16_t	ft_power(uint16_t nbr, uint8_t power);



#endif