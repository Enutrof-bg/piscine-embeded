#include "main.h"

__attribute__((signal))
void TIMER1_COMPA_vect() {
	uint8_t c = ft_adc_read();
	ft_uart_print_adc(c);
	wheel(c);
	ft_led(c);
}

void ft_init_led() {
	DDRD |= (1 << DDD3); //blue
	DDRD |= (1 << DDD5); //red
	DDRD |= (1 << DDD6); //green

	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	DDRB |= (1 << PB4);

}

void ft_init() {
	uart_init(MYUBRR);
	ft_init_led();
	init_rgb();
	setup_timer();
	ft_adc();
}

int main() {
	
	ft_init();
	
	while(1)
	{
	}

}
