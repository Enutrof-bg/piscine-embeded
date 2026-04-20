#include "main.h"

#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

// UBRR (USART BAUD RATE USAGE)
// UBRR = (F_CPU / (8 * BAUD)) - 1
// UBRR = ((16.000.000 / (8 * 115200))) -1 ~= 16.36 = 16

// Error% = ((Real Speed / Target Speed) - 1) * 100
// Error% = ((F_CPU / (8 * (UBRR+1) * BAUD_RATE) - 1) * 100
// Error% = ((16000000 / (8 * (16+1) * 115200) -1) * 100 ~= 2.12%

void uart_printstr(const char *str);

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


}

void uart_tx(unsigned char data) {

	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}


	
	UDR0 = data;
	//Put data into buffer, send this data
	//DATASHEET PAGE 200 SECTION 20.11


	// while (!(UCSR0A & (1 << TXC0))) 
	// {
	// }


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
/*	 
	Bit 7 – RXCn: USART Receive Complete
	This flag bit is set when there are unread data in the receive buffer and cleared when the receive buffer is empty
	(i.e., does not contain any unread data). If the Receiver is disabled, the receive buffer will be flushed and
	consequently the RXCn bit will become zero. The RXCn Flag can be used to generate a Receive Complete
	interrupt (see description of the RXCIEn bit
*/
	return UDR0;
}


int main() {
	char c;

	uart_init(MYUBRR);

	while (1) 
	{
		c = uart_rx();
		if (c == '\r')
			uart_printstr("\r\n");
		else
			uart_tx(c);
	}
}

// UCRSnA (Control and Status Register)

	/*
	DATASHEET PAGE 201
		Bit 1 – U2Xn: Double the USART Transmission Speed
		This bit only has effect for the asynchronous operation. Write this bit to zero when using synchronous operation.
		Writing this bit to one will reduce the divisor of the baud rate divider from 16 to 8 effectively doubling the transfer
		rate for asynchronous communication.
	*/


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

	/*
	DATASHEET PAGE 203
		• Bit 2:1 – UCSZn1:0: Character Size
		The UCSZn1:0 bits combined with the UCSZn2 bit in UCSRnB sets the number of data bits (Character SiZe) in
		a frame the Receiver and Transmitter use
	*/

	/*
	DATASHEET PAGE 200
		• Bit 5 – UDREn: USART Data Register Empty
		The UDREn Flag indicates if the transmit buffer (UDRn) is ready to receive new data. If UDREn is one, the
		buffer is empty, and therefore ready to be written. The UDREn Flag can generate a Data Register Empty
		interrupt (see description of the UDRIEn bit). UDREn is set after a reset to indicate that the Transmitter is ready.
	*/

	/*
	• Bit 6 – TXCn: USART Transmit Complete
	This flag bit is set when the entire frame in the Transmit Shift Register has been shifted out and there are no
	new data currently present in the transmit buffer (UDRn). The TXCn Flag bit is automatically cleared when a
	transmit complete interrupt is executed, or it can be cleared by writing a one to its bit location. The TXCn Flag
	can generate a Transmit Complete interrupt (see description of the TXCIEn bit).
	*/