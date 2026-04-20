#include "main.h"

int main() {
	
	DDRB |= (1 << PB1);

	OCR1A = F_CPU / 256UL / 2; // Le nombre de cycle pour 0.5sec
	TCNT1 = 0; //compteur depart jusqu'a OCR1A
	// 16000000 / 256 = 62500; 1sec vaut 62500 tics pour l'horloge timer1 mais 16000000 tics pour le cpu
	// 16000000 / 256 = 62500; 62500 / 2 = 31250

	// MODE
	// DATASHEET PAGE 131
	TCCR1B |= (1 << WGM12);
	// DATASHEET PAGE 140-141: MODE 4 CTC
	// TOP: OCR1A



	// PRESCALER 256
	TCCR1B |= (1 << CS12); // 256 tics du cpu vaut 1 tics du timer1
	// CONNECT TIMER A L'HORLOGE DU MICROCONTROLLEUR
	// DATASHEET PAGE 139



	TCCR1A |= (0 << COM1A1) | (1 << COM1A0);
	// DATASHEET PAGE 140: COMPARE OUTPUT MODE NON PWM
	// Toggle OC1A/OC1B on Compare Match.


	// OC1A DATASHEET PAGE 90:
	// Output Compare Match output: The PB1 pin can serve as an external output for the Timer/Counter1
	// Compare Match A. 


	// La led se "toggle" au compare match
	// Au compare match TCNT1==OCR1A, la led se "toggle"
	// Puis TCNT1 revient 0

	while (1)
	{
	}

}

//PRESCALER
// (1 << CS10)					= 1	16 MHz	4 ms
// (1 << CS11)					= 8	2 MHz	32 ms
// (1 << CS11) | (1 << CS10)	= 64	250 kHz	262 ms
// (1 << CS12)					= 256	62.5 kHz	1.04 s
// (1 << CS12) | (1 << CS10)	= 1024


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

//https://www.locoduino.org/spip.php?article89
//https://medium.com/@claudinebitee/electronique-embarqu%C3%A9e-d590d091e721