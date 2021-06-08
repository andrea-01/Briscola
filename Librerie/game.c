#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CODES
#define CODES
#define UP_LINE "\x1b[1A"
#define STANDARD_WHITE "\033[0m"
#define BOLD_WHITE "\033[1m"
#define BOLD_RED "\033[1;31m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_PURPLE "\033[1;35m"
#define BOLD_CYAN "\033[1;36m"

#endif

int is_empty_partita(Partita my_partita){
	if(my_partita == NULL)
		return 1;
	else
		return 0;
}

Partita crea_partita(){
	return NULL;
}

int inizializza_partita(Partita* my_partita, Player user1, Player user2){
	if(is_empty_partita(*my_partita) == 0)
		return 0;
	Partita temp_partita = (Partita)malloc(sizeof(struct partita));
	if(is_empty_partita(temp_partita) == 1)	return 0;
	temp_partita->user1 = user1;
	temp_partita->user2 = user2;
	temp_partita->points1 = 0;
	temp_partita->points2 = 0;
	Mazzo temp_mazzo = crea_mazzo_mischiato();
	temp_partita->mazzo = temp_mazzo;
	Carta temp_terra = crea_lista_carte();
	temp_partita->terra = temp_terra;

	//Ottiene la briscola
	Carta temp_briscola = ricerca_carta_per_posizione(temp_partita->mazzo, 40);

	//Crea una copia della briscola
	temp_partita->briscola = copia_carta(temp_briscola);

	//Do le carte
	Carta* mano1 = get_mano(temp_partita->user1);
	Carta* mano2 = get_mano(temp_partita->user2);
	sposta_pila_a_lista(&(temp_partita->mazzo), mano1);
        sposta_pila_a_lista((&(temp_partita)->mazzo), mano2);
        sposta_pila_a_lista(&(temp_partita->mazzo), mano1);
        sposta_pila_a_lista(&(temp_partita->mazzo), mano2);
        sposta_pila_a_lista(&(temp_partita->mazzo), mano1);
        sposta_pila_a_lista(&(temp_partita->mazzo), mano2);
	(*my_partita) = temp_partita;
	return 1;
}

