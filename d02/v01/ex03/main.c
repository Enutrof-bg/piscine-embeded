#include "main.h"
	
int main() {

	DDRB |= (1 << PB1);

	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD4);

	uint8_t DUTY_CYCLE = 10; // de 10 a 100

	ICR1 = F_CPU / 256UL - 1; // 256 car prescaler de 256
	OCR1A = ((((uint32_t)ICR1 + 1) * DUTY_CYCLE) / 100) - 1; // 10% de ICR1 au debut
	TCNT1 = 0; //compteur depart jusqu'a ICR1
	// 16000000 / 256 = 62500

	//MODE
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12); //MODE 14 FAST PMW
	// DATASHEET PAGE 140-141: MODE 14 FAST PMW
	// TOP: ICR1
	// BOTTOM: OCR1A

	// PRESCALER 256
	TCCR1B |= (1 << CS12); // 256 tics du cpu vaut 1 tics du timer1
	// CONNECT TIMER A L'HORLOGE DU MICROCONTROLLEUR (SINON 0)
	// DATASHEET PAGE 139



	TCCR1A |= (1 << COM1A1) | (0 << COM1A0);
	// DATASHEET PAGE 140: COMPARE OUTPUT MODE PWM
	// Clear OC1A/OC1B on Compare Match, set
	// OC1A/OC1B at BOTTOM (non-inverting mode)


	// La led s'allume quand TCNT1==0
	// Au compare match TCNT1==OCR1A, la led s'eteint
	// Pour TCNT1==ICR1, TCNT1 revient a 0(se rallume donc)

	while (1)
	{
		if (!(PIND & (1 << PD2)))
		{
			if (DUTY_CYCLE <= 90)
			{
				DUTY_CYCLE += 10;
				OCR1A = ((((uint32_t)ICR1 + 1) * DUTY_CYCLE) / 100) - 1;
				//Modifie le DUTY_CYCLE puis met a jour OCR1A
			}
			
			while (!(PIND & (1 << PD2)))
			{}
			_delay_ms(50); //bounce 💃
		}

		if (!(PIND & (1 << PD4)))
		{
			if (DUTY_CYCLE > 10)
			{
				DUTY_CYCLE -= 10;
				OCR1A = ((((uint32_t)ICR1 + 1) * DUTY_CYCLE) / 100) - 1;
			}
			
			while (!(PIND & (1 << PD4)))
			{}
			_delay_ms(50); //bounce 💃
		}
	}
}

// TCNT1 (Timer/Counter Register): compte les tick
// OCR1A/B (Output Compare Register): valeur de reference
// ICR1 (Input Capture Register): limit max

// TCCR1A (Timer Counter Control Register): config sortie
// TCCR1B (Timer Counter Control Register): config mode + vitesse

// CS (Clock set): diviseur horloge
// WGM (Waveform generation mode): mode
// COM (COmpare output mode): action sur les pin

// CTC: (Clear Timer on Compare Match) reset auto du compteur
// PWM (Pulse Width Modulation): signal on/off de variable
