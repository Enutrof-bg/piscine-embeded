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

void ft_uart_print_adc(uint8_t c) {
	uart_tx(ft_hex(c / 16));
	uart_tx(ft_hex(c % 16));
	uart_printstr("\r\n");
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