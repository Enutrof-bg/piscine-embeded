#include "main.h"

uint8_t ft_strlen(char *input) {
	uint8_t i = 0;

	while (input[i])
		i++;
	return i;
}

uint16_t ft_power(uint16_t nbr, uint8_t power) {
	uint16_t result = 0;
	if (power == 0)
		return 1;

	while (power > 0) {
		result = result * nbr;
		power--;
	}
	return result;
}

bool ft_check_input(uint16_t address, uint16_t value) {
	if (address > 1023)
		return false;
	if (value > 255)
		return false;
	return true;
}

void ft_print_input(uint8_t *input) {
	uint8_t max = ft_strlen(input);
	for (uint8_t i = 0; i < max; i++) {
		uart_tx(input[i]);
	}
	uart_printstr("\r\n");
}

void ft_get_eeprom() {

	bool isInputValid = true;

	uart_printstr("> ");

	bool address_valid = true;
	bool value_valid = true;
	uint16_t address = ft_get_input(4, &address_valid);
	uint16_t value = ft_get_input(2, &value_valid);
	
	uart_printstr("\r\n");

	ft_uart_print_adc_10bit(address);
	uart_printstr(" ");
	ft_uart_print_adc_10bit(value);
	uart_printstr("\r\n");

	isInputValid = address_valid && value_valid && ft_check_input(address, value);
	if (isInputValid) {
		uart_printstr("GOOD input\r\n");
		if (ft_replace_eeprom(address, value) == true)
			ft_print_eeprom(address);
		else {
			uart_printstr("Value is already the same at the address\r\n");
		}
	}
	else {
		uart_printstr("BAD\r\n");
	}
}

void ft_init(void) {
	uart_init(MYUBRR);
}

int main() {

	ft_init();

	while (1)
	{
		ft_get_eeprom();
	}	
}