#include "main.h"

uint8_t ft_adc_read();
void ft_uart_print_adc(uint8_t c);
void ft_switch_state(uint8_t state);

volatile uint16_t adc_results[3]; 

void ft_print_data(void) {
	ft_uart_print_adc_10bit(adc_results[0]);
	uart_printstr(", ");
	ft_uart_print_adc_10bit(adc_results[1]);
	uart_printstr(", ");
	ft_uart_print_adc_10bit(adc_results[2]);
	uart_printstr("\r\n");
}

__attribute__((signal))
void TIMER1_COMPA_vect() {

	ft_print_data();
	
}

void ft_switch_state(uint8_t state) {
	uint16_t c;

	switch (state) {
		case TENSIOMETRE:

			ft_adc_potensiometer();
			c = ft_adc_read_10bit();
			adc_results[TENSIOMETRE] = c;

			break;

		case LIGHT:

			ft_adc_light();
			c = ft_adc_read_10bit();
			adc_results[LIGHT] = c;

			break;

		case TEMPERATURE:

			ft_adc_temperature();
			c = ft_adc_read_10bit();
			adc_results[TEMPERATURE] = c;

			break;
		
		default:
			break;
	}
}

void ft_init() {
	uart_init(MYUBRR);
	setup_timer();
	ft_adc_init();

	SREG |= (1 << SREG_I); //DATASHEET PAGE 20 SECTION 7.3.1

}

int main() {
	
	ft_init();

	while(1)
	{
		ft_switch_state(TENSIOMETRE);

		ft_switch_state(LIGHT);

		ft_switch_state(TEMPERATURE);
	}

}
