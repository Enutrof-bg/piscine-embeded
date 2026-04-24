#include "main.h"

volatile uint16_t g_state = 0;

void i2c_init(void) {
	/*
		DATASHEET PAGE 222
		TWBR = Value of the TWI Bit Rate Register
		PrescalerValue = Value of the prescale // datasheet page241 table22-7
		
		SCL frequency = CPU Clock frequency / (16 + 2(TWBR) * PrescalerValue) //datasheet page222
		100.000 = 16.000.000 / (16 + 2(TWBR) * PrescalerValue);
		(16 + 2(TWBR) * PrescalerValue) = 160
		TWBR = 160-16 / 2 / PrescalerValue
		TWBR = 72 / PrescalerValue
	*/

	//datasheet page 239	section 22.9.1
	TWBR |= 72; //BITRATE

	//datasheet page 240	section 22.9.3
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // set prescaler to 1? ;

	//datasheet page 226	section 22.7.1  and 22.7.3
	// TWCR |= (1 << TWEN);
	//TWEN must be set to enable the 2-wire Serial Interface
	//TWEN must be written to one to enable the TWI
}

void i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while ( !(TWCR & (1 << TWINT)) )
	{
	}
	g_state = TWSR & 0xF8;

	ft_uart_print_adc_10bit(g_state);
	uart_printstr("\r\n");
}

void i2c_stop(void) {
	TWCR = (1 << TWINT);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_write(unsigned char data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while ( !(TWCR & (1 << TWINT)) )
	{
	}
}

void ft_protocole(void) {
	

	if (g_state != STATE_START)
	{
		uart_printstr("START_STATUS_NOT_DETECTED\r\n");
		return ;
	}
	TWDR = (T_SENSOR_ADDR << 1) | 1;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while ( !(TWCR & (1 << TWINT)) )
	{
	}

	g_state = TWSR & 0xF8;
	ft_uart_print_adc_10bit(g_state);
	uart_printstr("\r\n");

	if (g_state != MT_SLA_ACK) //sla has been transmitted and ack received
	{
		uart_printstr("MT_SLA_ACK_STATUS_NOT_DETECTED\r\n");
		return ;
	}
	_delay_ms(100);
	
	TWDR = 0x71;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ( !(TWCR & (1 << TWINT)) )
	{
	}
	g_state = TWSR & 0xF8;
	ft_uart_print_adc_10bit(g_state);
	uart_printstr("\r\n");


	if ((g_state) != 1) 
	{
		uart_printstr("NEED TO SEND 0xbe\r\n");
		TWDR = 0x08;
		TWCR = (1 << TWINT) | (1 << TWEN);
		while ( !(TWCR & (1 << TWINT)) )
		{
		}

		TWDR = 0x00;
		TWCR = (1 << TWINT) | (1 << TWEN);
		while ( !(TWCR & (1 << TWINT)) )
		{
		}

		_delay_ms(10);
	}

	TWDR = 0x33;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ( !(TWCR & (1 << TWINT)) )
	{
	}

	TWDR = 0x00;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ( !(TWCR & (1 << TWINT)) )
	{
	}

	_delay_ms(80);

	g_state = TWSR & 0xF8;
	if ( g_state & )



		// ft_uart_print_adc_10bit(TWSR & 0xF8);
	// print_hex_value(TWSR & 0xF8);
	// uart_printstr("\r\n");
}

void i2c_read(void)  {
	// print_hex_value(TWSR & 0xF8);
	// uart_printstr("\r\n");
}

void print_hex_value(char c) {
	uart_tx(ft_hex(c / 16));
	uart_tx(ft_hex(c % 16));
}