int carica_partita(Partita* my_partita, Player elenco_utenti, int* turno, int* options){

	//Partita già inizializzata
	if(is_empty_partita(*my_partita) == 0)
		return 0;

	//Apro il file "Partita" in lettura
	FILE* f = fopen("Partita", "r");
	//Se non lo trovo restituisco 0
	if(f == NULL) return 0;

	//Il boolean load mi dice se il file va letto o meno
	int load;
	fscanf(f, "%d\n", &load);
	if(!load){
		fclose(f);
		return 0;
	}

	//Variabili d'appoggio
        Carta my_carta;
	int points1, points2, value;
	char username1[9], username2[9], seme[6];

	//Scnasione prima riga, contenente i campi della struct partita e alcune variabili passate alla funzione
	fscanf(f,"%s%s%d%d%d%d%d", username1, username2, &points1, &points2, turno, options, &value);
	//Levo lo spazio
	fgetc(f);
	//Scanciono 5 caratteri esatti per il seme, che puo contenere spazi
	fscanf(f, "%5c", seme);
	//Assegno il carattere di terminazione alla stringa seme
	seme[5] = '\0';
	//Levo il \n
	fgetc(f);

	//Debug per verificare la corretta lettura dei dati
/*	printf("\n\n\tUser 1: %s\n\tUser 2: %s\n\tTurno: %d\n\t\t\t\tPremi INVIO per continuare\n", username1, username2, *turno);
	scanf("[\n]");
	//Svuoto il buffer
	char ch;
	while((ch = getchar()) != EOF && ch != '\n');
*/

	//Cerco gli utenti nell'elenco e ne conservo i puntatori
	Player temp_user1 = ricerca_username(username1, elenco_utenti);
	//In caso di search miss, chiudo il file ed esco
	if(is_empty_user(temp_user1) == 1){
		fclose(f);
		return -1;
	}
	Player temp_user2 = ricerca_username(username2, elenco_utenti);
        if(is_empty_user(temp_user2) == 1){
		fclose(f);
                return -2;
	}

	//Alloco la struct partita
	(*my_partita) = (Partita)malloc(sizeof(struct partita));
        if(is_empty_partita(*my_partita) == 1){
		fclose(f);
         	return -3;
	}

	//Assegno le variabili lette ai campi della mia struttura dati
	(*my_partita)->user1 = temp_user1;
	(*my_partita)->user2 = temp_user2;
	(*my_partita)->points1 = points1;
	(*my_partita)->points2 = points2;

	//Inizializzo le liste e i mazzi
	(*my_partita)->mazzo = crea_pila_carte();
	(*my_partita)->terra = crea_lista_carte();

	//Inizializzo la mia copia della briscola
	(*my_partita)->briscola = crea_carta(value, seme);

	//Proseguo con la lettura riga per riga delle liste di carte
	char c;

	Mazzo temp_mazzo = crea_pila_carte();
	//Leggo fino al simbolo "a capo"
	while((c = fgetc(f)) != '\n'){
		//Ottengo il valore della carta
		fscanf(f, "%d", &value);
		//Levo lo spazio
		fgetc(f);
		//Ottengo il seme che puo contenere spazi
		fscanf(f, "%5c", seme);
		//Completo la stringa con il terminatore
		seme[5] = '\0';
		//Creo la carta letta
		my_carta = crea_carta(value, seme);
		//La inserisco nel mazzo
		push_carta(&temp_mazzo, my_carta);
	}
	my_carta = pop_carta(&temp_mazzo);
	while(is_empty_carta(my_carta) != 1){
		push_carta(&((*my_partita)->mazzo), my_carta);
		my_carta = pop_carta(&temp_mazzo);
	}

	//Ripeeto per la lista di carte a terra
	while((c = fgetc(f)) != '\n'){
		fscanf(f, "%d", &value);
		fgetc(f);
		fscanf(f, "%5c", seme);
		seme[5] = '\0';
                my_carta = crea_carta(value, seme);
                inserisci_in_lista(&((*my_partita)->terra), my_carta);
        }

	//Ripeto per la lista di carte nella mano di user1
	Carta* temp_lista = get_mano((*my_partita)->user1);
	while((c = fgetc(f)) != '\n'){
		fscanf(f, "%d", &value);
		fgetc(f);
		fscanf(f, "%5c", seme);
		seme[5] = '\0';
                my_carta = crea_carta(value, seme);
                inserisci_in_lista(temp_lista , my_carta);
        }

	//Ripeto per la lista di carte nella mano di user2
	temp_lista = get_mano((*my_partita)->user2);
	while((c = fgetc(f)) != '\n'){
		fscanf(f, "%d", &value);
		fgetc(f);
		fscanf(f, "%5c", seme);
		seme[5] = '\0';
                my_carta = crea_carta(value, seme);
                inserisci_in_lista(temp_lista, my_carta);
        }

	//Ripeto per la pila di carte prese da user1
	temp_lista = get_prese((*my_partita)->user1);
	while((c = fgetc(f)) != '\n'){
		fscanf(f, "%d", &value);
		fgetc(f);
		fscanf(f, "%5c", seme);
		seme[5] = '\0';
                my_carta = crea_carta(value, seme);
                inserisci_in_lista(temp_lista, my_carta);
        }

	//Ripeto per la pila di carte prese da user2
	temp_lista = get_prese((*my_partita)->user2);
	while((c = fgetc(f)) != '\n' && c != EOF){
		fscanf(f, "%d", &value);
		fgetc(f);
		fscanf(f, "%5c", seme);
		seme[5] = '\0';
                my_carta = crea_carta(value, seme);
                inserisci_in_lista(temp_lista, my_carta);
        }

	//Chiudo il file e restituisco 1
	fclose(f);
	return 1;
}

