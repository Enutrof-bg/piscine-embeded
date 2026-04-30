#include "main.h"

__attribute__((signal))
void SPI_STC_vect() {
	uart_printstr("proutprout2\r\n");	
	PORTB ^= (1 << PB0);
}

void SPI_tx(uint8_t data) {

	SPDR = data;
	SPDR = 0x00;
	SPDR = 0x00;
	SPDR = 0x00;
	SPDR = 0x00;
	SPDR = 0x00;
	SPDR = 0x00;

	uart_printstr("Load data in SPDR\r\n");	
	while(!(SPSR & (1<<SPIF)))
	{
		// uart_printstr("while !(1<<SPIF)...\r\n");
	}
	uart_printstr("Wait finished for SPIF\r\n");
}

void ft_spi_init(void) {

	//DATASHEET PAGE 169, PRSPI must be enable to make uart works properly
	PRR |= (1 << PRSPI);

	DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);
	uart_printstr("Init PORT as output\r\n");

	// DDRB |= (1 << PB0);
	// PORTB |= (1 << PB0);

	//set master, clock rate
	// SPCR |= (1 << SPE) | (1 << MSTR) | (1 < SPR0) | (1 << CPOL) | (1 << CPHA) |(1 << SPIE);
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 < SPR0);
	uart_printstr("Set SPCR register\r\n");

	// SPSR |= (1 << SPIF);
	// SREG |= (1 << SREG_I);

	// DDRB &= ~(1 << PB2);
	SPI_tx(0x55);
	// DDRB |= (1 << PB2);
}