#include "main.h"

static volatile uint16_t slot[SLOT_SIZE] = {0x00, 0x100, 0x200, 0x300};

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
	uart_printstr("Node ID: ");
	ft_uart_print_adc_10bit(node->nodeID);
	uart_printstr("\r\nPriority: ");
	uart_putnbr(node->priority);
	uart_printstr("\r\nSlot: ");
	ft_uart_print_adc_10bit(SLOT_SIZE);
	uart_printstr("\r\nTag: ");
	uart_printstr(node->tag);
	uart_printstr("\r\n");
}

uint8_t ft_status(t_node *node) {
	for (uint8_t i = 0; i < SLOT_SIZE; i++) {
		if (EEPROM_read(slot[i]) == MAGIC_NUMBER) {
			uint8_t crc_data[39];
			for (uint8_t j = 0; j < 39; j++) {
				crc_data[j] = EEPROM_read(slot[i] + 1 + j);
			}
			uint16_t crc = ft_get_crc16(crc_data, 39);
			uint16_t stored_crc = (uint16_t)EEPROM_read(slot[i] + 40)
				| ((uint16_t)EEPROM_read(slot[i] + 41) << 8);
			if (stored_crc != crc) {
				uart_printstr("CRITICAL: Data corruption detected\r\n");
				return 2;
			} else {
				ft_display_status(node);
				return 1;
			}
		}
	}
	uart_printstr("Node unconfigured\r\n");
	return 0;
}

uint8_t ft_set(t_node *node, int8_t cmd) {
	uint8_t *node_bytes = (uint8_t *)node;
	const uint8_t data_size = sizeof(t_node);
	bool slot_ok = true;
	if (cmd == CMD_SET_ID) {
		uint32_t data = ft_get_id();
		node->nodeID = data;
		ft_uart_print_adc_10bit(node->nodeID);
	}
	else if (cmd == CMD_SET_PRIO) {
		int16_t data = ft_get_prio();
		node->priority = data;
	}
	else if (cmd == CMD_SET_TAG) {
		ft_get_tag(node->tag);
	}

	//calculate CRC, integrity value
	uint16_t crc = ft_get_crc16((uint8_t *)&node->nodeID, 39);
	node->integrity = crc;

	//WRITING PROCESS
	// for (uint8_t i = 0; i < SLOT_SIZE; i++) {
	for (uint8_t i = 0; i < SLOT_SIZE; i++) {
		slot_ok = true;
		for (uint8_t addr = 0; addr < data_size; addr++) {
			uint8_t value = node_bytes[addr];
			ft_replace_eeprom((uint16_t)(slot[i] + addr), value);
			if (!ft_verification_eeprom((uint16_t)(slot[i] + addr), value)) {
				slot_ok = false;
				break;
			}
		}
		if (slot_ok) {
			uart_printstr("Done\r\n");
			return 0;
		}
		uart_printstr("Corruption detected\r\n");
		uart_printstr("Relocating config to slot ");
		uart_putnbr(i + 1);
		uart_printstr("...\r\n");
	}

	return 0;
}

uint8_t ft_reset(t_node *node) {

	const uint8_t data_size = sizeof(t_node);
	for (uint8_t i = 0; i < SLOT_SIZE; i++) {
		for (uint8_t addr = 0; addr < data_size; addr++) {
			EEPROM_write((uint16_t)(slot[i] + addr), 0xFF);
		}
	}
	node->nodeID = 0;
	node->priority = 0;
	for (uint8_t i = 0; i < TAG_SIZE; i++) {
		node->tag[i] = 0;
	}
	node->integrity = 0;
	uart_printstr("Factory reset complete\r\n");
	return 0;
}

void ft_get_eeprom(t_node *node) {

	bool isInputValid = true;

	uart_printstr("> ");

	int8_t command = ft_get_cmd();
	// ft_uart_print_adc_10bit(command);
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

		case CMD_NOT_KNOWN:
			uart_printstr("Command not recognized");
		default:
			break;
	}
	uart_printstr("\r\n");
}

void ft_init(void) {
	uart_init(MYUBRR);
}

int main() {

	ft_init();
	t_node node;
	node.magic_number = MAGIC_NUMBER;
	// node.tag = {'U', 'n', 'c', 'o', 'n','f','i','g','u','r','e', 'd'};
	while (1)
	{
		ft_get_eeprom(&node);
	}	
}