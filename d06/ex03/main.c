#include "main.h"

__attribute__((signal))
void TIMER1_COMPA_vect() {
	uint16_t c = ft_adc_read_10bit();
	uint16_t t = (c - 324) * 100 / 122;
	ft_uart_print_adc_10bit(t);
	uart_printstr("\r\n");
}

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
