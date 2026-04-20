#include "main.h"

void ft_display(uint8_t value)
{
	if (value & (1))
		PORTB |= (1 << PB0);
	else
		PORTB &= ~(1 << PB0);

	if (value & (2))
		PORTB |= (1 << PB1);
	else
		PORTB &= ~(1 << PB1);

	if (value & (4))
		PORTB |= (1 << PB2);
	else
		PORTB &= ~(1 << PB2);

	if (value & (8))
		PORTB |= (1 << PB4);
	else
		PORTB &= ~(1 << PB4);
}

int main() {
	uint8_t value = 0;

	DDRB |= (1 << PB0); // set PB as output pin
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	DDRB |= (1 << PB4);

	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD4);

	bool isPressedPD2 = false;
	bool isPressedPD4 = false;

	while (1)
	{
		isPressedPD2 = !(PIND & (1 << PD2)); //1 si appuyer et 0 si relacher
		if (isPressedPD2 == true)
		{
			if (value < 15)
				value++;
			_delay_ms(200);
		}

		isPressedPD4 = !(PIND & (1 << PD4)); //1 si appuyer et 0 si relacher
		if (isPressedPD4 == true)
		{
			if (value > 0)
				value--;
			_delay_ms(200);
		}

		ft_display(value);
	}

}