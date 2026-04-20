#include "main.h"

int main() {
	DDRB |= (1 << PB0); // set PB as output pin
	// DDRB &= ~(1 << PB0); // set PB as input pin

	while (1) {
		PORTB |= (1 << PB0);
	}
}

// DATASHEET: PAGE 59
// DDRX: pour set en input ou output 
// PORTX: pour demander a executer qquechose
// PINX: pour lire une donnee/ un etat

// DDRB met pb0 en output(on "ecrire" des donnees), puis PORTB permet d'activer pb0

// Puis une boucle while sinon la board va continuer a lire les zones memoires suivantes
// puis redamarrer et recommencer au debut