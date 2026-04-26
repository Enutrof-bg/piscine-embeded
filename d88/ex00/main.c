#include "main.h"


//https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf



void ft_init(void) {
	uart_init(MYUBRR);

}

int main() {

	ft_init();

	while (1)
	{
		uint8_t wordByte;
		uint16_t index = 0;
		wordByte = eeprom_read_byte((uint8_t*)index);
		index++;
		ft_uart_print_hex(wordByte);
	}	
}