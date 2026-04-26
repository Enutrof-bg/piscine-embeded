#include "main.h"

volatile uint16_t g_state = 0;
volatile uint8_t g_data[7] = {0};
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

void i2c_status(void) {
	g_state = TWSR & 0xF8;
}

void ft_print_status(void) {
	ft_uart_print_adc_10bit(g_state);
	uart_printstr("\r\n");
}

void ft_update_and_print_status() {

	i2c_status();
	uart_printstr("Status code:");
	ft_print_status();
}

void i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while ( !(TWCR & (1 << TWINT)) )
	{
	}

	i2c_status();
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

	i2c_status();
}

uint8_t i2c_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void print_hex_value(char c) {
	uart_tx(ft_hex(c / 16));
	uart_tx(ft_hex(c % 16));
}

void ft_display_data() {
	for (uint8_t i = 0; i < 7; i++){
		ft_uart_print_hex(g_data[i]);
		// print_hex_value((char)g_data[i]);
		if (i < 6)
			uart_printstr(" ");
	}
	uart_printstr("\r\n");
}

void i2c_read(void) {
	i2c_start();
	i2c_write((T_SENSOR_ADDR << 1) | 1);

	g_data[0] = i2c_read_ack();
	g_data[1] = i2c_read_ack();
	g_data[2] = i2c_read_ack();
	g_data[3] = i2c_read_ack();
	g_data[4] = i2c_read_ack();
	g_data[5] = i2c_read_ack();
	g_data[6] = i2c_read_nack();

	ft_display_data();
}


void ft_protocole(void) {
	
	i2c_start();
	i2c_write((T_SENSOR_ADDR << 1) | 0);

	if (g_state != MT_SLA_ACK) //sla has been transmitted and ack received
	{
		uart_printstr("MT_SLA_ACK_STATUS_NOT_DETECTED\r\n");
		return ;
	}
	
	_delay_ms(40);
	i2c_write(0x71);

	if ((g_state & (1 << 3))) 
	{
		i2c_write(0xBE);
		i2c_write(0x08);
		_delay_ms(160);
		i2c_write(0x00);
		i2c_stop();

		_delay_ms(10);
	}

	i2c_start();
	i2c_write((T_SENSOR_ADDR << 1) | 0);
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);

	_delay_ms(80);

	i2c_status();

	if (!(g_state & (1 << 7)))
	{
		i2c_read();
	}
	i2c_stop();
}

