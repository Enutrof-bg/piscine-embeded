#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdbool.h>

#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

#define MODE_INPUT 1
#define BUFFER_SIZE 6

#define EEPROM_SIZE 1024
#define EEPROM_LINE 16

#define MODE_CHANGE 1
#define MODE_NO_CHANGE 0

#define CMD_STATUS 1
#define CMD_SET_ID 2
#define CMD_SET_PRIO 3
#define CMD_SET_TAG 4
#define CMD_FACTORY_RESET 5
#define CMD_NOT_KNOWN 99
#define CMD_SHOW 9

#define CMD_BUFFER 14

#define MAGIC_NUMBER 0x42
#define SLOT_SIZE 4
#define TAG_SIZE 33

typedef struct s_node {
	uint8_t magic_number; //1
	uint32_t nodeID; //4
	int16_t priority; //2
	uint8_t tag[TAG_SIZE]; //33
	uint16_t integrity; //2 // == 42
} t_node;

//eeprom.c
void 		EEPROM_write(uint16_t uiAddress, uint8_t ucData);
uint8_t 	EEPROM_read(uint16_t uiAddress);
void 		ft_print_eeprom(uint16_t address, uint8_t mode);
bool 		ft_replace_eeprom(uint16_t address, uint8_t value);
bool 		ft_verification_eeprom(uint16_t address, uint8_t value);

//timer.c
void 		setup_timer();


//uart.c
void		uart_init(unsigned int ubrr);
void 		uart_printstr(const char *str);
void 		uart_tx(unsigned char data);
char 		uart_rx(void);
void		ft_uart_print_hex(uint8_t c);
void 		ft_uart_print_adc_10bit(uint32_t c);
void 		ft_uart_print_base(uint32_t c, uint8_t base, uint8_t *byte, uint8_t i);
void 		ft_hexdump_addr(uint32_t c, uint8_t base);
uint16_t 	ft_get_input();
int8_t		ft_get_cmd();
void 		uart_putnbr(int16_t n);

uint32_t	ft_get_id();
int16_t		ft_get_prio(); 
void 		ft_get_tag(uint8_t* data);

//utils.c
bool		ft_compare(uint8_t *s1, uint8_t *s2);
uint8_t		ft_hex(uint8_t val);
bool		ft_find(char c, char *str1);
bool 		ft_check_dec(char c);
bool		ft_check_hex(char c);
bool		ft_check_tag(char c);
uint16_t	ft_get_value(char c);
uint8_t		ft_strlen(char *input);
uint16_t	ft_power(uint16_t nbr, uint8_t power);

#endif