int salva_partita(Partita my_partita, int turno, int options){

	//Caso di partita vuota
	if(is_empty_partita(my_partita))
		return 0;

	//Partita completata
	if(is_empty_carta(my_partita->terra) == 1 && is_empty_mazzo(my_partita->mazzo) == 1 && is_empty_carta(*get_mano(my_partita->user1)) == 1 && is_empty_carta(*get_mano(my_partita->user2)) == 1 && my_partita->points1 + my_partita->points2 == 120){
		//Apro il file in scrittura
		FILE* f = fopen("Partita", "w");
		if(f == NULL)	return 0;
		//Scrivo solo 0, che rappresenta una partita vuota
		fprintf(f, "0\n");
		//Chiudo il file e restituisco 2
		fclose(f);
		return 2;
	}

	//Caricamento su file
	//Apro il file in lettura
	FILE* f = fopen("Partita", "w");
	if(f == NULL) return 0;

	//Variabili d'appoggio
	char username1[9], username2[9], seme[6];
	int value;
	Carta temp = my_partita->briscola;
	value = get_value(temp);
	get_seme(temp, seme);
	get_username(my_partita->user1, username1);
	get_username(my_partita->user2, username2);

	//Scrivo su file il valore 1, che rappresenta un file che contiene una partita, dopodiche' scrivo i campi della struttura
	fprintf(f, "1\n%s %s %d %d %d %d %d %s\n", username1, username2, my_partita->points1, my_partita->points2, turno, options, value, seme);

	//Ottengo il puntatore al mazzo
	temp = pop_carta(&(my_partita->mazzo));
	Mazzo temp_mazzo = crea_pila_carte();
	//Se il mazzo e' vuoto stampo una riga vuota
	if(is_empty_carta(temp) == 1)
		fputc('\n', f);
	//Altrimenti uno spazio
	else
		fputc(' ', f);
	//Ciclo tutto il mazzo fino a raggiungere la carta vuota
	while(is_empty_carta(temp) != 1){
		//Ottengo valore e seme della carta
		value = get_value(temp);
		get_seme(temp, seme);
		//Li stampo
		fprintf(f, "%d %s", value, seme);
		//Rimetto la carta in una pila d'appoggio
		push_carta(&temp_mazzo, temp);
		//Ottengo la carta successiva
		temp = pop_carta(&(my_partita->mazzo));
		//Se esiste stampo uno spazio, altrimenti vado a capo
		if(is_empty_carta(temp) == 0)
			fputc(' ', f);
		else
			fputc('\n', f);
	}
	//Finito di svuotare il mazzo, ricompongo la pila originale
	temp = pop_carta(&temp_mazzo);
	while(is_empty_carta(temp) != 1){
		push_carta(&(my_partita->mazzo), temp);
		temp = pop_carta(&temp_mazzo);
	}

	//Per le liste di carte posso effettuare ricerche per posizione successive
	temp = my_partita->terra;
	if(is_empty_carta(temp) == 1)
                fputc('\n', f);
	else
                fputc(' ', f);
	while(is_empty_carta(temp) != 1){
		value = get_value(temp);
                get_seme(temp, seme);
                fprintf(f, "%d %s", value, seme);
                temp = ricerca_carta_per_posizione(temp, 2);
		if(is_empty_carta(temp) == 0)
                        fputc(' ', f);
                else
                        fputc('\n', f);
	}

	//Ripeto per la mano di user1
	temp = *get_mano(my_partita->user1);
	if(is_empty_carta(temp) == 1)
                fputc('\n', f);
	else
                fputc(' ', f);
	while(is_empty_carta(temp) != 1){
		value = get_value(temp);
                get_seme(temp, seme);
                fprintf(f, "%d %s", value, seme);
                temp = ricerca_carta_per_posizione(temp, 2);
		if(is_empty_carta(temp) == 0)
                        fputc(' ', f);
                else
                        fputc('\n', f);
	}

	//Ripeto per la mano di user2
        temp = *get_mano(my_partita->user2);
	if(is_empty_carta(temp) == 1)
                fputc('\n', f);
	else
                fputc(' ', f);
        while(is_empty_carta(temp) != 1){
                value = get_value(temp);
                get_seme(temp, seme);
                fprintf(f, "%d %s", value, seme);
                temp = ricerca_carta_per_posizione(temp, 2);
		if(is_empty_carta(temp) == 0)
                        fputc(' ', f);
                else
                        fputc('\n', f);
        }

	//Ripeto per le prese di user1
        temp = *get_prese(my_partita->user1);
	if(is_empty_carta(temp) == 1)
                fputc('\n', f);
	else
                fputc(' ', f);
        while(is_empty_carta(temp) != 1){
                value = get_value(temp);
                get_seme(temp, seme);
                fprintf(f, "%d %s", value, seme);
                temp = ricerca_carta_per_posizione(temp, 2);
		if(is_empty_carta(temp) == 0)
                        fputc(' ', f);
                else
                        fputc('\n', f);
        }

	//Ripeto per le prese di user2
        temp = *get_prese(my_partita->user2);
	if(is_empty_carta(temp) == 1)
                fputc('\n', f);
	else
                fputc(' ', f);
        while(is_empty_carta(temp) != 1){
                value = get_value(temp);
                get_seme(temp, seme);
                fprintf(f, "%d %s", value, seme);
                temp = ricerca_carta_per_posizione(temp, 2);
		if(is_empty_carta(temp) == 0)
                        fputc(' ', f);
                else
                        fputc('\n', f);
        }

	//Chiudo il file e restituisco 1
	fclose(f);
	return 1;
}

