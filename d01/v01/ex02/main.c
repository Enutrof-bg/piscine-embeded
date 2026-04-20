#include "main.h"

int main() {
	DDRB |= (1 << PB0); // set PB as output pin

	DDRD &= ~(1 << PD2); // set PD2 as input pin

	bool isPressed = false;
	
	while (1)
	{
		isPressed = !(PIND & (1 << PD2)); // lit la tension du boutton PD2 et stocke
										  // isPressed==true si PD2 appuye sinon false
		if (isPressed) 
		{
			PORTB |= (1 << PB0); // allume la led si isPressed est true
		}
		else {
			PORTB &= ~(1 << PB0); // eteint la led
		}
	}

}

// DATASHEET: PAGE 70 - 73
