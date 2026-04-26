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