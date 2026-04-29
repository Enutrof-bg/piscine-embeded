#include "main.h"

static volatile uint16_t slot[SLOT_SIZE] = {0, 50, 100, 200};

uint16_t ft_get_crc16(uint8_t *data, uint16_t size) {
    uint16_t crc = 0xFFFF; // Valeur initiale

    for (uint16_t i = 0; i < size; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; // Polynôme CCITT
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void ft_display_status(t_node *node) {
	uart_printstr("\r\nNode ID: ");
	ft_uart_print_adc_10bit(node->nodeID);
	uart_printstr("\r\nPriority: ");
	uart_putnbr(node->priority);
	uart_printstr("\r\nSlot: ");
	ft_uart_print_adc_10bit(SLOT_SIZE);
	uart_printstr("\r\nTag: ");
	uart_printstr(node->tag);
}

uint8_t ft_status(t_node *node) {
	for (uint8_t i = 0; i < SLOT_SIZE; i++)
	{
		//CHECK FOR MAGIC NUMBER FIRST
		if (EEPROM_read(slot[i]) == MAGIC_NUMBER)
		{
			//READ EVERY BYTE FROM NODE, PRIORITY AND TAG BYTE
			uint8_t crc_data[39];
			for (uint8_t j = 0; j < 39; j++)
			{
				crc_data[j] = EEPROM_read(slot[i] + 1 + j);
			}

			//RECALCULATE THE CRC VALUE AND CHECK IT MATCH TO THE STORED BYTE AT 40 AND 41
			uint16_t crc = ft_get_crc16(crc_data, 39);
			uint32_t stored_crc = ((uint32_t)EEPROM_read(slot[i] + 40))
				| ((uint32_t)EEPROM_read(slot[i] + 41) << 8);

			if (stored_crc != crc)
			{
				uart_printstr("\r\nCRITICAL: Data corruption detected!");
				continue;
			}
			else
			{
				ft_display_status(node);
				return 1;
			}
		}
	}
	uart_printstr("\r\nNode unconfigured");
	return 0;
}

//CHANGE VALUE IN CONFIG
uint8_t ft_set(t_node *node, int8_t cmd) {

	//1. First get input value after a SET COMMAND
	uint8_t *nodebyte = (uint8_t *)node;
	const uint8_t nodesize = sizeof(t_node);
	bool errorcheck = true;
	#ifdef DEBUG
		uint8_t debug = DEBUG;
	#endif

	if (cmd == CMD_SET_ID) {
		uint32_t data = ft_get_id();
		node->nodeID = data;
	}
	else if (cmd == CMD_SET_PRIO) {
		int16_t data = ft_get_prio();
		node->priority = data;
	}
	else if (cmd == CMD_SET_TAG) {
		ft_get_tag(node->tag);
	}

	//2.calculate CRC, integrity value
	uint16_t crc = ft_get_crc16((uint8_t *)&node->nodeID, 39);
	node->integrity = crc;

	//3.WRITING PROCESS
	//REPLACE EVERY BYTE FROM 0 TO 42(size of struct node) IF DIFFERENT
	//CHECK THE WRITE WAS SUCCECCFULL BY CHECKING THE NEW VALUE AT THE WRITTEN ADRESS
	//IF NOT, SWITCH TO ANOTHER SLOT
	for (uint8_t i = 0; i < SLOT_SIZE; i++) {
		errorcheck = true;
		for (uint8_t addr = 0; addr < nodesize; addr++)
		{
			uint8_t value = nodebyte[addr];
			ft_replace_eeprom((uint16_t)(slot[i] + addr), value);
			
			#ifndef DEBUG
				if (ft_verification_eeprom((uint16_t)(slot[i] + addr), value) == false)
				{
					errorcheck = false;
					break;
				}
			#endif

			#ifdef DEBUG
				if (debug > 0)
				{
					errorcheck = false;
					debug--;
					break;
				}
			#endif
		}
		if (errorcheck == true) {
			uart_printstr("\r\nDone");
			return 0;
		}
		uart_printstr("\r\nCorruption detected\r\n");
		uart_printstr("Relocating config to slot ");
		uart_putnbr(i + 1);
		uart_printstr("...\r\n");

		if (i == SLOT_SIZE - 1) {
			uart_printstr("CRITICAL EEPROM FAILURE.\r\n");
		}
	}

	return 1;
}

// WRITE EEPROM BYTE TO FF AND RESTORE CONFIG STATE
uint8_t ft_reset(t_node *node) {

	uint8_t data_size = sizeof(t_node);
	for (uint8_t i = 0; i < SLOT_SIZE; i++) {
		for (uint8_t addr = 0; addr < data_size; addr++) {
			EEPROM_write((uint16_t)(slot[i] + addr), 0xFF);
		}
	}
	node->nodeID = 0;
	node->priority = 0;
	char str[]= "Unconfigured";
	uint8_t len = ft_strlen(str);
	for (uint8_t i = 0; i < len; i++) {
		node->tag[i] = str[i];
	}
	node->tag[len] = '\0';
	node->integrity = 0;
	uart_printstr("\r\nFactory reset complete");
	return 0;
}

void ft_get_eeprom(t_node *node) {

	bool isInputValid = true;

	uart_printstr("> ");

	int8_t command = ft_get_cmd();

	//GET COMMAND FIRST
	switch (command) {
		case CMD_STATUS:
			ft_status(node);
			break;

		case CMD_SET_ID:
			ft_set(node, CMD_SET_ID);
			break;

		case CMD_SET_PRIO:
			ft_set(node, CMD_SET_PRIO);
			break;

		case CMD_SET_TAG:
			ft_set(node, CMD_SET_TAG);
			break;

		case CMD_FACTORY_RESET:
			ft_reset(node);
			break;

		case CMD_SHOW:
			uart_printstr("\r\n");
			ft_print_eeprom(0, 0);
			break;

		case CMD_NOT_KNOWN:
			uart_printstr("\r\nCommand not recognized");
			break;

		default:
			break;
	}
	uart_printstr("\r\n");
}

uint8_t ft_init_node(t_node *node) {
	for (uint8_t i = 0; i < SLOT_SIZE; i++)
	{
		//CHECK FOR MAGIC NUMBER FIRST
		if (EEPROM_read(slot[i]) == MAGIC_NUMBER)
		{
			//READ EVERY BYTE FROM NODE, PRIORITY AND TAG BYTE
			uint8_t crc_data[39];
			for (uint8_t j = 0; j < 39; j++)
			{
				crc_data[j] = EEPROM_read(slot[i] + 1 + j);
			}

			//RECALCULATE THE CRC VALUE AND CHECK IT MATCH TO THE STORED BYTE AT 40 AND 41
			uint16_t crc = ft_get_crc16(crc_data, 39);
			uint16_t stored_crc = (uint16_t)EEPROM_read(slot[i] + 40)
				| ((uint16_t)EEPROM_read(slot[i] + 41)  << 8);

			if (crc == stored_crc) {
				uint8_t size = sizeof(node);
				uint8_t *nodebyte = (uint8_t *)node;
				for (uint8_t addr = 1; addr < 42; addr++)
				{
					nodebyte[addr] = EEPROM_read(slot[i] + addr);
				}
				uart_printstr("Config found\r\n");
				return 0;
			}
			else
				continue;
		}
	}
	return 1;
}

void ft_init(void) {
	uart_init(MYUBRR);
}

int main() {

	ft_init();

	//INIT NODE
	t_node node;
	node.magic_number = MAGIC_NUMBER;
	node.nodeID = 0;
	node.priority = 0;
	char str[]= "Unconfigured";
	uint8_t len = ft_strlen(str);
	for (uint8_t i = 0; i < len; i++) {
		node.tag[i] = str[i];
	}
	node.tag[len] = '\0';
	node.integrity = 0;
	ft_init_node(&node);
	while (1)
	{
		ft_get_eeprom(&node);
	}	
}