#include "main.h"

#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

#define BUFFER_SIZE 7
#define MODE_INPUT 1

void ft_rgb_led(uint8_t index);

void init_rgb();
void set_rgb(uint8_t r, uint8_t g, uint8_t b);
void wheel(uint8_t pos);

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void init_rgb() {

	// DATASHEET PAGE 113
	TCNT0 = 0;

	TCCR0A |= (1 << WGM01) | (1 << WGM00); //MODE 3  // TOP 0xFF

	TCCR0B |= (1 << CS02); //prescaler 256

	//red
	OCR0B = (256 * 0.01); // DUTY CYCLE DE PD5 RED
	TCCR0A |= (1 << COM0B1) | (0 << COM0B0); //handle red PD5   // OC0B

	//green
	OCR0A = (255 * 0.01); //DUTY CYCLE DE PD6 GREEN
	TCCR0A |= (1 << COM0A1) | (0 << COM0A0); //handle green PD6  // OC0A


	//DATASHEET PAGE 163
	//blue
	TCNT2 = 0;
	TCCR2A |= (1 << WGM21) | (1 << WGM20); // MODE 3 // TOP:0xFF

	TCCR2B |= (1 << CS22) | (1 << CS21); // PRESCALER 256

	OCR2B = (255 * 0.01);  //DUTY CYCLE DE PD6 GREEN
	TCCR2A |= (1 << COM2B1); // handle blue PD3 // OC2B
}

void ft_init_led(void) {
	DDRD |= (1 << DDD3); //blue
	DDRD |= (1 << DDD5); //red
	DDRD |= (1 << DDD6); //green	
}

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
	/*
	DATASHEET PAGE 201
		Bit 1 – U2Xn: Double the USART Transmission Speed
		This bit only has effect for the asynchronous operation. Write this bit to zero when using synchronous operation.
		Writing this bit to one will reduce the divisor of the baud rate divider from 16 to 8 effectively doubling the transfer
		rate for asynchronous communication.
	*/

	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	//enable receiver transmitter
		// RXENO : Receiver enable : Listen on pin RX
		// TXENO : Transmitter enable : Speak on pin RX
	/*
	DATASHEET PAGE 202
		• Bit 4 – RXENn: Receiver Enable n
		Writing this bit to one enables the USART Receiver. The Receiver will override normal port operation for the
		RxDn pin when enabled. Disabling the Receiver will flush the receive buffer invalidating the FEn, DORn, and
		UPEn Flags.

		• Bit 3 – TXENn: Transmitter Enable n
		Writing this bit to one enables the USART Transmitter. The Transmitter will override normal port operation for
		the TxDn pin when enabled. The disabling of the Transmitter (writing TXENn to zero) will not become effective
		until ongoing and pending transmissions are completed, i.e., when the Transmit Shift Register and Transmit
		Buffer Register do not contain data to be transmitted. When disabled, the Transmitter will no longer override the
		TxDn port.
	*/


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


	UCSR0A |= (1 << RXC0);
	/* 
	DATASHEET PAGE 200
	Bit 7 – RXCn: USART Receive Complete
	This flag bit is set when there are unread data in the receive buffer and cleared when the receive buffer is empty
	(i.e., does not contain any unread data). If the Receiver is disabled, the receive buffer will be flushed and
	consequently the RXCn bit will become zero. The RXCn Flag can be used to generate a Receive Complete
	interrupt (see description of the RXCIEn bit).*/

	// UCSR0B |= (1 << RXCIE0);
	// /* Bit 7 – RXCIEn: RX Complete Interrupt Enable n
	// Writing this bit to one enables interrupt on the RXCn Flag. A USART Receive Complete interrupt will be
	// generated only if the RXCIEn bit is written to one, the Global Interrupt Flag in SREG is written to one and the
	// RXCn bit in UCSRnA is set*/
	// SREG |= (1 << SREG_I);
}

