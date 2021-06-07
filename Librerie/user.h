#ifndef USER_H
#define USER_H
#include "carte.h"
#include "pilecarte.h"

typedef struct utente* Player;

struct utente{
	char username[9];
	char password[9];
	int punteggio;
	int admin;
	Carta mano;
	Carta prese;
	Player next;
	Player next_l;
};

//Ci dice se la lista e' vuota
int is_empty_user(Player lista_utenti);

//Crea una nuova struct utente con punteggio nullo e username richiesto
Player crea_user(char username[], char password[], int admin);

//Restituisce un puntatore a NULL
Player crea_lista_utenti();

//Dealloca la memoria di una struct utente
void clear_user(Player* my_utente);

//Dealloca tutti gli utenti delle liste elenco e leaderboard
void clear_lista_utenti(Player* lista_utenti, Player* leaderboard);

//Inserisce ordinatamente per nome l'utente nella lista passata
int inserisci_utente_elenco(Player my_utente, Player* lista_utenti);

//Inserisce ordinatamente per punteggio l'utente nella lista passata
int inserisci_utente_leaderboard(Player my_utente, Player* leaderboard);

//Rimuove un dato utente da un elenco ordinato rispetto a next
int rimuovi_utente_elenco(Player my_utente, Player* lista_utenti);

//Rimuove un dato utente da un elenco ordinato rispetto a next_l
int rimuovi_utente_leaderboard(Player my_utente, Player* leaderboard);

//Efffettua la rimozione di un utente e il suo inserimento ordinato, nel caso sia stato modificato
int aggiorna_utente_elenco(Player my_utente, Player* lista_utenti);

//Effettua la rimozione e l'inserimento ordnato di un utente che ha accumulato punteggio
int aggiorna_utente_leaderboard(Player my_utente, Player* leaderboard);

//Ricerca un utente in base allo username e ne restituisce il puntatore
Player ricerca_username(char username[], Player lista_utenti);

//Stampa username e punteggio di un utente dato il suo puntatore
void stampa_utente(Player my_utente);

//Stampa un elenco di tutti gli utenti con relativi punteggi
void stampa_elenco_utenti(Player lista_utenti);

//Stampa una classifica ordinata per punteggio
void stampa_leaderboard(Player leaderboard);

//Modifica lo username di un utente dato il suo puntatore
int modifica_username(Player my_utente, char new_username[]);

//Modifica la password di un utente dato il suo puntatore
int modifica_password(Player my_utente, char new_password[]);

//Salva su file le liste elenco_utenti e leaderboard
int salva_elenchi(Player lista_utenti, Player leaderboard);

//Carica da un file, se lo trova (restituendo 1, altrimetni 0), le liste elenco_utenti e leaderboard di iterazioni precedenti
int carica_elenchi(Player* lista_utenti, Player* leaderboard);

//Restituisce il puntatore alla carta in posizione pos della mano di un utente
Carta* get_mano(Player my_utente);

//Restituisce il puntatore alle prese di un utente
Carta* get_prese(Player my_utente);

//Restituisce il puntatore al punteggio dell'utente
int* get_punteggio(Player my_utente);

//Copia in password la password dell'utente, restituendo 1 se presente, 0 altrimenti
int get_password(Player my_utente, char password[]);

//Copia in username lo username dell'utente, restituendo 1 se presente, 0 altrimenti
int get_username(Player my_utente, char username[]);

//Confronta la password dell'utente con la password passata e restituisce 1 se coincidono, 0 altrimenti
int login(Player my_utente, char password[]);

//Restituisce il valore del campo admin
int is_admin(Player my_utente);

//Rende admin 1, se gia' non lo e', restituisce 1 se lo cambia, 0 se l'utente non esiste o e' gia' admin
int make_admin(Player my_utente);

#endif
