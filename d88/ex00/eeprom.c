#include "main.h"

unsigned char EEPROM_read(unsigned int uiAddress)
{
	//DATASHEET PAGE 32 
	//EEPROM WRITE ENABLE
	while(EECR & (1<<EEPE))
	{
	}

	// set address register
	EEAR = uiAddress;

	//DATASHEET PAGE 32 SECTION 8.6.3  // NOTE1
	// start eeprom read
	EECR |= (1 << EERE);

	//return value of data register
	return EEDR;
}


void ft_print_eeprom() {
	// uint8_t wordByte;
	uint16_t index = 0;

	uint8_t eepromData[EEPROM_SIZE];

	// put the address wanted in EEAR and read his contents
	for (uint16_t index = 0; index < EEPROM_SIZE; index++)  {
		eepromData[index] = EEPROM_read(index);
	}


	for (uint16_t line = 0; line < EEPROM_SIZE; line++)
	{
		if ((line % 16 == 0))
		{
			ft_hexdump_addr(line, 16);
			uart_printstr(" ");
		}
		ft_uart_print_hex(eepromData[line]);
		uart_printstr(" ");
		if (line > 0 && ((line+1) % 16 == 0))
			uart_printstr("\r\n");
	}
	
}

//NOTE1
//the EEPROM Control Register
//DATASHEET PAGE 32 SECTION 8.6.3 
/*
	• Bit 0 – EERE: EEPROM Read Enable
	The EEPROM Read Enable Signal EERE is the read strobe to the EEPROM. When the correct address is set
	up in the EEAR Register, the EERE bit must be written to a logic one to trigger the EEPROM read. The
	EEPROM read access takes one instruction, and the requested data is available immediately. When the
	EEPROM is read, the CPU is halted for four cycles before the next instruction is executed.
	The user should poll the EEPE bit before starting the read operation. If a write operation is in progress, it is
	neither possible to read the EEPROM, nor to change the EEAR Register.
*/


/*
	• Bit 1 – EEPE: EEPROM Write Enable
	The EEPROM Write Enable Signal EEPE is the write strobe to the EEPROM. When address and data are
	correctly set up, the EEPE bit must be written to one to write the value into the EEPROM. The EEMPE bit must
	be written to one before a logical one is written to EEPE, otherwise no EEPROM write takes place. The
	following procedure should be followed when writing the EEPROM (the order of steps 3 and 4 is not essential):
	1. Wait until EEPE becomes zero.
	2. Wait until SPMEN in SPMCSR becomes zero.
	3. Write new EEPROM address to EEAR (optional).
	4. Write new EEPROM data to EEDR (optional).
	5. Write a logical one to the EEMPE bit while writing a zero to EEPE in EECR.
	6. Within four clock cycles after setting EEMPE, write a logical one to EEPE.
	The EEPROM can not be programmed during a CPU write to the Flash memory. The software must check that
	the Flash programming is completed before initiating a new EEPROM write. Step 2 is only relevant if the
	software contains a Boot Loader allowing the CPU to program the Flash. If the Flash is never being updated by
	the CPU, step 2 can be omitted. See ”Boot Loader Support – 
*/