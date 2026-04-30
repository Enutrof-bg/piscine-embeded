#include "main.h"

bool ft_compare(uint8_t *s1, uint8_t *s2) {
	while (*s1 != '\0' && *s2 != '\0') {
		if (*s1 != *s2) {
			return false;
		}
		s1++;
		s2++;
	}

	return (*s1 == '\0' && *s2 == '\0');
}

uint8_t ft_hex(uint8_t val) {
	char hex[] = "0123456789ABCDEF";

	return hex[val];
}

bool ft_find(char c, char *str1) {
	while (*str1 != '\0') {
		if (c == *str1) {
			return (true);
		}
		str1++;
	}
	return (false);
}

bool ft_check_dec(char c) {
	char dec[] = "0123456789";
	if (ft_find(c, dec) == false)
		return false;
	return true;
}

bool ft_check_hex(char c) {
	char hex[] = "0123456789abcdefABCDEF";
	if (ft_find(c, hex) == false)
		return false;
	return true;
}

bool ft_check_tag(char c) {
	char dec[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
	if (ft_find(c, dec) == false)
		return false;
	return true;
}

uint16_t ft_get_value(char c) {
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
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
