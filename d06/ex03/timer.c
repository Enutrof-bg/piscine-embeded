#include "main.h"

/*

k is a fixed coefficient
	1100mV / 1024(adc) ~= 1.07mV/adc
	
	//datasheet page256
		We suppose 1mV = 1degree Celsius, we will work with this for this part 

	1.07mV = 1adc
	1mV ~= 0.93adc

	0.93adc ~= 1degree Celsius
	1adc ~= 1.07degree Celsius

	coefficient k ~= 1.07 (107 / 100)

*/
#define K (107 / 100)


/*
Temperature sensor offset 
	TOS = adc - (Temperature reel / k)
		~= 358 - (25 / K)

	- we will work with a temperature of 25degree celsius in the room
	- the internal temperature sensor gives 358adc on average:
*/

#define TEMPERATURE_IRL 25
#define TEMPERATURE_ADC 354
#define TOS (TEMPERATURE_ADC - (TEMPERATURE_IRL / K))

__attribute__((signal))
void TIMER1_COMPA_vect() {
	uint16_t c = ft_adc_read_10bit();

	//T = { [(ADCH << 8) | ADCL] - TOS} / k  //datashhet page 257
	// k is a fixed coefficient and TOS is the temperature sensor offset value
	// determined and stored into EEPROM as a part of the production test.
	// int16_t t = (c);
	int16_t t = (c - TOS) * K;

	ft_uart_print_adc_10bit(t);
	uart_printstr("\r\n");
}


void setup_timer() {

	OCR1A = (F_CPU / 1024UL / 50UL) - 1; //0.02SEC POUR QUE TCNT1 REACH OCR1A
	TCNT1 = 0;

	// PAGE 142 SECTION 16.11.2
	TCCR1B |= (1 << WGM12); //MODE 4 CTC

	TCCR1B |= (1 << CS12) | (1 << CS10); //PRESCALER 1024

	TIMSK1 |= (1 << OCIE1A);

	/*
	DATASHEET PAGE 144
	 Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally enabled), the
	Timer/Counter1 Output Compare A Match interrupt is enabled. The corresponding Interrupt Vector (see
	“Interrupts” on page 66) is executed when the OCF1A Flag, located in TIFR1, is set
	*/
	SREG |= (1 << SREG_I); //DATASHEET PAGE 20 SECTION 7.3.1
}

// test save
// int16_t t = (c);
// int16_t t = (c - 334) * 107 / 100;
// int16_t t = (c - 324) * 100 / 122;