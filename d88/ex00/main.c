#include "main.h"


void ft_init(void) {
	uart_init(MYUBRR);
	// setup_timer();
	ft_print_eeprom();
}

int main() {

	ft_init();
	
	while (1)
	{
		
	}	
}