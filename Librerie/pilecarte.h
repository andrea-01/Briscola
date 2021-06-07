#ifndef PILECARTE_H
#define PILECARTE_H
#include "carte.h"

typedef Carta Mazzo;

//Restituisce 1 se il mazzo e' NULL, 0 altrimenti
int is_empty_mazzo(Mazzo pila_carte);

//Restituisce NULL, la pila vuota
Mazzo crea_pila_carte();

//Aggiunge una carta ad una pila di carte (in cima)
void push_carta(Mazzo* pila_carte, Carta my_carta);

//Rimuove una carta da una pila di carte (dalla cima), restituendola
Carta pop_carta(Mazzo* pila_carte);

//Pulisce una struttura pila di carte
void clear_pila_carte(Mazzo* pila_carte);

//Effettua una pop_carta dalla pila lancio e una push_carta nella pila arrivo
int sposta_pila_a_pila(Mazzo* pila_lancio, Mazzo* pila_arrivo);

//Effettua una pop dalla pila lancio e un inserimento in coda nella lista_arrivo
int sposta_pila_a_lista(Mazzo* pila_lancio, Carta* lista_arrivo);

//Effettua una rimuovi_carta_lista dalla lista di lancio in posizione pos ed una push nella pila arrivo
int sposta_lista_a_pila(Carta* lista_lancio, int pos, Mazzo* pila_arrivo);

//Crea un mazzo (impleementato come pila
Mazzo crea_mazzo_mischiato();

#endif
