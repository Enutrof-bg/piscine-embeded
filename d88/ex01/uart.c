#include "main.h"

void uart_init(unsigned int ubrr) {

	//set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)(ubrr);
		// UBRRO : 16 bits register
			// >> 8 to keep high bit and store on UBBROHigh
			// keep low bit on UBBROLow


	UCSR0A |= (1 << U2X0);
	// DATASHEET PAGE 200
	//mode double speed


	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	//enable receiver transmitter
		// RXENO : Receiver enable : Listen on pin RX
		// TXENO : Transmitter enable : Speak on pin RX


	// 115200 8N1
	// set frame format 8data, 1 stop bit
	// UCSR0C = (1 << USBS0) | (3 << UCSZ00);
	// UCSR0C = (3 << UCSZ00);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
		// USBS0  : Stop bit select : set 2 bit for stop
		// UCSZ00 : 3 == 11 (base2)
			// UCSZ01 and UCSZ00 set to 1
			// set char size to 8
	/*
	DATASHEET PAGE 203
		• Bit 2:1 – UCSZn1:0: Character Size
		The UCSZn1:0 bits combined with the UCSZn2 bit in UCSRnB sets the number of data bits (Character SiZe) in
		a frame the Receiver and Transmitter use
	*/

}

void uart_tx(unsigned char data) {

	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}

	
	UDR0 = data;
	//Put data into buffer, send this data
	//DATASHEET PAGE 200 SECTION 20.11


}

void uart_printstr(const char *str) {

	while (*str != '\0')
	{
		uart_tx(*str);
		str++;
	}

}

char uart_rx(void) {

	while ( !(UCSR0A & (1 << RXC0)))
	{
	}
	return UDR0;
}


uint8_t ft_hex(uint8_t val) {
	char hex[] = "0123456789ABCDEF";

	return hex[val];
}

void ft_uart_print_hex(uint8_t c) {
	uart_tx(ft_hex(c / 16));
	uart_tx(ft_hex(c % 16));
}

void ft_uart_print_adc_10bit(uint32_t c) {
	if (c == 0)
		uart_tx('0');
	if (c >= 10) {
		ft_uart_print_adc_10bit(c / 10);
	}
	uart_tx('0' + (c % 10));
}

void ft_uart_print_base(uint32_t c, uint8_t base, uint8_t *byte, uint8_t i) {

	if (c >= base) {
		ft_uart_print_base(c / base, base, byte, i - 1);
	}
	byte[i] = ft_hex(c % 16);
}

void ft_hexdump_addr(uint32_t c, uint8_t base) {
	static uint8_t byte[7] = {'0', '0', '0', '0', '0', '0', '0'};

	ft_uart_print_base(c, base, byte, 6);

	for (uint8_t i = 0; i < 7; i++){
		uart_tx(byte[i]);
	}

}


bool ft_compare_login(char *input, char *database) {
	while (*input != '\0' && *database != '\0') {
		if (*input != *database) {
			return false;
		}
		input++;
		database++;
	}

	return (*input == '\0' && *database == '\0');
}

// void ft_get_input(char *value, int mode) {
// 	char input = 0;
// 	int index = 0;

// 	while ((input = uart_rx()) != '\r' && index < BUFFER_SIZE)
// 	{
// 		//read input
// 		if ((input == '\b' || input == 0x7F) && index > 0) {
// 			uart_printstr("\b \b");

// 			value[index] = '\0';
// 			if (index > 0) {
// 				index--;
// 			}
// 			continue;
// 		} else if (input == '\b' || input == 0x7F) {
// 			continue; 
// 		}
// 		else {
// 			if (input >= 'A' && input <= 'Z')
// 				input += 32;
// 			value[index] = input;
// 			index++;
// 			value[index] = '\0';
// 		}


// 		//write to screen
// 		if (mode == MODE_INPUT)
// 			uart_tx(input);
// 		else
// 			uart_tx('*');
// 	}
// 	uart_printstr("\r\n");
// }

bool ft_find(char c, char *str1) {
	while (*str1 != '\0') {
		if (c == *str1) {
			return (true);
		}
		str1++;
	}
	return (false);
}

bool ft_check_hex(char c) {
	char hex[] = "0123456789abcdefABCDEF";
	if (ft_find(c, hex) == false)
		return false;
	return true;
}

uint16_t ft_get_value(char c) {
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	return 0;
}

uint16_t ft_get_input(uint8_t max_digits, bool *is_valid) {
	uint16_t input = 0;
	uint16_t index = 0;
	uint16_t total = 0;
	uint16_t input_converted = 0;
	bool valid = true;

	while (1)
	{
		input = uart_rx();

		//read input
		if ((input == '\b' || input == 0x7F) && index > 0) {
			uart_printstr("\b \b");

			index--;
			total >>= 4;
			continue;
		}
		else if (input == '\b' || input == 0x7F) {
			continue; 
		}
		else if (input == '\r' || input == ' ') {
			uart_tx(' ');
			if (index > 0)
				break;
			valid = false;
			continue;
		}
		else {
			if (ft_check_hex(input) == false) {
				valid = false;
				continue;
			}
			if (index >= max_digits) {
				valid = false;
				continue;
			}

			index++;

			input_converted = ft_get_value(input);
			total = (total << 4) | input_converted;

			uart_tx(input);
		}
	}

	if (is_valid != 0)
		*is_valid = valid;
	return total;
}