#include "main.h"

void ft_rgb_led(uint8_t index);

void init_rgb();
void set_rgb(uint8_t r, uint8_t g, uint8_t b);
void wheel(uint8_t pos);

void TIMER1_COMPA_vect() {
	static uint8_t index = 1;
	wheel(index);
	index++;
	if (index == 256)
		index = 0;
	SREG |= (1 << SREG_I); //DATASHEET PAGE 20 SECTION 7.3.1
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
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


void setup_timer() {

	OCR1A = (F_CPU / 256 / 256) - 1; //1SEC POUR QUE TCNT1 REACH OCR1A
	TCNT1 = 0;

	TCCR1B |= (1 << WGM12); //MODE 4 CTC

	TCCR1B |= (1 << CS12); //PRESCALER 256

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


void ft_init_led(void) {
	DDRD |= (1 << DDD3); //blue
	DDRD |= (1 << DDD5); //red
	DDRD |= (1 << DDD6); //green	
}

int main() {
	ft_init_led();
	init_rgb();
	setup_timer();

	while(1)
	{
	}

}

// OC0A  
// OC0B 
// OC2B
