#ifndef GAME_H
#define GAME_H

#include "carte.h"
#include "pilecarte.h"
#include "user.h"

typedef struct partita* Partita;

struct partita{
	Player user1;
	Player user2;
	int points1;
	int points2;
	Mazzo mazzo;
	Carta terra;
	Carta briscola;
};

//Dice se la partita e' vuota o meno
int is_empty_partita(Partita my_partita);

//Inizializza a NULL una partita
Partita crea_partita();

//Dati i due utenti in partita, inizializza una nuova partita
int inizializza_partita(Partita* my_partita, Player user1, Player user2);

//Cerca un file con i dati della partita precedente e lo assegna alla struct partita
int carica_partita(Partita* my_partita, Player elenco_utenti, int* turno, int* options);

//Salva su file lo stato della partita restituendo 1 se riesce, 2 se la partita e' conclusa, 0 in caso di errore
int salva_partita(Partita my_partita, int turno, int options);

//Pulisce la struttura dati deallocando la memoria
void clear_partita(Partita* my_partita);

//Stampa lo stato attuale della partita
void stampa_campo(Partita my_partita, int turno, int display);

//Esegue la mossa richiesta tentendo conto delle regole della briscola, restituendo 1 se riesce, 1 altrimenti
int esegui_mossa(Partita my_partita, int turno, int choice);

//Passa le carte dal tavolo a terra, stabilendo chi prende e chi gioca successivamente
int aggiorna_tavolo(Partita my_partita, int* turno);

//Copia lo username del giocatore con il punteggio maggiore in username e restituisce 1, se pareggio 2, se partita vuota 0
int get_winner(Partita my_partita, char username[], int* points);

//Somma ai punteggi complessivi degli utenti in partita, i punteggi ottenuti in partita
void update_points(Partita my_partita, Player* leaderboard);
#endif
