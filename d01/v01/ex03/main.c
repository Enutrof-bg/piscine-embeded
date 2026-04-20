#include "main.h"

int main() {
	DDRB |= (1 << PB0); // set PB as output pin
	DDRD &= ~(1 << PD2);

	bool isPressed = false;

	while (1)
	{
		isPressed = !(PIND & (1 << PD2)); //1 si appuyer et 0 si relacher
		if (isPressed == true)
		{
			PORTB ^= (1 << PB0); //xor operateur, inverse la valeur au bit voulu
			
			while(!(PIND & (1 << PD2)))
			{
				//truc
				//ou pas
			}
			_delay_ms(50);
			//bounce effect: possibilite que des inputs soit reconnu quand on relache le bouton
		}
	}

}
