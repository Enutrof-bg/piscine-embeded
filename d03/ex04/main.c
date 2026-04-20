#include "main.h"

#define UART_BAUDRATE 115200
#define MYUBRR ((F_CPU/(8UL*UART_BAUDRATE))-1)

// UBRR (USART BAUD RATE USAGE)
// UBRR = (F_CPU / (8 * BAUD)) - 1
// UBRR = ((16.000.000 / (8 * 115200))) -1 ~= 16.36 = 16

// Error% = ((Real Speed / Target Speed) - 1) * 100
// Error% = ((F_CPU / (8 * (UBRR+1) * BAUD_RATE) - 1) * 100
// Error% = ((16000000 / (8 * (16+1) * 115200) -1) * 100 ~= 2.12%

#define LOGIN "kevwang"
#define PASSWORD "1234"
#define BUFFER_SIZE 255
#define MODE_LOGIN 0
#define MODE_PASSWORD 1

void uart_printstr(const char *str);
void uart_tx(unsigned char data);
char uart_rx(void);

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

void ft_iconic(void) {
	uart_printstr("                                        .\r\n");
	uart_printstr("              . .                     -:-             .  .  .\r\n");
	uart_printstr("            .'.:,'.        .  .  .     ' .           . \\ | / .\r\n");
	uart_printstr("            .'.;.`.        ._. ! ._.       \\           .__\\:/__.\r\n");
	uart_printstr("              `,:.'         ._\\!/_.                     .';`.      . ' .\r\n");
	uart_printstr("              ,'              . ! .         ,.,      ..======..        .:.\r\n");
	uart_printstr("             ,                  .          ._!_.     ||::: : | .        ',\r\n");
	uart_printstr("      .====.,                   .            ;  .~.===: : : :|   ..===.\r\n");
	uart_printstr("      |.::'||       .=====.,    ..=======.~,    |\"|: :|::::::|   ||:::|=====|\r\n");
	uart_printstr("   ___| :::|!__.,  |:::::|!_,   |: :: ::|\"|l_l|\"|:: |:;;:::|___!| ::|: : :|\r\n");
	uart_printstr("  |: :|::: |:: |!__|; :: |: |===::: :: :|\"||_||\"| : |: :: :|: : |:: |:::::|\r\n");
	uart_printstr("  |:::| _::|: :|:::|:===:|::|:::|:===F=:|\"!/|\\!\"|::F|:====:|::_:|: :|::__:|\r\n");
	uart_printstr("  !_[]![_]_!_[]![]_!_[__]![]![_]![_][I_]!//_:_\\\\![]I![_][_]!_[_]![]_!_[__]!\r\n");
	uart_printstr("  -----------------------------------\"---''''```---\"-----------------------\r\n");
	uart_printstr("  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |= _ _:_ _ =| _ _ _ _ _ _ _ _ _ _ _ _\r\n");
	uart_printstr("                                      |=    :    =|\r\n");
	uart_printstr("______________________________________L___________J________________________\r\n");
	uart_printstr("--------------------------------------------------------------------------\r\n");
}

// void ft_iconic(void) {
// 	uart_printstr("🎆✺🎆💥🎆🎇💥⋆✴︎˚｡⋆✨🥳🎆°🥂⋆.ೃ🍾࿔*:･🎇🎈🎉⋆˙⟡ ⋆.˚ ⊹🎯₊⟡ ⋆✶⋆.˚💛✨🌟🚀⚡️💫🎇🪔🏵️🧨🎆\r\n");
// }

void ft_cool(void) {
	for (int i = 0; i < 10; i++) {
		PORTB ^= (1 << PB0);
		_delay_ms(50);
		PORTB ^= (1 << PB1);
		_delay_ms(50);
		PORTB ^= (1 << PB2);
		_delay_ms(50);
		PORTB ^= (1 << PB4);
		_delay_ms(50);
	}

	for (int i = 0; i < 10; i++) {
		PORTB ^= (1 << PB4);
		_delay_ms(50);
		PORTB ^= (1 << PB2);
		_delay_ms(50);
		PORTB ^= (1 << PB1);
		_delay_ms(50);
		PORTB ^= (1 << PB0);
		_delay_ms(50);
	}

	for (int i = 0; i < 10; i++) {
		PORTB ^= (1 << PB0);
		_delay_ms(50);
		PORTB ^= (1 << PB4);
		_delay_ms(50);
		PORTB ^= (1 << PB1);
		_delay_ms(50);
		PORTB ^= (1 << PB2);
		_delay_ms(50);
	}

}

void ft_init_led(void) {
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

	PORTB |= (0 << PB0);
	PORTB |= (0 << PB1);
	PORTB |= (0 << PB2);
	PORTB |= (0 << PB4);
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
			value[index] = input;
			index++;
			value[index] = '\0';
		}


		//write to screen
		if (mode == MODE_LOGIN)
			uart_tx(input);
		else
			uart_tx('*');
	}
	uart_printstr("\r\n");
}

void ft_login(void) {
	bool isLoginGood = true;
	bool isPasswordGood = true;
	char value[BUFFER_SIZE];

	uart_printstr("Enter your login:\r\n");
	uart_printstr("		username:");
	
	ft_get_input(value, MODE_LOGIN);
	isLoginGood = ft_compare_login(value, LOGIN);

	uart_printstr("		password:");
	ft_get_input(value, MODE_PASSWORD);
	isPasswordGood = ft_compare_login(value, PASSWORD);

	if (isLoginGood == true && isPasswordGood == true)
	{
		uart_printstr("GOOD LOGIN\r\n");
		
		ft_iconic();
		ft_cool();
	}
	else
	{
		uart_printstr("Bad combination username/password\r\n");
	}
}


int main() {
	uart_init(MYUBRR);
	ft_init_led();
	while (1) 
	{
		ft_login();
	}
}

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

