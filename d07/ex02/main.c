#include "main.h"


//https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf



void ft_init(void) {
	uart_init(MYUBRR);
	i2c_init();
	// setup_timer();
}

int main() {

	ft_init();

	while (1)
	{
		ft_protocole();
		_delay_ms(2000);
	}	
}