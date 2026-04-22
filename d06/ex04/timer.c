#include "main.h"

void setup_timer() {

	OCR1A = (F_CPU / 1024UL / 50UL) - 1; //1SEC POUR QUE TCNT1 REACH OCR1A
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
