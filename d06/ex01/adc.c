#include "main.h"

void ft_adc_init() {
	// DATASHEET PAGE 258 SECTION 24.9.2 //NOTE1
	ADCSRA |= (1 << ADEN); // ENABLE ADC

	//DATASHHET PAGE 259 TABLE 24-5 //NOTE2
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //DIVISION FACTOR 128

	//DATASHEET PAGE 259 // NOTE 6
	// ADCSRA |= (1 << ADIE); //When this bit is written to one and the I-bit in SREG is set, the ADC Conversion Complete Interrupt is activated
}

void ft_adc_potensiometer() {
	ADMUX = 0;

	//DATASHEET PAGE 257 SECTION 24.9.1 //NOTE3
	ADMUX |= (1 << REFS0); //AVCC with external capacitor at AREF pin

	//DATASHEET PAGE 259 SECTION 24.9.3 //NOTE4
	ADMUX |= (1 << ADLAR); //ADCH and ADCL:	When an ADC conversion is complete, the result is found in these two registers
}

void ft_adc_light(void) {
	ADMUX = 0;

	//DATASHEET PAGE 257 SECTION 24.9.1 //NOTE3
	ADMUX |= (1 << REFS0); //AVCC with external capacitor at AREF pin

	//DATASHEET PAGE 259 SECTION 24.9.3 //NOTE4
	ADMUX |= (1 << ADLAR); //ADCH and ADCL:	When an ADC conversion is complete, the result is found in these two registers


	ADMUX |= (1 << MUX0);	
}

void ft_adc_temperature(void) {
	ADMUX = 0;

	//DATASHEET PAGE 257 SECTION 24.9.1 //NOTE3
	ADMUX |= (1 << REFS1) | (1 << REFS0);

	//DATASHEET PAGE 259 SECTION 24.9.3 //NOTE4
	ADMUX |= (1 << ADLAR); //ADCH and ADCL:	When an ADC conversion is complete, the result is found in these two registers

	ADMUX |= (1 << MUX3);
}
