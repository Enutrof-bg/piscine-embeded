#include "main.h"

void ft_init() {
	uart_init(MYUBRR);
	setup_timer();
	ft_adc();
}

int main() {
	
	ft_init();
	
	while(1)
	{
	}

}