void clear_partita(Partita* my_partita){
	//Se gia' vuota non faccio nulla
	if(is_empty_partita(*my_partita) == 1)
		return;
	//Pulisco la pila mazzo
	clear_pila_carte(&((*my_partita)->mazzo));
	//Pulisco la pila terra
	clear_lista_carte(&((*my_partita)->terra));
	//Ottengo la mano degli utenti e pulisco la lista
	Carta* temp = get_mano((*my_partita)->user1);
	clear_lista_carte(temp);
	temp = get_mano((*my_partita)->user2);
	clear_lista_carte(temp);
	//Ottengo le prese degli utenti e pulisco la lista
	temp = get_prese((*my_partita)->user1);
        clear_pila_carte(temp);
	temp = get_prese((*my_partita)->user2);
        clear_pila_carte(temp);
	//Pulisco la carta briscola
	clear_carta(&((*my_partita)->briscola));
	//Dealloco la memoria della struct partita
	free(*my_partita);
	//Ne azzero il puntatore
	(*my_partita) = NULL;
}

void stampa_campo(Partita my_partita, int turno, int display){
	system("@cls||clear");
	if(is_empty_partita(my_partita) == 1) return;
	//Numero carte
	int mano_opp = 0;
	int mano_mia = 0;
	int terra = 0;

	Carta scorri;
	//Ottengo il numero di carte in mano dell'avversario
	if(turno == 1)
		scorri = *get_mano(my_partita->user2);
	else
		scorri = *get_mano(my_partita->user1);
	for(; is_empty_carta(scorri) != 1; scorri = ricerca_carta_per_posizione(scorri, 2))
		mano_opp++;

	//Ottengo il numero di carte nella mia mano
	if(turno == 1)
                scorri = *get_mano(my_partita->user1);
        else
                scorri = *get_mano(my_partita->user2);
        for(; is_empty_carta(scorri) != 1; scorri = ricerca_carta_per_posizione(scorri, 2))
                mano_mia++;

	//Ottengo il numero di carte a terra
	for(scorri = my_partita->terra; is_empty_carta(scorri) != 1; scorri = ricerca_carta_per_posizione(scorri, 2))
		terra++;

/*			  Prototipo Interfaccia (80x24)
Turno di andrea01	    |_-_-_|  |_-_-_|  |_-_-_|
			    |-_-_-|  |-_-_-|  |-_-_-|
			    |_-_-_|  |_-_-_|  |_-_-_|
			    |_____|  |_____|  |_____|


	   |Mazzo|			    |Tavolo|
	 _____ _____ 			 _____    _____
	|_-_-_|     |			|     |  |     |
	|-_-_-|  3  |			|  4  |  |  10 |
	|_-_-_|spade|			| oro |  | oro |
	|_____|_____|			|_____|  |_____|



				     |Mano|
				 _____    _____
				|     |  |     |
				|  6  |  |  1  |
				|mazze|  | oro |
				|_____|  |_____|
				  (1)      (2)
	Tira:
							  Inserisci 0 per uscire
*/

	char username[9];
	if(turno == 1)
		get_username(my_partita->user1, username);
	else
		get_username(my_partita->user2, username);
	printf(BOLD_WHITE"Turno di "STANDARD_WHITE BOLD_PURPLE"%s"STANDARD_WHITE"\r", username);


	switch (mano_opp){
		case 0:
			printf("\n\n\n\n\n\n");
			break;
		case 1:
			printf(BOLD_BLUE"\t\t\t\t    |_-_-_|\n\t\t\t\t    |-_-_-|\n\t\t\t\t    |_-_-_|\n\t\t\t\t    |_____|\n\n\n"STANDARD_WHITE);
			break;
		case 2:
			printf(BOLD_BLUE"\t\t\t\t |_-_-_|  |_-_-_|\n\t\t\t\t |-_-_-|  |-_-_-|\n\t\t\t\t |_-_-_|  |_-_-_|\n\t\t\t\t |_____|  |_____|\n\n\n"STANDARD_WHITE);
			break;
		case 3:
			printf(BOLD_BLUE"\t\t\t    |_-_-_|  |_-_-_|  |_-_-_|\n\t\t\t    |-_-_-|  |-_-_-|  |-_-_-|\n\t\t\t    |_-_-_|  |_-_-_|  |_-_-_|\n\t\t\t    |_____|  |_____|  |_____|\n\n\n"STANDARD_WHITE);
			break;
		default:
			printf(BOLD_PURPLE"\n\n\n\t\t\t    ERRORE NEL CONTARE MANO_OPP\n\n\n\n"STANDARD_WHITE);
			break;
	}
	printf(BOLD_YELLOW"\t   |Mazzo|"STANDARD_WHITE"\t\t\t    "BOLD_YELLOW"|Tavolo|\r"STANDARD_WHITE);
	char briscola[6];
	char carta1[6];
	char carta2[6];
	char carta3[6];
	int value1, value2, value3;
	get_seme(my_partita->briscola, briscola);
	int value_briscola = get_value(my_partita->briscola);
	switch(terra){
		case(0):
			if(is_empty_carta(my_partita->mazzo) == 0)
				printf(BOLD_BLUE"\n\t _____ _____ \n\t|_-_-_|     |\n\t|-_-_-|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\b\b\b\b|\n\t|_-_-_|"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t|_____|_____|\n\n"STANDARD_WHITE, value_briscola, briscola);
			else
				printf("\n\n\n\n\n\n\n");
			break;
		case(1):
			value1 = get_value(my_partita->terra);
			get_seme(my_partita->terra, carta1);
			if(is_empty_carta(my_partita->mazzo) == 0){
				printf("\n\t "BOLD_BLUE"_____ _____ \t\t\t _____ \n\t|_-_-_|     |\t\t\t|     |\n\t|-_-_-|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\b\b\b\b|\t\t\t|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b|\n\t|_-_-_|"STANDARD_WHITE"%s"BOLD_BLUE"|\t\t\t|"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t|_____|_____|\t\t\t|_____|\n\n"STANDARD_WHITE, value_briscola, value1, briscola, carta1);
			}
			else{
				printf(BOLD_BLUE"\n\t\t\t\t\t _____ \n\t\t\t\t\t|     |\n\t\t\t\t\t|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b|\n\t\t\t\t\t|"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t\t\t|_____|\n\n"STANDARD_WHITE, value1, carta1);
			}
			break;
		case(2):
			get_seme(my_partita->terra, carta1);
			get_seme(ricerca_carta_per_posizione(my_partita->terra, 2), carta2);
			value1 = get_value(my_partita->terra);
			value2 = get_value(ricerca_carta_per_posizione(my_partita->terra, 2));
			if(is_empty_carta(my_partita->mazzo) == 0){
				printf(BOLD_BLUE"\n\t _____ _____ \t\t\t _____    _____ \n\t|_-_-_|     |\t\t\t|     |  |     |\n\t|-_-_-|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\b\b\b\b|\t\t\t|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\t\b|\n\t|_-_-_|"STANDARD_WHITE"%s"BOLD_BLUE"|\t\t\t|"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t|_____|_____|\t\t\t|_____|  |_____|\n\n"STANDARD_WHITE, value_briscola, value2, value1, briscola, carta2, carta1);
			}
			else{
				printf(BOLD_BLUE"\n\t\t\t\t\t _____    _____ \n\t\t\t\t\t|     |  |     |\n\t\t\t\t\t|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\t\b|\n\t\t\t\t\t|"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t\t\t|_____|  |_____|\n\n"STANDARD_WHITE, value2, value1, carta2, carta1);
			}
			break;
		default:
			printf(BOLD_PURPLE"\n\t\t\tERRORE NEL CONTARE TERRA\n\n\n\n"STANDARD_WHITE);
			break;
	}

	printf(BOLD_YELLOW"\n\n\t\t\t\t     |Mano|\r"STANDARD_WHITE);

	if(display){
	    switch (mano_mia){
		    case 0:
			    printf("\n\n\n\n\n\n\n");
			    break;
		    case 1:
			    if(turno == 1){
				    get_seme(*get_mano(my_partita->user1), carta1);
				    value1 = get_value(*get_mano(my_partita->user1));
				    printf(BOLD_BLUE"\n\t\t\t\t      _____\n\t\t\t\t     |     |\n\t\t\t\t     |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b\b\b\b|\n\t\t\t\t     |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t\t     |_____|\n\t\t\t\t"STANDARD_WHITE BOLD_PURPLE"       (1)\n"STANDARD_WHITE, value1, carta1);
			    }
			    else{
				    get_seme(*get_mano(my_partita->user2), carta1);
				    value1 = get_value(*get_mano(my_partita->user2));
				    printf(BOLD_BLUE"\n\t\t\t\t      _____\n\t\t\t\t     |     |\n\t\t\t\t     |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b\b\b\b|\n\t\t\t\t     |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t\t     |_____|\n\t\t\t\t"STANDARD_WHITE BOLD_PURPLE"       (1)\n"STANDARD_WHITE, value1, carta1);
			    }
			    break;
		    case 2:
			    if(turno == 1){
				    get_seme(*get_mano(my_partita->user1), carta1);
				    get_seme(ricerca_carta_per_posizione(*get_mano(my_partita->user1), 2), carta2);
				    value1 = get_value(*get_mano(my_partita->user1));
				    value2 = get_value(ricerca_carta_per_posizione(*get_mano(my_partita->user1), 2));
				    printf(BOLD_BLUE"\n\t\t\t\t _____    _____ \n\t\t\t\t|     |  |     |\n\t\t\t\t|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b|\n\t\t\t\t|"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t\t|_____|  |_____|\n\t\t\t\t"STANDARD_WHITE BOLD_PURPLE"  (1)      (2)\n"STANDARD_WHITE, value1, value2, carta1, carta2);
			    }
			    else{
				    get_seme(*get_mano(my_partita->user2), carta1);
				    get_seme(ricerca_carta_per_posizione(*get_mano(my_partita->user2), 2), carta2);
				    value1 = get_value(*get_mano(my_partita->user2));
				    value2 = get_value(ricerca_carta_per_posizione(*get_mano(my_partita->user2), 2));
				    printf(BOLD_BLUE"\n\t\t\t\t _____    _____ \n\t\t\t\t|     |  |     |\n\t\t\t\t|  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b|\n\t\t\t\t|"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t\t|_____|  |_____|\n\t\t\t\t"STANDARD_WHITE BOLD_PURPLE"  (1)      (2)\n"STANDARD_WHITE, value1, value2, carta1, carta2);
			    }
			    break;
		    case 3:
			    if(turno == 1){
				    get_seme(*get_mano(my_partita->user1), carta1);
				    get_seme(ricerca_carta_per_posizione(*get_mano(my_partita->user1), 2), carta2);
				    get_seme(ricerca_carta_per_posizione(*get_mano(my_partita->user1), 3), carta3);
				    value1 = get_value(*get_mano(my_partita->user1));
                                    value2 = get_value(ricerca_carta_per_posizione(*get_mano(my_partita->user1), 2));
                                    value3 = get_value(ricerca_carta_per_posizione(*get_mano(my_partita->user1), 3));
				    printf(BOLD_BLUE"\n\t\t\t     _____    _____    _____ \n\t\t\t    |     |  |     |  |     |\n\t\t\t    |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\b\b\b\b\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b\b\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\t\b\b\b\b|\n\t\t\t    |"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t    |_____|  |_____|  |_____|\n\t\t\t"STANDARD_WHITE BOLD_PURPLE"      (1)      (2)      (3)\n"STANDARD_WHITE, value1, value2, value3, carta1, carta2, carta3);
			    }
			    else{
				    get_seme(*get_mano(my_partita->user2), carta1);
                                    get_seme(ricerca_carta_per_posizione(*get_mano(my_partita->user2), 2), carta2);
                                    get_seme(ricerca_carta_per_posizione(*get_mano(my_partita->user2), 3), carta3);
                                    value1 = get_value(*get_mano(my_partita->user2));
                                    value2 = get_value(ricerca_carta_per_posizione(*get_mano(my_partita->user2), 2));
                                    value3 = get_value(ricerca_carta_per_posizione(*get_mano(my_partita->user2), 3));
				    printf(BOLD_BLUE"\n\t\t\t     _____    _____    _____ \n\t\t\t    |     |  |     |  |     |\n\t\t\t    |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\b\b\b\b\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\b\b\b\b\b|  |  "STANDARD_WHITE"%d"BOLD_BLUE"\r\t\t\t\t\t\t\t\b\b\b\b|\n\t\t\t    |"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|  |"STANDARD_WHITE"%s"BOLD_BLUE"|\n\t\t\t    |_____|  |_____|  |_____|\n\t\t\t"STANDARD_WHITE BOLD_PURPLE"      (1)      (2)      (3)\n"STANDARD_WHITE, value1, value2, value3, carta1, carta2, carta3);
			    }
			    break;
		    default:
			    printf(BOLD_PURPLE"\n\n\n\t\t\tERRORE NEL CONTARE MANO\n\n\n\n"STANDARD_WHITE);
			    break;
	    }
	}
    else{
        switch (mano_mia){
		    case 0:
			    printf("\n\n\n\n\n\n\n");
			    break;
		    case 1:
			    printf(BOLD_BLUE"\n\t\t\t\t     _____ \n\t\t\t\t    |_-_-_|\n\t\t\t\t    |-_-_-|\n\t\t\t\t    |_-_-_|\n\t\t\t\t    |_____|\n\n"STANDARD_WHITE);
			    break;
		    case 2:
			    printf(BOLD_BLUE"\n\t\t\t\t  _____    _____ \n\t\t\t\t |_-_-_|  |_-_-_|\n\t\t\t\t |-_-_-|  |-_-_-|\n\t\t\t\t |_-_-_|  |_-_-_|\n\t\t\t\t |_____|  |_____|\n\n"STANDARD_WHITE);
			    break;
		    case 3:
			    printf(BOLD_BLUE"\n\t\t\t     _____    _____    _____ \n\t\t\t    |_-_-_|  |_-_-_|  |_-_-_|\n\t\t\t    |-_-_-|  |-_-_-|  |-_-_-|\n\t\t\t    |_-_-_|  |_-_-_|  |_-_-_|\n\t\t\t    |_____|  |_____|  |_____|\n\n"STANDARD_WHITE);
			    break;
		    default:
			    printf(BOLD_PURPLE"\n\n\n\t\t\tERRORE NEL CONTARE MANO\n\n\n\n"STANDARD_WHITE);
			    break;
	    }
    }
}

int esegui_mossa(Partita my_partita, int turno, int choice){
	if(is_empty_partita(my_partita) == 1 || (is_empty_carta(my_partita->terra) == 1 && is_empty_mazzo(my_partita->mazzo) == 1 && is_empty_carta(*get_mano(my_partita->user1)) == 1 && is_empty_carta(*get_mano(my_partita->user2)) == 1 && my_partita->points1 + my_partita->points2 == 120))
		return 0;
	Carta* mano;
	if(turno == 1)
		mano = get_mano(my_partita->user1);
	else
		mano = get_mano(my_partita->user2);
	sposta_lista_a_lista(mano, choice, &(my_partita->terra));
	return 1;
}

int aggiorna_tavolo(Partita my_partita, int* turno){
	if(is_empty_partita(my_partita) == 1)
		return 0;

	int opt1 = 0, opt2 = 0;
	for(Carta temp = *get_mano(my_partita->user1); is_empty_carta(temp) != 1; temp = ricerca_carta_per_posizione(temp, 2))
		opt1++;
	for(Carta temp = *get_mano(my_partita->user2); is_empty_carta(temp) != 1; temp = ricerca_carta_per_posizione(temp, 2))
                opt2++;

	if(is_empty_carta(my_partita->terra) == 1)
                return ((*turno) == 1)?opt1:opt2;

	if(is_empty_carta(ricerca_carta_per_posizione(my_partita->terra, 2))){
		(*turno) = (*turno)%2 + 1;
		return ((*turno) == 1)?opt1:opt2;
	}

	Carta temp = ricerca_carta_per_posizione(my_partita->terra, 2);
	int value2 = get_value(my_partita->terra);
	int value1 = get_value(temp);
	int weight2 = calcola_punti(my_partita->terra);
	int weight1 = calcola_punti(temp);
	char seme1[6], seme2[6], briscola[6];
	get_seme(my_partita->terra, seme2);
	get_seme(temp, seme1);
	get_seme(my_partita->briscola, briscola);

	Carta* prese1 = get_prese(my_partita->user1);
	Carta* prese2 = get_prese(my_partita->user2);



	//Stesso seme e stesso valore della carta (liscio)
	if(strcmp(seme1, seme2) == 0 && weight1 == weight2){
		//La presa dipende dal numero della carta
		if(value2 > value1){
			if((*turno) == 1)
				my_partita->points1 += weight1 + weight2;
			else
				my_partita->points2 += weight1 + weight2;
			sposta_lista_a_pila(&(my_partita->terra), 2, ((*turno) == 1)?prese1:prese2);
			sposta_lista_a_pila(&(my_partita->terra), 1, ((*turno) == 1)?prese1:prese2);
		}
		else{
			if((*turno) == 1)
                                my_partita->points2 += weight1 + weight2;
                        else
                                my_partita->points1 += weight1 + weight2;
			sposta_lista_a_pila(&(my_partita->terra), 2, ((*turno) == 1)?prese2:prese1);
                        sposta_lista_a_pila(&(my_partita->terra), 1, ((*turno) == 1)?prese2:prese1);
                        (*turno) = (*turno)%2 + 1;
		}
	}
	//Caso di carte dello stesso seme, con valore diverso
	else if(strcmp(seme1, seme2) == 0){
		//La presa dipende dal punteggio della carta
		if(weight2 > weight1){
			if((*turno) == 1)
                                my_partita->points1 += weight1 + weight2;
                        else
                                my_partita->points2 += weight1 + weight2;
			sposta_lista_a_pila(&(my_partita->terra), 2, ((*turno) == 1)?prese1:prese2);
                        sposta_lista_a_pila(&(my_partita->terra), 1, ((*turno) == 1)?prese1:prese2);
		}
		else{
			if((*turno) == 1)
                                my_partita->points2 += weight1 + weight2;
                        else
                                my_partita->points1 += weight1 + weight2;
                        sposta_lista_a_pila(&(my_partita->terra), 2, ((*turno) == 1)?prese2:prese1);
                        sposta_lista_a_pila(&(my_partita->terra), 1, ((*turno) == 1)?prese2:prese1);
                        (*turno) = (*turno)%2 + 1;
		}
	}
	//Caso di carte con seme diverso che non sia la briscola
	else if(strcmp(seme1, seme2) != 0 && strcmp(seme1, briscola) != 0 && strcmp(seme2, briscola) != 0){
		if((*turno) == 1)
	                my_partita->points2 += weight1 + weight2;
		else
			my_partita->points1 += weight1 + weight2;

       		sposta_lista_a_pila(&(my_partita->terra), 2, ((*turno) == 1)?prese2:prese1);
                sposta_lista_a_pila(&(my_partita->terra), 1, ((*turno) == 1)?prese2:prese1);
	        (*turno) = (*turno)%2 + 1;
	}
	else if(strcmp(seme1, briscola) == 0){
		if((*turno) == 1)
                        my_partita->points2 += weight1 + weight2;
                else
                        my_partita->points1 += weight1 + weight2;

                sposta_lista_a_pila(&(my_partita->terra), 2, ((*turno) == 1)?prese2:prese1);
                sposta_lista_a_pila(&(my_partita->terra), 1, ((*turno) == 1)?prese2:prese1);
                (*turno) = (*turno)%2 +1;
	}
	else if(strcmp(seme2, briscola) == 0){
		if((*turno) == 1)
                        my_partita->points1 += weight1 + weight2;
                else
                        my_partita->points2 += weight1 + weight2;

                sposta_lista_a_pila(&(my_partita->terra), 2, ((*turno) == 1)?prese1:prese2);
                sposta_lista_a_pila(&(my_partita->terra), 1, ((*turno) == 1)?prese1:prese2);
	}
	else{
		printf("\n\n\t\tErrore: premi INVIO\n\n");
		scanf("[\n]");
	}

	Carta* mano1 = get_mano(my_partita->user1);
	Carta* mano2 = get_mano(my_partita->user2);
	if(is_empty_carta(my_partita->mazzo) == 0){
		sposta_pila_a_lista(&(my_partita->mazzo), ((*turno) == 1)?mano1:mano2);
		sposta_pila_a_lista(&(my_partita->mazzo), ((*turno) == 1)?mano2:mano1);
		opt1++;
		opt2++;
	}
	return ((*turno) == 1)?opt1:opt2;
}

int get_winner(Partita my_partita, char username[], int* points){
	//Errore
	if(is_empty_partita(my_partita) == 1)
		return 0;
	//Vince user1
	if(my_partita->points1 > my_partita->points2){
		strcpy(username, my_partita->user1->username);
        (*points) = my_partita->points1;
		return 1;
	}
	//Vince user2
	else if(my_partita->points1 < my_partita->points2){
		strcpy(username, my_partita->user2->username);
        (*points) = my_partita->points2;
		return 1;
	}
	//Pareggio
	else
		return 2;
}

void update_points(Partita my_partita, Player* leaderboard){
	if(is_empty_partita(my_partita) == 1)
		return;
	int* punteggio1 = get_punteggio(my_partita->user1);
	int* punteggio2 = get_punteggio(my_partita->user2);
	(*punteggio1) += my_partita->points1;
	(*punteggio2) += my_partita->points2;
	aggiorna_utente_leaderboard(my_partita->user1, leaderboard);
	aggiorna_utente_leaderboard(my_partita->user2, leaderboard);
}
