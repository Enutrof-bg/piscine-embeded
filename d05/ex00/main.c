#include "main.h"

//DATASHEET PAGE 66 
__attribute__((signal))
void PCINT2_vect() {
	PORTB ^= (1 << PB0);
}


void ft_init(void) {
	DDRD &= ~(1 << PD2);
	DDRB |= (1 << PB0);

	// DATASHEET PAGE 83 SECTION 13.2.6 // *note.1
	PCMSK2 |= (1 << PCINT18);

	// // DATASHEET PAGE 82  SECTION 13.2.4 // *note.2
	PCICR |= (1 << PCIE2);

	SREG |= (1 << SREG_I);
}

int main() {
	
	ft_init();


	while(1)
	{
		_delay_ms(10);
	}

}


// note.1
// PCMSK2
// DATASHEET PAGE 83 SECTION 13.2.6 
/*
	• Bit 7:0 – PCINT[23:16]: Pin Change Enable Mask 23...16
	Each PCINT[23:16]-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If
	PCINT[23:16] is set and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O
	pin. If PCINT[23:16] is cleared, pin change interrupt on the corresponding I/O pin is disabled.
*/


// note.2 
// PCICR
// DATASHEET PAGE 82  SECTION 13.2.4
/*
	• Bit 2 – PCIE2: Pin Change Interrupt Enable 2
	When the PCIE2 bit is set (one) and the I-bit in the Status Register (SREG) is set (one), pin change interrupt 2 is
	enabled. Any change on any enabled PCINT[23:16] pin will cause an interrupt. The corresponding interrupt of
	Pin Change Interrupt Request is executed from the PCI2 Interrupt Vector. PCINT[23:16] pins are enabled
	individually by the PCMSK2 Register.
*/