void uart_tx(unsigned char data) {

	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}
	/*
	DATASHEET PAGE 200
		• Bit 5 – UDREn: USART Data Register Empty
		The UDREn Flag indicates if the transmit buffer (UDRn) is ready to receive new data. If UDREn is one, the
		buffer is empty, and therefore ready to be written. The UDREn Flag can generate a Data Register Empty
		interrupt (see description of the UDRIEn bit). UDREn is set after a reset to indicate that the Transmitter is ready.
	*/

	
	UDR0 = data;
	//Put data into buffer, send this data
	//DATASHEET PAGE 200 SECTION 20.11


	// while (!(UCSR0A & (1 << TXC0))) 
	// {
	// }
	/*
	• Bit 6 – TXCn: USART Transmit Complete
	This flag bit is set when the entire frame in the Transmit Shift Register has been shifted out and there are no
	new data currently present in the transmit buffer (UDRn). The TXCn Flag bit is automatically cleared when a
	transmit complete interrupt is executed, or it can be cleared by writing a one to its bit location. The TXCn Flag
	can generate a Transmit Complete interrupt (see description of the TXCIEn bit).
	*/

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

void ft_get_input(char *value, int mode) {
	char input = 0;
	int index = 0;

	while ((input = uart_rx()) != '\r' && index < BUFFER_SIZE)
	{
		//read input
		if ((input == '\b' || input == 0x7F) && index > 0) {
			uart_printstr("\b \b");

			value[index] = '\0';
			if (index > 0) {
				index--;
			}
			continue;
		} else if (input == '\b' || input == 0x7F) {
			continue; 
		}
		else {
			if (input >= 'a' && input <= 'z')
				input -= 32;
			value[index] = input;
			index++;
			value[index] = '\0';
		}


		//write to screen
		if (input >= 'a' && input <= 'z')
			uart_tx(input - 32);
		else
			uart_tx(input);
	}
	uart_printstr("\r\n");
}

bool ft_compare(char c, char *str1) {
	while (*str1 != '\0') {
		if (c == *str1) {
			return (true);
		}
		str1++;
	}
	return (false);
}

bool ft_check_format(char *value) {
	uint8_t index = 1;
	char hex[] = "0123456789ABCDEF";
	
	if (value[0] != '#')
		return false;

	while (value[index] && index < BUFFER_SIZE) {
		if (ft_compare(value[index], hex) == false)
			return false;
		index++;
		
		if (index > 7)
			return false;
	}
	return true;
}

uint8_t ft_get_hexa(char c) {
	char hex[16] = "0123456789ABCDEF";
	uint8_t i = 0;
	while (hex[i])
	{
		if (hex[i] == c)
			break;
		i++;
	}
	return i;
}

void ft_change_rgb(char *rgb) {
	uint8_t r = (ft_get_hexa(rgb[1]) * 16 + ft_get_hexa(rgb[2]));
	uint8_t g = (ft_get_hexa(rgb[3]) * 16 + ft_get_hexa(rgb[4]));
	uint8_t b = (ft_get_hexa(rgb[5]) * 16 + ft_get_hexa(rgb[6]));

	set_rgb(r, g ,b);
}

void ft_rgb(void) {
	char rgb[BUFFER_SIZE];
	bool isRGBformat = true;

	uart_printstr("Enter RGB value (#RRGGBB format):");
	ft_get_input(rgb, MODE_INPUT);

	isRGBformat = ft_check_format(rgb);
	if (isRGBformat) {
		uart_printstr("Nice color choice ! 🌴😎 \r\n");
		uart_printstr(rgb);
		uart_printstr("\r\n");
		ft_change_rgb(rgb);
	}
	else {
		uart_printstr("WRONG FORMAT (#RRGGBB) 😡😡😡\r\n");
	}
}


int main() {
	ft_init_led();
	init_rgb();
	uart_init(MYUBRR);

	while(1)
	{
		ft_rgb();
	}

}
