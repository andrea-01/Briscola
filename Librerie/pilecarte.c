#include "pilecarte.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int is_empty_mazzo(Mazzo pila_carte){
	if(pila_carte == NULL)
                return 1;
        else
                return 0;
}

Mazzo crea_pila_carte(){
	return NULL;
}

void push_carta(Mazzo* pila_carte, Carta my_carta){
	if(my_carta != NULL){
		my_carta->next = (*pila_carte);
		(*pila_carte) = my_carta;
	}
}

Carta pop_carta(Mazzo* pila_carte){
	if(is_empty_carta(*pila_carte) == 1)
		return NULL;
	Carta temp;
	temp = *pila_carte;
	*pila_carte = (*pila_carte)->next;
	return temp;
}

void clear_pila_carte(Mazzo* pila_carte){
	if(is_empty_carta(*pila_carte) == 1) return;
	for(Carta i = pop_carta(pila_carte); is_empty_carta(i) == 0; i = pop_carta(pila_carte)){
		//Cancello la carta attuale
		clear_carta(&i);
	}
}

int sposta_pila_a_pila(Mazzo* pila_lancio, Mazzo* pila_arrivo){
	Carta temp = pop_carta(pila_lancio);
	if(is_empty_carta(temp) == 1)
		return 0;
	else
		push_carta(pila_arrivo, temp);
	return 1;
}

int sposta_pila_a_lista(Mazzo* pila_lancio, Mazzo* lista_arrivo){
	Carta temp = pop_carta(pila_lancio);
	if(is_empty_carta(temp) == 1)
		return 0;
	else
		inserisci_in_lista(lista_arrivo, temp);
	return 0;
}

int sposta_lista_a_pila(Carta* lista_lancio, int pos, Mazzo* pila_arrivo){
	Carta temp = ricerca_carta_per_posizione(*lista_lancio, pos);
	if(is_empty_carta(temp) == 1)
		return 0;
	if(rimuovi_carta_lista(temp, lista_lancio) == 1){
		push_carta(pila_arrivo, temp);
		return 1;
	}
	else
		return 0;
}

Mazzo crea_mazzo_mischiato(){
	//Creo la pila mazzo e la lista ordinata
	Mazzo mazzo = crea_pila_carte();
	Carta lista_carte = crea_lista_ordinata();
	srand(time(NULL));
	for(int k = 40; k > 0; k--){
		int j = rand()%k + 1;
		Carta r = ricerca_carta_per_posizione(lista_carte, j);
		rimuovi_carta_lista(r, &lista_carte);
		push_carta(&mazzo, r);
	}
	if(!is_empty_carta(lista_carte)) printf("\n\tErrore\n");
	return mazzo;
}
