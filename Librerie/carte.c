#include "carte.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Restituisce 1 se vuota, 0 altrimenti
int is_empty_carta(Carta lista_carte){
	if(lista_carte == NULL)
		return 1;
	else
		return 0;
}

//Crea una carta di valore value e seme corrispondente a seme e la restituisce
Carta crea_carta(int value, char seme[]){
	if(value < 1 || value > 10)
		return NULL;

	Semi my_seme;
	if(strcmp(seme, " oro ") == 0)
		my_seme = Oro;
	else if(strcmp(seme, "spade") == 0)
		my_seme = Spade;
	else if(strcmp(seme, "mazze") == 0)
		my_seme = Mazze;
	else if(strcmp(seme, "coppe") == 0)
		my_seme = Coppe;
	else
		return NULL;

	Carta my_carta = (Carta)malloc(sizeof(struct carta));
        if(is_empty_carta(my_carta) == 1)
                return NULL;

	my_carta->value = value;
	my_carta->seme = my_seme;
	my_carta->next = NULL;
	return my_carta;
}

//Restituisce NULL (lista vuota)
Carta crea_lista_carte(){
	return NULL;
}

//Se la carta non e' vuota, la svuota
void clear_carta(Carta* my_carta){
	if(is_empty_carta(*my_carta) == 0){
		free(*my_carta);
		//Imposto a NULL il puntatore
		(*my_carta) = NULL;
	}
}

//Se la lista non e' vuota, la svuota
void clear_lista_carte(Carta* lista_carte){
	//Se vuota, esco
	if(is_empty_carta(*lista_carte) == 1) return;
	for(Carta i = ricerca_carta_per_posizione(*lista_carte, 1); is_empty_carta(i) != 1; i = ricerca_carta_per_posizione(*lista_carte, 1)){
		//La rimuovo dalla lista
		rimuovi_carta_lista(i, lista_carte);
		//Cancello la carta attuale
		clear_carta(&i);
	}
}

Carta ricerca_carta_per_posizione(Carta lista_carte, int pos){
	int p;
	for(p = 1; is_empty_carta(lista_carte) != 1 && p < pos; p++)
		lista_carte = lista_carte->next;
	if(p == pos && is_empty_carta(lista_carte) == 0)
		return lista_carte;
	else
		return NULL;
}

int rimuovi_carta_lista(Carta my_carta, Carta* lista_carte){
	Carta q = *lista_carte;
	Carta p = *lista_carte;
	while(is_empty_carta(p) == 0 && my_carta != p){
		q = p;
		p = p->next;
	}
	if(p == my_carta){
		//Caso in testa
		if(p == *lista_carte){
			*lista_carte = p->next;
			return 1;
		}
		//Caso generico
		q->next = p->next;
		return 1;
	}
	else
		return 0;
}

void inserisci_in_lista(Carta* lista_carte, Carta my_carta){
	if(is_empty_carta(my_carta) == 1)
		return;
	my_carta->next = *lista_carte;
	(*lista_carte) = my_carta;
}

int sposta_lista_a_lista(Carta* lista_lancio, int pos, Carta* lista_arrivo){
	Carta temp = ricerca_carta_per_posizione(*lista_lancio, pos);
	if(is_empty_carta(temp) == 1)
		return 0;
	if(rimuovi_carta_lista(temp, lista_lancio) == 1){
		inserisci_in_lista(lista_arrivo, temp);
		return 1;
	}
	else
		return 0;
}

void get_seme(Carta my_carta, char seme[]){
	if(is_empty_carta(my_carta) == 0){
		switch(my_carta->seme){
			case Oro:	strcpy(seme," oro "); break;
			case Spade:	strcpy(seme,"spade"); break;
			case Mazze:	strcpy(seme,"mazze"); break;
			case Coppe:	strcpy(seme,"coppe"); break;
			default: 	strcpy(seme,"Error"); break;
		}
	}
}

int get_value(Carta my_carta){
	if(is_empty_carta(my_carta) == 1)
		return 0;
	else
		return my_carta->value;
}

int calcola_punti(Carta my_carta){
	if(is_empty_carta(my_carta) == 1)
		return 0;
	switch(my_carta->value){
		case 8:		return 2;
		case 9:		return 3;
		case 10:	return 4;
		case 3:		return 10;
		case 1:		return 11;
		default:	return 0;
	}
}

void stampa_carta(Carta my_carta){
	if(is_empty_carta(my_carta) == 0){
		char seme[5];
		get_seme(my_carta, seme);
		printf("%d %s\n", my_carta->value, seme);
	}
	else
		printf("Carta non valida\n");
}

void stampa_lista_carte(Carta lista_carte){
	if(is_empty_carta(lista_carte) == 0){
		for(int k = 1; lista_carte != NULL; lista_carte = lista_carte->next){
			printf("%d)\t", k++);
			stampa_carta(lista_carte);
		}
	}
	else
		printf("La lista di carte e' vuota\n");

}

Carta crea_lista_ordinata(){
	Carta lista_carte = crea_lista_carte();
	//Ciclo per i 4 diversi semi
        for(Semi j = 4; j > 0; j--){
                //Ciclo per i 10 valori
                for(int i = 10; i >= 1; i--){
                        //Alloco la carta
                        Carta my_carta = (Carta)malloc(sizeof(struct carta));
                        //Controllo sia andato tutto a buon fine
                        if(my_carta == NULL)
                                return NULL;
                        //Assegno valore i e seme j
                        my_carta->value = i;
                        my_carta->seme = j-1;
                        my_carta->next = NULL;
                        //Aggiungo la carta al lista_carte
                        inserisci_in_lista(&lista_carte, my_carta);
                }
        }
        //Restituisco il lista_carte
        return lista_carte;
}

Carta copia_carta(Carta my_carta){
	if(is_empty_carta(my_carta) == 1)
		return NULL;
	Carta new_carta = (Carta)malloc(sizeof(struct carta));
	new_carta->value = my_carta->value;
        new_carta->seme = my_carta->seme;
	return new_carta;
}
