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


void ft_print_status(void) {
    uart_printstr("Status code: 0x");
    ft_uart_print_hex(g_state);
    uart_printstr(" : ");

    switch(g_state) {
        case 0x08:
            uart_printstr("START condition transmitted");
            break;
        case 0x10:
            uart_printstr("Repeated START transmitted");
            break;

        case 0x18: 
	        uart_printstr("SLA+W transmitted, ACK received");
            break;

        case 0x20:
            uart_printstr("SLA+W transmitted, NACK received");
            break;

        case 0x28: 
            uart_printstr("Data byte transmitted, ACK received");
            break;

        case 0x30: 
            uart_printstr("Data byte transmitted, NACK received");
            break;
      
        case 0x40:
            uart_printstr("SLA+R transmitted, ACK received");
            break;

        case 0x48:
            uart_printstr("SLA+R transmitted, NACK received");
            break;

        case 0x50:
            uart_printstr("Data byte received, ACK returned");
            break;

        case 0x58:
            uart_printstr("Data byte received, NACK returned");
            break;

        case 0x38:
            uart_printstr("Arbitration lost");
            break;

        case 0x00:
            uart_printstr("Bus error (Illegal START/STOP)");
            break;

        default:
            uart_printstr("Unknown status");
            break;
    }
    uart_printstr("\r\n");
}


void i2c_status(void) {
	// DATASHEET PAGE 240 SECTION 22.9.3
	// get status code with a mask to hide prescaler bit
	g_state = TWSR & 0xF8;
	ft_print_status();
}


void i2c_start(void) {
	// DATAHSSET PAGE 239 SECTION 22.9.2 //note1
	// TWINT: interrupt flag
	// TWSTA: start condition bit
	// TWEN: enable bit
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while ( !(TWCR & (1 << TWINT)) )
	{
	}

	i2c_status();
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}


/*
DATASHEET PAGE 224 
The application must load a data packet into TWDR. Subsequently, a
specific value must be written to TWCR, instructing the TWI hardware to transmit the data packet present
in TWDR.
*/
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
    while (!(TWCR & (1 << TWINT)))
	{
	}

	i2c_status();
    return TWDR;
}

uint8_t i2c_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
	{
	}

	i2c_status();
    return TWDR;
}

void print_hex_value(char c) {
	uart_tx(ft_hex((uint8_t)c / 16));
	uart_tx(ft_hex((uint8_t)c % 16));
}

void ft_display_data() {
	for (uint8_t i = 0; i < 7; i++){
		print_hex_value(g_data[i]);
		if (i < 6)
			uart_printstr(" ");
	}
	uart_printstr("\r\n");
}

// Read the next 7 bytes to get the data and display it
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

// Sensor reading process datasheet SECTION 5.4
void ft_protocole(void) {
	
	/*
	After the transmission is initiated, the first byte of the
	subsequent I2C transmission includes the 7-bit I2C
	device address 0x38 and a SDA direction bit x (read R:
	‘1’, write W: ‘0’). 
	*/
	i2c_start();
	i2c_write((T_SENSOR_ADDR << 1) | 0);

	/*
	1. Wait 40ms after power-on.
	Before reading the temperature and humidity values, first check whether the calibration
	enable bit Bit [3] of the status word is 1 (you can get a byte of
	status word by sending 0x71). If not 1, need to send 0xbe
	command (for initialization), this command parameter has
	two bytes, the first byte is 0x08, the second byte is 0x00,
	and then wait for 10ms.
	*/
	_delay_ms(40);

	i2c_write(0x71);
	if ((g_state & (1 << 3))) 
	{
		i2c_write(0xBE);
		i2c_write(0x08);
		i2c_write(0x00);
		_delay_ms(10);
	}

	/*
	2. Send the 0xAC command directly (trigger measurement).
	The parameter of this command has two bytes, the first byte
	is 0x33 and the second byte is 0x00.
	*/

	i2c_start();
	i2c_write((T_SENSOR_ADDR << 1) | 0);
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);

	/*
	3. Wait for 80ms to wait for the measurement to be completed.
	If the read status word Bit [7] is 0, it indicates that the
	measurement is completed, and then six bytes can be read
	in a row; otherwise, continue to wait.
	*/
	_delay_ms(80);

	i2c_status();

	if (!(g_state & (1 << 7)))
	{
		i2c_read();
	}
	i2c_stop();
}

// DATAHSSET PAGE 239 SECTION 22.9.2 
	// TWINT: interrupt flag
	// TWSTA: start condition bit
	// TWEN: enable bit

	//note1
/*
	Bit 7 – TWINT: TWI Interrupt Flag
	This bit is set by hardware when the TWI has finished its current job and expects application software response.
	If the I-bit in SREG and TWIE in TWCR are set, the MCU will jump to the TWI Interrupt Vector. While the TWINT
	Flag is set, the SCL low period is stretched. The TWINT Flag must be cleared by software by writing a logic one
	to it. Note that this flag is not automatically cleared by hardware when executing the interrupt routine. Also note
	that clearing this flag starts the operation of the TWI, so all accesses to the TWI Address Register (TWAR), TWI
	Status Register (TWSR), and TWI Data Register (TWDR) must be complete before clearing this flag.
*/

/*
	• Bit 5 – TWSTA: TWI START Condition Bit
	The application writes the TWSTA bit to one when it desires to become a Master on the 2-wire Serial Bus. The
	TWI hardware checks if the bus is available, and generates a START condition on the bus if it is free. However,
	if the bus is not free, the TWI waits until a STOP condition is detected, and then generates a new START
	condition to claim the bus Master status. TWSTA must be cleared by software when the START condition has
	been transmitted.
*/

/*
	Bit 2 – TWEN: TWI Enable Bit
	The TWEN bit enables TWI operation and activates the TWI interface. When TWEN is written to one, the TWI
	takes control over the I/O pins connected to the SCL and SDA pins, enabling the slew-rate limiters and spike
	filters. If this bit is written to zero, the TWI is switched off and all TWI transmissions are terminated, regardless of
	any ongoing operation.
*/