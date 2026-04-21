#include "main.h"

//DATASHEET PAGE 66 

// __attribute__((signal))
// void TIMER0_COMPA_vect() {
// 	static uint8_t index = 1;
// 	int8_t coefficient;
// 	index++;
// 	if (index >= 100)
// 		coefficient = 100 - (index - 100);
// 	else
// 		coefficient = index;
// 	OCR1A = ICR1 * coefficient / 100; 
// 	if (index == 200)
// 		index = 0;
// }

__attribute__((signal))
void TIMER0_COMPA_vect() {
	static uint8_t index = 1;

	static int8_t signe = 1;
	index += signe;
	 
	if (index >= 100)
		signe = -1;
	if (index <= 0)
		signe = 1;
	OCR1A = ICR1 * index / 100; // ~= 24
}

void ft_init(void) {

	DDRB |= (1 << PB1);

	SREG |= (1 << SREG_I);
}

void set_timer1() {

	ICR1 = F_CPU / 1024UL / 61 - 1; // ~= 256
	OCR1A = ICR1 * 1 / 100;

	TCNT1 = 0;
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12); //MODE 14 FAST PMW
	// DATASHEET PAGE 140-141: MODE 14 FAST PMW

	// PRESCALER 256
	TCCR1B |= (1 << CS12) | (1 << CS10); // 1024 tics du cpu vaut 1 tics du timer1
	// CONNECT TIMER A L'HORLOGE DU MICROCONTROLLEUR (SINON 0)
	// DATASHEET PAGE 139
	TCCR1A |= (1 << COM1A1) | (0 << COM1A0);

}

void set_timer0() {

	TCNT0 = 0;

	OCR0A = (F_CPU / 1024UL / 200UL - 1);
	// OCR0A = 77;
	//DATASHEET PAGE 115 TABLE 15-8
	TCCR0A |= (1 << WGM01); //MODE 2 CTC // TOP OCRA


	//DATASHEET PAGE 116-117 TABLE 15-9
	TCCR0B |= (1 << CS02) | (1 << CS00); //prescaler 1024

	//DATASHEET PAGE 118 SECTION 15.9.6 // *note.3
	TIMSK0 |= (1 << OCIE0A);

	//DATASHEET PAGE 118 SECTION 15.9.7 // 
	TIFR0 |= (1 << OCF0A);
}

void ft_init_timer(void) {
	
	set_timer1();
	set_timer0();
}

int main() {
	
	ft_init();
	ft_init_timer();

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

// note.3
// TIMSK0
// DATASHEET PAGE 118 SECTION 15.9.6
/*
	• Bit 1 – OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable
	When the OCIE0A bit is written to one, and the I-bit in the Status Register is set, the Timer/Counter0 Compare
	Match A interrupt is enabled. The corresponding interrupt is executed if a Compare Match in Timer/Counter0
	occurs, i.e., when the OCF0A bit is set in the Timer/Counter 0 Interrupt Flag Register – TIFR0
*/