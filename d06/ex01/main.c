#include "main.h"
#include "uart.h"
#include "timer.h"
#include "adc.h"

uint8_t ft_adc_read();
void ft_uart_print_adc(uint8_t c);
void ft_switch_state(uint8_t state);

volatile uint8_t adc_results[3]; 

void ft_print_data(void) {
	ft_uart_print_adc(adc_results[0]);
	uart_printstr(", ");
	ft_uart_print_adc(adc_results[1]);
	uart_printstr(", ");
	ft_uart_print_adc(adc_results[2]);
	uart_printstr("\r\n");
}

__attribute__((signal))
void TIMER1_COMPA_vect() {

	ft_switch_state(TENSIOMETRE);

	ft_switch_state(LIGHT);

	ft_switch_state(TEMPERATURE);

	ft_print_data();
}

void ft_switch_state(uint8_t state) {
	uint8_t c;

	switch (state) {
		case TENSIOMETRE:

			ft_adc_potensiometer();
			c = ft_adc_read();
			ft_uart_print_adc(c);
			uart_printstr(", ");
			adc_results[TENSIOMETRE] = c;

			break;

		case LIGHT:

			ft_adc_light();
			c = ft_adc_read();
			ft_uart_print_adc(c);
			uart_printstr(", ");
			adc_results[LIGHT] = c;

			break;

		case TEMPERATURE:

			ft_adc_temperature();
			// _delay_ms(100);
			c = ft_adc_read();
			ft_uart_print_adc(c);
			uart_printstr("\r\n");
			adc_results[TEMPERATURE] = c;
	
			break;
		
		default:
			break;
	}
}


uint8_t ft_adc_read() {
	//DATASHSSET PAGE 258 SECTION 24.9.2 //NOTE5
	ADCSRA |= (1 << ADSC);

	while ((ADCSRA & (1 << ADSC)))
	{
		;
	}
	return ADCH;
}

uint8_t ft_hex(uint8_t val) {
	char hex[] = "0123456789ABCDEF";
	
	return hex[val];
}

void ft_uart_print_adc(uint8_t c) {
	uart_tx(ft_hex(c / 16));
	uart_tx(ft_hex(c % 16));
}

void ft_init() {
	uart_init(MYUBRR);
	setup_timer();
	ft_adc_init();

	SREG |= (1 << SREG_I); //DATASHEET PAGE 20 SECTION 7.3.1

}

int main() {
	
	ft_init();

	while(1)
	{
	}

}

	// ADMUX |= (1 << MUX0);	
	// ADMUX |= (1 << MUX1);
	// ADMUX |= (1 << MUX1)  | (1 << MUX0); //TIMER INVERSE LENT ??
	// ADMUX |= (1 << MUX2); // FF
	// ADMUX |= (1 << MUX2) | (1 << MUX0); // FF
	// ADMUX |= (1 << MUX2) | (1 << MUX1);
	// ADMUX |= (1 << MUX2) | (1 << MUX1)  | (1 << MUX0);
	// ADMUX |= (1 << MUX3); //temperature

//DATASHEET PAGE 251 

// ntc: Negative Temperature Coefficient

//NOTE1
/*
	• Bit 7 – ADEN: ADC Enable
	Writing this bit to one enables the ADC. By writing it to zero, the ADC is turned off. Turning the ADC off while a
	conversion is in progress, will terminate this conversion
*/

//NOTE2
/*
	• Bits 2:0 – ADPS[2:0]: ADC Prescaler Select Bits
	These bits determine the division factor between the system clock frequency and the input clock to the ADC.
*/


//NOTE3
/*
	Bit 7:6 – REFS[1:0]: Reference Selection Bits
	These bits select the voltage reference for the ADC, as shown in Table 24-3. If these bits are changed during a
	conversion, the change will not go in effect until this conversion is complete (ADIF in ADCSRA is set). The
	internal voltage reference options may not be used if an external reference voltage is being applied to the AREF
	pin.
*/


//NOTE4
/*
	Bit 5 – ADLAR: ADC Left Adjust Result
	The ADLAR bit affects the presentation of the ADC conversion result in the ADC Data Register. Write one to
	ADLAR to left adjust the result. Otherwise, the result is right adjusted. Changing the ADLAR bit will affect the
	ADC Data Register immediately, regardless of any ongoing conversions. For a complete description of this bit,
	see ”ADCL and ADCH – The ADC Data Register” on page 25
*/


//NOTE5
/*
	• Bit 6 – ADSC: ADC Start Conversion
	In Single Conversion mode, write this bit to one to start each conversion. In Free Running mode, write this bit to
	one to start the first conversion. The first conversion after ADSC has been written after the ADC has been
	enabled, or if ADSC is written at the same time as the ADC is enabled, will take 25 ADC clock cycles instead of
	the normal 13. This first conversion performs initialization of the ADC.
	ADSC will read as one as long as a conversion is in progress. When the conversion is complete, it returns to
	zero. Writing zero to this bit has no effect.
*/

//NOTE6
/*
 Bit 4 – ADIF: ADC Interrupt Flag
This bit is set when an ADC conversion completes and the Data Registers are updated. The ADC Conversion
Complete Interrupt is executed if the ADIE bit and the I-bit in SREG are set. ADIF is cleared by hardware when
executing the corresponding interrupt handling vector. Alternatively, ADIF is cleared by writing a logical one to
the flag. Beware that if doing a Read-Modify-Write on ADCSRA, a pending interrupt can be disabled. This also
applies if the SBI and CBI instructions are used.
*/