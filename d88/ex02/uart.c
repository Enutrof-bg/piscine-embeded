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

void uart_putnbr(int16_t n) {
    if (n == 0) {
        uart_tx('0');
        return;
    }

    if (n < 0) {
        uart_tx('-');
        if (n == -32768) {
            uart_printstr("32768");
            return;
        }
        n = -n;
    }

    char buffer[6];
    int8_t i = 0;

    while (n > 0) {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }

    while (--i >= 0) {
        uart_tx(buffer[i]);
    }
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

// oui
int8_t ft_get_cmd_code(uint8_t *buff) {
	if (ft_compare(buff, "STATUS"))
		return CMD_STATUS;
	else if (ft_compare(buff, "SET_ID"))
		return CMD_SET_ID;
	else if (ft_compare(buff, "SET_PRIO"))
		return CMD_SET_PRIO;
	else if (ft_compare(buff, "SET_TAG"))
		return CMD_SET_TAG;
	else if (ft_compare(buff, "FACTORY_RESET"))
		return CMD_FACTORY_RESET;
	return CMD_NOT_KNOWN;
}

int8_t ft_get_cmd() {
	uint8_t buff[CMD_BUFFER];
	uint8_t input = 0;
	uint8_t index = 0;

	while (1)
	{
		input = uart_rx();
		//read input
		if (index > CMD_BUFFER)
			break;
		else if ((input == '\b' || input == 0x7F) && index > 0) {
			uart_printstr("\b \b");

			buff[index] = '\0';
			if (index > 0) {
				index--;
			}
			continue;
		} else if (input == '\b' || input == 0x7F) {
			continue; 
		}
		else if (input == '\r' || input == ' ') {
				uart_tx(' ');
			if (index > 0)
				break;
			else
				continue;
		}
		else {
			if (input >= 'a' && input <= 'z')
				input -= 32;
			buff[index] = input;
			index++;
			buff[index] = '\0';
		}

		//write to screen
		uart_tx(input);
	}
	return ft_get_cmd_code(buff);
}

uint32_t ft_get_id() {
	uint16_t input = 0;
	uint8_t digits = 0;
	uint32_t total = 0;
	uint8_t input_converted = 0;
	const uint32_t max_value = 4294967295UL;

	while (1)
	{
		input = uart_rx();

		//read input
		if ((input == '\b' || input == 0x7F) && digits > 0) {
			uart_printstr("\b \b");

			if (digits > 0) {
				digits--;
				total = total / 10;
			}
			continue;
		}
		else if (input == '\b' || input == 0x7F) {
			continue; 
		}
		else if (input == '\r' || input == ' ') {
				uart_tx(' ');
			if (digits > 0)
				break;
			else
				continue;
		}
		else {
			if (ft_check_dec(input) == false)
				continue;

			input_converted = (uint8_t)ft_get_value(input);
			if (digits < 10 && total <= (max_value - input_converted) / 10) {
				total = total * 10 + input_converted;
				digits++;
				uart_tx(input);
			}
		}
	}

	return total;
}

int16_t ft_get_prio() {
	uint16_t input = 0;
	uint8_t digits = 0;
	int32_t total = 0;
	uint8_t input_converted = 0;
	int8_t signe = 1;

	while (1)
	{
		input = uart_rx();

		//read input
		if ((input == '\b' || input == 0x7F) && (digits > 0 || signe < 0)) {
			uart_printstr("\b \b");

			if (digits > 0) {
				digits--;
				total = total / 10;
			} else {
				signe = 1;
			}
			continue;
		}
		else if (input == '\b' || input == 0x7F) {
			continue; 
		}
		else if (input == '\r' || input == ' ') {
				uart_tx(' ');
			if (digits > 0)
				break;
			else
				continue;
		}
		else {
			if (input == '-' && digits == 0 && signe > 0) {
				signe = -1;
				uart_tx(input);
				continue;
			}
			if (ft_check_dec(input) == false)
				continue;

			input_converted = (uint8_t)ft_get_value(input);

			int32_t limit;
			if (signe < 0)
				limit = 32768;
			 else
				limit = 32767;
			if (total <= (limit - input_converted) / 10) {
				total = total * 10 + input_converted;
				digits++;
				uart_tx(input);
			}
		}
	}

	return (int16_t)(total * signe);
}

void ft_get_tag(uint8_t* data) {

	uint16_t input = 0;
	uint16_t index = 0;

	while (1)
	{
		input = uart_rx();

		//read input
		if ((input == '\b' || input == 0x7F) && index > 0) {
			uart_printstr("\b \b");

			if (index > 0) {
				index--;
				data[index] = '\0';
			}
			continue;
		}
		else if (input == '\b' || input == 0x7F) {
			continue; 
		}
		else if (input == '\r' || input == ' ') {
				uart_tx(' ');
			if (index > 0)
				break;
			else
				continue;
		}
		else {
			if (ft_check_tag(input) == false)
				continue;
			if (index >= (TAG_SIZE - 1))
				continue;

			data[index] = input;
			index++;
			data[index] = '\0';

			uart_tx(input);
		}
	}
}