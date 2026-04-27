#include "main.h"


void ft_led(uint8_t c) {
	PORTB = 0;
	if (c > 64)
		PORTB |= (1 << PB0);
	if (c > 128)
		PORTB |= (1 << PB1);
	if (c > 192)
		PORTB |= (1 << PB2);
	if (c >= 255)
		PORTB |= (1 << PB4);
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

	// DATASHEET PAGE 113 SECTION 15.9.1
	TCNT0 = 0;

	//DATASHEET PAGE 115 TABLE 15-8
	TCCR0A |= (1 << WGM01) | (1 << WGM00); //MODE 3 PMW // TOP 0xFF

	//DATASHEET PAGE 116-117 TABLE 15-9
	TCCR0B |= (1 << CS02); //prescaler 256

	//red
		//DATASHEET PAGE 114 TABLE 15-6
	OCR0B = (256 * 0.01); // DUTY CYCLE DE PD5 RED
	TCCR0A |= (1 << COM0B1); //handle red PD5   // OC0B

	//green
		//DATASHEET PAGE 114 TABLE 15-3
	OCR0A = (255 * 0.01); //DUTY CYCLE DE PD6 GREEN
	TCCR0A |= (1 << COM0A1); //handle green PD6  // OC0A


	//DATASHEET PAGE 163 SECTION 18.1.1
	TCNT2 = 0;

	//DATASHEET PAGE 164 TABLE 18-8
	TCCR2A |= (1 << WGM21) | (1 << WGM20); // MODE 3 PMW // TOP:0xFF

	//DATASHEET PAGE 165-166 TABLE 18-9
	TCCR2B |= (1 << CS22) | (1 << CS21); // PRESCALER 256

	//blue
		//DATASHEET PAGE 163 TABLE 18-6
	OCR2B = (255 * 0.01);  //DUTY CYCLE DE PD6 GREEN
	TCCR2A |= (1 << COM2B1); // handle blue PD3 // OC2B
}