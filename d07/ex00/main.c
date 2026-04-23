#include "main.h"

void ft_init(void) {
	uart_init(MYUBRR);
	i2c_init();
	setup_timer();
}

int main() {

	ft_init();

	while (1)
	{
	}	
}