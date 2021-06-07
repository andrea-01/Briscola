#ifndef CARTE_H
#define CARTE_H

typedef struct carta* Carta;
typedef enum{
                Oro, Spade, Mazze, Coppe
} Semi;

struct carta{
	int value;
	Semi seme;
	Carta next;
};

//Dice se la lista di carte e' vuota o meno
int is_empty_carta(Carta lista_carte);

//Crea una carta dato valore e seme passati
Carta crea_carta(int value, char seme[]);

//Crea una lista vuota
Carta crea_lista_carte();

//Dealloca la memoria della struct carta
void clear_carta(Carta* my_carta);

//Dealloca tutte le carte in una lista
void clear_lista_carte(Carta* lista_carte);

//Restituisce la carta in posizione pos della lista_carte se la trova, altrimenti NULL
Carta ricerca_carta_per_posizione(Carta lista_carte, int pos);

//Rimuove my_carta da lista_carte
int rimuovi_carta_lista(Carta my_carta, Carta* lista_carte);

//Effettua un inserimento in coda nella lista
void inserisci_in_lista(Carta* lista_carte, Carta my_carta);

//Effettua una ricerca_carta_per_posizione, il risultato lo passa alla rimuovi_carta_lista insieme
//a lista_lancio e poi effettua effettua l'inserimento in coda nella lista_arrivo
int sposta_lista_a_lista(Carta* lista_lancio, int pos, Carta* lista_arrivo);

//Modifica una stringa passata come parametro con il seme
void get_seme(Carta my_carta, char seme[]);

//Restituisce my_carta->value, oppue 0 se la carta è vuota
int get_value(Carta my_carta);

//Restituisce il punteggio di una carta
int calcola_punti(Carta my_carta);

//Stampa valore e seme della carta passata
void stampa_carta(Carta my_carta);

//Richiama la stampa_carta per ogni carta della lista passata
void stampa_lista_carte(Carta lista_carte);

//Crea una lista ordinata di tutte le carte
Carta crea_lista_ordinata();

//Crea una copia della carta e la restituisce
Carta copia_carta(Carta my_carta);

#endif
