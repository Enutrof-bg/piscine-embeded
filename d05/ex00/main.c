#include "main.h"

//DATASHEET PAGE 66 
// __attribute__((signal))
// void PCINT2_vect() {
// 	PORTB ^= (1 << PB0);
// }


// void ft_init(void) {
// 	DDRD &= ~(1 << PD2);
// 	DDRB |= (1 << PB0);

// 	// DATASHEET PAGE 83 SECTION 13.2.6 // *note.1
// 	PCMSK2 |= (1 << PCINT18);

// 	// DATASHEET PAGE 82  SECTION 13.2.4 // *note.2
// 	PCICR |= (1 << PCIE2);

// 	SREG |= (1 << SREG_I);
// }

// __attribute__((signal))
// void INT1_vect() {
// 	PORTB ^= (1 << PB0);

// 	EIFR &= ~(1 << INTF0);
// 	_delay_ms(50);
// 	EIFR |= (1 << INTF0);
// }

__attribute__((signal))
void TIMER1_COMPA_vect() {
	EIFR |= (1 << INTF0);
	EIMSK |= (1 << INT0);
	// EIFR |= (1 << INTF0);
}


//DATASHEET PAGE 74
__attribute__((signal))
void INT0_vect() {
	EIMSK &= ~(1 << INT0);

	PORTB ^= (1 << PB0);

	
}

void ft_init(void) {
	DDRD &= ~(1 << PD2);
	DDRB |= (1 << PB0);

	//DATASHEET PAGE 81 SECTION 13.2.2 //*note2.1
	EIMSK |= (1 << INT0);

	//DATASHEET PAGE 80 SECTION 13.2.1 //*note2.2
	EICRA |= (1 << ISC01) | (1 << ISC00);

	SREG |= (1 << SREG_I);
}


void ft_set_timer1() {

	OCR1A = (F_CPU / 1024UL / 5UL) - 1; //1SEC POUR QUE TCNT1 REACH OCR1A
	TCNT1 = 0;

	// PAGE 142 SECTION 16.11.2
	TCCR1B |= (1 << WGM12); //MODE 4 CTC

	TCCR1B |= (1 << CS12) | (1 << CS10); //PRESCALER 1024

	TIMSK1 |= (1 << OCIE1A);

	/*
	DATASHEET PAGE 144
	 Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally enabled), the
	Timer/Counter1 Output Compare A Match interrupt is enabled. The corresponding Interrupt Vector (see
	“Interrupts” on page 66) is executed when the OCF1A Flag, located in TIFR1, is set
	*/
	SREG |= (1 << SREG_I); //DATASHEET PAGE 20 SECTION 7.3.1
}

int main() {
	
	ft_init();
	ft_set_timer1();

	while(1)
	{
		_delay_ms(10);
	}

}

// note2.1
// EIMSK
// DATASHEET PAGE 81 SECTION 13.2.2
/*
	• Bit 0 – INT0: External Interrupt Request 0 Enable
	When the INT0 bit is set (one) and the I-bit in the Status Register (SREG) is set (one), the external pin interrupt
	is enabled. The Interrupt Sense Control0 bits 1/0 (ISC01 and ISC00) in the External Interrupt Control Register A
	(EICRA) define whether the external interrupt is activated on rising and/or falling edge of the INT0 pin or level
	sensed. Activity on the pin will cause an interrupt request even if INT0 is configured as an output. The
	corresponding interrupt of External Interrupt Request 0 is executed from the INT0 Interrupt Vector.
*/

//note 2.2
// EICRA
// DATASHEET PAGE 80 SECTION 13.2.1
/*
	• Bit 1, 0 – ISC01, ISC00: Interrupt Sense Control 0 Bit 1 and Bit 0
	The External Interrupt 0 is activated by the external pin INT0 if the SREG I-flag and the corresponding interrupt
	mask are set. The level and edges on the external INT0 pin that activate the interrupt are defined in Table 13-2.
	The value on the INT0 pin is sampled before detecting edges. If edge or toggle interrupt is selected, pulses that
	last longer than one clock period will generate an interrupt. Shorter pulses are not ensured to generate an
	interrupt. If low level interrupt is selected, the low level must be held until the completion of the currently
	executing instruction to generate an interrupt
*/




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


// note.3
// TIMSK0
// DATASHEET PAGE 118 SECTION 15.9.6
/*
	• Bit 1 – OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable
	When the OCIE0A bit is written to one, and the I-bit in the Status Register is set, the Timer/Counter0 Compare
	Match A interrupt is enabled. The corresponding interrupt is executed if a Compare Match in Timer/Counter0
	occurs, i.e., when the OCF0A bit is set in the Timer/Counter 0 Interrupt Flag Register – TIFR0
*/

//note.4
// TIFR0
//DATASHEET PAGE 118 SECTION 15.9.7 
/*
	• Bit 1 – OCF0A: Timer/Counter 0 Output Compare A Match Flag
	The OCF0A bit is set when a Compare Match occurs between the Timer/Counter0 and the data in OCR0A –
	Output Compare Register0. OCF0A is cleared by hardware when executing the corresponding interrupt
	handling vector. Alternatively, OCF0A is cleared by writing a logic one to the flag. When the I-bit in SREG,
	OCIE0A (Timer/Counter0 Compare Match Interrupt Enable), and OCF0A are set, the Timer/Counter0 Compare
	Match Interrupt is executed.
*/