#include "main.h"

void ft_rgb_led(uint8_t index);

void TIMER1_COMPA_vect() {
	static uint8_t index = 1;
	ft_rgb_led(index);
	index++;
	if (index == 7)
		index = 0;
	SREG |= (1 << SREG_I); //DATASHEET PAGE 20 SECTION 7.3.1
}

void ft_rgb_led(uint8_t index) {
	switch (index) {
		case 0:
			PORTD &= ~(1 << PD6);
			PORTD &= ~(1 << PD3);
			PORTD |= (1 << PD5); //red
			break;

		case 1:
			PORTD &= ~(1 << PD5);
			PORTD |= (1 << PD6); // green
			break;

		case 2:
			PORTD &= ~(1 << PD6);
			PORTD |= (1 << PD3); //blue
			break;

		case 3:
			PORTD &= ~(1 << PD3);
			PORTD |= (1 << PD5); //red
			PORTD |= (1 << PD6); //green = yellow
			break;

		case 4:
			PORTD &= ~(1 << PD5);
			PORTD |= (1 << PD3); //blue + green = cyan
			break;

		case 5:
			PORTD &= ~(1 << PD6);
			PORTD |= (1 << PD5); //red + blue = magenta
			break;

		case 6:
			PORTD |= (1 << PD6); //red + green + blue = white
			break;


		default:
			break;
	}
}

void setup_timer() {

	OCR1A = (F_CPU / 1024UL) - 1; //1SEC POUR QUE TCNT1 REACH OCR1A
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

void ft_init_led(void) {
	DDRD |= (1 << DDD3); //blue
	DDRD |= (1 << DDD5); //red
	DDRD |= (1 << DDD6); //green	
}

int main() {
	ft_init_led();
	setup_timer();
	PORTD |= (1 << PD5);
	while(1)
	{
	}

}




// int main() {
// 	DDRD |= (1 << DDD3); //blue
// 	DDRD |= (1 << DDD5); //red
// 	DDRD |= (1 << DDD6); //green

// 	while(1)
// 	{

// 		PORTD |= (1 << PD5); //red

// 		_delay_ms(1000);
// 		PORTD &= ~(1 << PD5);
// 		PORTD |= (1 << PD6); //green

// 		_delay_ms(1000);
// 		PORTD &= ~(1 << PD6);
// 		PORTD |= (1 << PD3); //blue

// 		_delay_ms(1000);
// 		PORTD &= ~(1 << PD3);
// 		PORTD |= (1 << PD5); //red
// 		PORTD |= (1 << PD6); //green = yellow

// 		_delay_ms(1000);
// 		PORTD &= ~(1 << PD5);
// 		PORTD |= (1 << PD3); //blue + green = cyan

// 		_delay_ms(1000);
// 		PORTD &= ~(1 << PD6);
// 		PORTD |= (1 << PD5); //red + blue = magenta

// 		_delay_ms(1000);
// 		PORTD |= (1 << PD6); //red + green + blue = white

// 		_delay_ms(1000);
// 		PORTD &= ~(1 << PD6);
// 		PORTD &= ~(1 << PD3);
// 	}

// }

