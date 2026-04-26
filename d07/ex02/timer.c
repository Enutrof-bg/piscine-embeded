#include "main.h"


__attribute__((signal))
void TIMER1_COMPA_vect() {

}

/*
Moreover, when the measurement frequency is too
high, the temperature of the sensor itself will rise,
which will affect the measurement accuracy. In order
to make its temperature rise below 0.1,
the activation time of AHT20 should not exceed 10% of
the measurement time -
it is recommended to measure data every 2 seconds
*/

void setup_timer() {

	OCR1A = (F_CPU / 1024UL * 2UL) - 1; //2SEC POUR QUE TCNT1 REACH OCR1A
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
