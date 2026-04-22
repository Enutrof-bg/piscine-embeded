#include "main.h"

uint8_t ft_adc_read();
void ft_uart_print_adc(uint8_t c);

__attribute__((signal))
void TIMER1_COMPA_vect() {
	uint8_t c = ft_adc_read();
	ft_uart_print_adc(c);
}


void ft_adc() {
	// DATASHEET PAGE 258 SECTION 24.9.2 //NOTE1
	ADCSRA |= (1 << ADEN); // ENABLE ADC

	//DATASHHET PAGE 259 TABLE 24-5 //NOTE2
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //DIVISION FACTOR 128

	//DATASHEET PAGE 257 SECTION 24.9.1 //NOTE3
	ADMUX |= (1 << REFS0); //AVCC with external capacitor at AREF pin

	//DATASHEET PAGE 259 SECTION 24.9.3 //NOTE4
	ADMUX |= (1 << ADLAR); //ADCH and ADCL:	When an ADC conversion is complete, the result is found in these two registers

	ADCSRA |= (1 << ADSC);

	while ((ADCSRA & (1 << ADSC)))
	{
	}
}

uint8_t ft_adc_read() {
	//DATASHSSET PAGE 258 SECTION 24.9.2 //NOTE5
	ADCSRA |= (1 << ADSC); // start conversion

	while ((ADCSRA & (1 << ADSC))) // ADSC is up while adsc is still ongoing
	{
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
	uart_printstr("\r\n");
}

void ft_init() {
	uart_init(MYUBRR);
	setup_timer();
	ft_adc();
}

int main() {
	
	ft_init();
	
	while(1)
	{
	}

}

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