#include "main.h"

int main() {
	
//PMW
	DDRB |= (1 << PB1);

	ICR1 = F_CPU / 256UL - 1; // == 62499
	OCR1A = ICR1 * 0.1; // == 6249
	TCNT1 = 0; //compteur depart jusqu'a OCR1A
	// 16000000 / 256 = 62500; 1sec vaut 62500 tics pour l'horloge timer1 mais 16000000 tics pour le cpu

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
	}
}

//PRESCALER
// (1 << CS10)					1	16 MHz	4 ms
// (1 << CS11)					8	2 MHz	32 ms
// (1 << CS11) | (1 << CS10)	64	250 kHz	262 ms
// (1 << CS12)					256	62.5 kHz	1.04 s
// (1 << CS12) | (1 << CS10)	1024



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





//LE BYTE TIFR1 CONTIENT LES BIT OCF1A ET OCF1B
	//OCF1A LE FLAG UP QUAND TCNT1==OCR1A
	//OCF1B LE FLAG UP QUAND TCNT1==OCR1B



//PMW 
	//COM1A1 CONTROLE OC1A
	//COM1B1 CONTROLE OC1B