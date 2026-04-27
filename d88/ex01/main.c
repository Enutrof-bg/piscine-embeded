#include "main.h"

bool ft_find(char c, char *str1) {
	while (*str1 != '\0') {
		if (c == *str1) {
			return (true);
		}
		str1++;
	}
	return (false);
}

bool ft_check_hex(char c) {
	char hex[] = "0123456789ABCDEF";
	if (ft_find(c, hex) == false)
		return false;
	return true;
}

uint8_t ft_strlen(char *input) {
	uint8_t i;

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

// bool ft_check_input(char *input, uint16_t *address, uint8_t *value) {
// 	uint8_t max = ft_strlen(input);
// 	if (max == 5)
// 	{
// 		value[5] = value[4];
// 		value[4] = value[3];
// 		value[3] = value[2];
// 		value[2] = value[1];
// 		value[1] = value[0];
// 		value[0] = '0';
// 	}

// 	uint8_t i = 0;
// 	while (input[i] != ' ' && input[i] != '\0') {
// 		if (ft_check_hex(input[i]) == false)
// 			return false;
// 		address += ft_hex(input[i]) * ft_power(16, 2 - i);
// 		i++;
// 	}

// 	while (input[i] != '\0') {
// 		if (input[i] != ' ' && ft_check_hex(input[i]) == false)
// 			return false;
// 		value += ft_hex(input[i]) * ft_power(16, 5 - i);
// 		i++;
// 	}

// 	return true;
// }

bool ft_check_input(uint8_t *input, uint16_t *address, uint16_t *value) {
	if (ft_check_hex(input[0]) == false)
		return false;
	if (ft_check_hex(input[1]) == false)
		return false;
	if (input[2] != ' ')
		return false;
	if (ft_check_hex(input[3]) == false)
		return false;
	if (ft_check_hex(input[4]) == false)
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
	uint8_t input[BUFFER_SIZE];
	bool isInputValid = true;
	uint16_t address = 0;
	uint16_t value = 0;

	uart_printstr(">");
	ft_get_input(input, MODE_INPUT);
	
	ft_print_input(input);

	isInputValid = ft_check_input(input, &address, &value);
	ft_print_input(input);
	if (isInputValid) {
		ft_print_input(input);
		uart_printstr("GOOD\r\n");
		uart_tx(input[0] + '0');
		uart_printstr("\r\n");
		address = ft_hex(input[0] + '0') * 16 + ft_hex(input[1] + '0');
		value = ft_hex(input[3] + '0') * 16 + ft_hex(input[4] + '0');

		ft_print_input(input);
		// ft_uart_print_adc_10bit(address);
		// uart_printstr("\r\n");
		// ft_uart_print_adc_10bit(value);
		// uart_printstr("\r\n");
		// // ft_eeprom_replace();
	}
	else {
		uart_printstr("BAD\r\n");
		uart_printstr("FORMAT: 2 or 3 Hexa value - space - 2 Hexa value (HH HH)");
	}
	uart_printstr("\r\n");
}

void ft_init(void) {
	uart_init(MYUBRR);

	// ft_print_eeprom();
}

int main() {

	ft_init();
	
	while (1)
	{
		ft_get_eeprom();
	}	
}