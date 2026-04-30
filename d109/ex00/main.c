#include "main.h"


void ft_init() {
	uart_init(MYUBRR);
	uart_printstr("Init UART\r\n");
	ft_spi_init();
	// ft_init_led();
	// init_rgb();
	// setup_timer();
	// ft_adc();
}

int main() {
	
	ft_init();
	
	while(1)
	{
	}

}
