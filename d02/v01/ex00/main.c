#include "main.h"

// #define LOOP_CYCLE 65536

void ft_delay() 
{
	volatile uint8_t j;
	volatile uint16_t i;

	//empeche la suppression et respect chaque instruction dans le cas ou le flag optimization -Os est activee
	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 52631; i++)
		{
		}
	}
}

int main() {
	
	DDRB |= (1 << PB1);
	// PORTB ^= (1 << PB1);
	while (1)
	{
		ft_delay();
		PORTB ^= (1 << PB1); //XOR
	}
}

// lecture de  j : 5 cycles
// incrementation de j : 5 cycles
// ecriture/lecture de j : 2 cycles
// comparaison < de j : 5 cycles
// saut : 2 cycles
// (52631 boucles * 19 cycles) = (999989 * 8 boucles) = 8000000


//avr-objdump -S main.bin --disassemble=ft_delay
//DATASHEET PAGE 626: cycle asm