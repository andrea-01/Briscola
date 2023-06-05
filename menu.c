//Includo tutte le librerie necessarie
#include "Librerie/user.h"
#include "Librerie/carte.h"
#include "Librerie/pilecarte.h"
#include "Librerie/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Definisco i colori ASCII, se non gia' definiti in altre librerie
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

//Funzione apposita per l'inserimento di username secondo la formattazione specifica del programma
void inserimento_da_tastiera(char username[]){
	char ch;
	scanf("%8s", username);
	while((ch=getchar()) != '\n' && ch != EOF);
}

//Funzione che attende l'inserimento di un INVIO prima di andare avanti
void stop(){

	char ch;
	scanf("[\n]");
	//Svuoto il buffer
	while((ch=getchar()) != '\n' && ch != EOF);
}

int main(){

	int choice = 0;	//Scelta della voce del menu
	int menu = 1;	//Indica in che menu ci si trova
	char ch;	//Per svuotare il buffer
	int logged = 0;
	int logged1 = 0;	//Stato log in temp_user
	int logged2 = 0;	//Stato log in temp_user2
	int tries = 0;	//Contatore tentativi di log in
	char temp_username[9];	//Array in cui salvare gli username degli utenti (di massimo 8 caratteri + \0)
	char temp_username2[9];
	char temp_password[9];	//Array in cui salvare le password inserite
	int admin = 0;	//Boolean che tiene conto se si possiedono i permessi di amministratore o meno
	int options;	//Indica il numero di opzioni selezionabili in un menù
	int result;	//Conserva i valori di ritorno delle funzioni
	int turno = 1;	//Tiene conto del turno in gioco
	Player temp_user, temp_user2, temp_user3;
	Partita partita = crea_partita();
	Player leaderboard = crea_lista_utenti();
	Player elenco_utenti = crea_lista_utenti();

	//Svuoto lo schermo
	system("@cls||clear");
	//Stampo istruzioni per ridimensionare la finestra
	printf(".------------------------------------------------------------------------------.\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t    Ridimensiona lo schermo\t\t\t       |\n|\t\t\t      allineando i bordi\t\t\t       |\n|\t\t\t(o imposta il terminale a 80x24)\t\t       |\n|\t\t\t    Al termine premere "BOLD_YELLOW"INVIO"STANDARD_WHITE"\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n|\t\t\t\t\t\t\t\t\t       |\n.------------------------------------------------------------------------------.\r");
	//Richiedo un INVIO per andare avanti
	stop();

	//Carico gli elenchi utenti da file
	carica_elenchi(&elenco_utenti, &leaderboard);

	//Per aggiornare il menu utilizzo un ciclo, da cui si uscirà solo con l'inserimento dello 0 nella variabile choice
	do{
		system("@cls||clear");

		//Il titolo e' mostrato solo sui tre menu principali, non nelle finestre delle funzioni
		if(menu <= 3 || menu == 10){
			//Stampo titolo
			printf(BOLD_YELLOW);
			printf("\n\n\t ____________________________________________________________\n");
			printf("\t|  ______ ______  _____  _____  _____  _____  _       ___    |\n");
			printf("\t|  | ___ \\| ___ \\|_   _|/  ___|/  __ \\|  _  || |     / _ \\   |\n");
			printf("\t|  | |_/ /| |_/ /  | |  \\ `--. | /  \\/| | | || |    / /_\\ \\  |\n");
			printf("\t|  | ___ \\|    /   | |   `--. \\| |    | | | || |    |  _  |  |\n");
			printf("\t|  | |_/ /| |\\ \\  _| |_ /\\__/ /| \\__/\\\\ \\_/ /| |____| | | |  |\n");
			printf("\t|  \\____/ \\_| \\_| \\___/ \\____/  \\____/ \\___/ \\_____/\\_| |_/  |\n");
			printf("\t!____________________________________________________________!\n\n");
			printf(STANDARD_WHITE);
		}

		//Costrutto switch che ci permette di accedere ad i vari menu e sottomenu tramite un indice
		switch(menu){

			case 1:	//Menu principale
				options = 3;
				printf(BOLD_WHITE"\n\t\t\t\tMenu Principale\n\n"STANDARD_WHITE);
				printf("\t\t(1) Gioca\t\t    (2) Utenti\n\n");
				printf("\t\t(3) Classifica\t\t    (0) Salva ed esci\n\n");
				//Chiedo che operazione si voglia fare, verificando la validita' dell'input
				do{
					printf("\t\tScelta: ");
					scanf("%d", &choice);
					while((ch=getchar()) != '\n' && ch != EOF);
					if(choice < 0 || choice > options) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
				}while(choice < 0 || choice > options);

				//Secondo costrutto switch dal quale eseguiamo l'azione mostrata dal menu
				switch(choice){
					case 1: menu = 2; break;
					case 2: menu = 3; break;
					case 3:
						system("@cls||clear");
						printf(BOLD_CYAN"\n\t\t\t\t   Classifica\n\n"STANDARD_WHITE);
						printf(BOLD_WHITE"\t\t\tUsername\t\tPunteggio\n"STANDARD_WHITE);
						stampa_leaderboard(leaderboard);
						printf("\n\n\t\t   Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al Menu Principale\n");
						stop();
						break;
				}

				break;

			case 2:
				options = 3;
				printf(BOLD_WHITE"\n\t\t\t\t     Gioca\n\n"STANDARD_WHITE);
				printf("\t\t(1) Nuova partita\t    (2) Carica partita\n\n");
				printf("\t\t(3) Torna indietro\n\n");
                                do{
                                        printf("\t\tScelta: ");
                                        scanf("%d", &choice);
					while((ch=getchar()) != '\n' && ch != EOF);
					if(choice < 1 || choice > options) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                }while(choice < 1 || choice > options);

                                switch(choice){
                                        case 1: menu = 4; break;
                                        case 2:
						system("@cls||clear");
						printf(BOLD_WHITE"\n\t\t\t      Carica ultima partita\n"STANDARD_WHITE);
						result = carica_partita(&partita, elenco_utenti, &turno, &options);
						if(result == 1){
							printf(BOLD_PURPLE"\n\n\n\t\t\t  Partita correttamente caricata\n"STANDARD_WHITE);
							printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tPremi "BOLD_RED"INVIO"STANDARD_WHITE" per avviare la partita\n");
							menu = 9;
						}
						else if(result == 0){
							printf(BOLD_RED"\n\n\n\t\t\t     Nessuna partita trovata\n"STANDARD_WHITE);
							printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tPremi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare indietro\n");
						}
						else if(result == -1){
							printf(BOLD_RED"\n\n\n\t\t\t      Utente 1 non trovato\n"STANDARD_WHITE);
                                                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tPremi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare indietro\n");
						}
						else if(result == -2){
							printf(BOLD_RED"\n\n\n\t\t\t      Utente 2 non trovato\n"STANDARD_WHITE);
                                                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tPremi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare indietro\n");
						}
						else if(result == -3){
							printf(BOLD_RED"\n\n\n\t\t\tNon riesco ad allocare la partita in memoria\n"STANDARD_WHITE);
                                                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tPremi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare indietro\n");
						}
						stop();
						break;
                                        case 3: menu = 1; break;
                                }


				break;

			case 3:
				options = 3;
				printf(BOLD_WHITE"\n\t\t\t\t     Utenti\n\n"STANDARD_WHITE);
				printf("\t\t(1) Registrati\t\t    (2) Login\n\n\t\t(3) Torna indietro\n\n\r");

				do{
                                	printf("\t\tScelta: ");
                 	        	scanf("%d", &choice);
					while((ch=getchar()) != '\n' && ch != EOF);
					if(choice < 1 || choice > options) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                }while(choice < 1 || choice > options);

                                switch(choice){
                                        case 1: menu = 5; break;
                                        case 2:
						system("@cls||clear");
						choice = 1;
						printf(BOLD_WHITE"\n\t\t\t\t     Login\n\n"STANDARD_WHITE);
						if(logged){
							get_username(temp_user3, temp_username);
							printf(BOLD_GREEN"\n\t\tLogin effettuato come: "STANDARD_WHITE BOLD_PURPLE"%s\n"STANDARD_WHITE, temp_username);
						}
						while(!logged && choice != 2){
							printf("\n\t\tUsername: ");
							inserimento_da_tastiera(temp_username);
							temp_user3 = ricerca_username(temp_username, elenco_utenti);
							if(is_empty_user(temp_user3) == 0){
								printf(BOLD_GREEN UP_LINE"\r\t\t\t\t\t\tUtente trovato"STANDARD_WHITE);
								while(!logged && choice != 2){
									printf("\n\t\tPassword: ");
									inserimento_da_tastiera(temp_password);
									if(login(temp_user3, temp_password) == 1){
										printf(BOLD_GREEN UP_LINE"\r\t\t\t\t\t\tLogin effettuato\n"STANDARD_WHITE);
										logged = 1;
									}
									else{
										printf(BOLD_RED UP_LINE"\r\t\t\t\t\t\tPassword errata\n"STANDARD_WHITE);
										printf(BOLD_YELLOW"\n\t\tPassword dimenticata?\n"STANDARD_WHITE);
                                                				printf("\t\tInserisci "BOLD_WHITE"1"STANDARD_WHITE" per riprovare, "BOLD_WHITE"2"STANDARD_WHITE" per uscire\n");
                                                				do{
                                                        				printf("\t\tScelta: ");
                                                        				scanf("%d", &choice);
                                                        				while((ch=getchar()) != '\n' && ch != EOF);
                                                        				if(choice < 1 || choice > 2) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                                				}while(choice < 1 || choice > 2);
										if(choice == 2) break;
									}
								}
							}
							else{
								printf(BOLD_RED UP_LINE"\r\t\t\t\t\t\tUtente non trovato\n"STANDARD_WHITE);
								printf(BOLD_YELLOW"\n\t\tUsername dimenticato?\n"STANDARD_WHITE);
                		                                printf("\t\tInserisci "BOLD_WHITE"1"STANDARD_WHITE" per riprovare, "BOLD_WHITE"2"STANDARD_WHITE" per uscire\n");
	                	                                do{
		                                                        printf("\t\tScelta: ");
		                                                        scanf("%d", &choice);
		                                                        while((ch=getchar()) != '\n' && ch != EOF);
		                                                        if(choice < 1 || choice > 2) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
		                                                }while(choice < 1 || choice > 2);
								if(choice == 2) break;
							}
						}
						if(logged){
							menu = 10;
							printf("\n\n\n\n\t\t\tPremi "BOLD_RED"INVIO"STANDARD_WHITE" per proseguire al menu\n");
							stop();
						}
						break;
                                        case 3: menu = 1; break;
				}
				break;

			case 4:
				printf(BOLD_WHITE"\n\t\t\t\t Nuova partita\n"STANDARD_WHITE);

				choice = 2;

				printf("\n\t\t"BOLD_WHITE"Log in primo utente"STANDARD_WHITE);
				if(logged1){
					get_username(temp_user3, temp_username);
					printf(BOLD_GREEN"\n\tLogin effettuato come: "STANDARD_WHITE BOLD_PURPLE"%s\n"STANDARD_WHITE, temp_username);
				}
				while(!logged1){
					printf("\n\tUsername: ");
					inserimento_da_tastiera(temp_username);
					temp_user3 = ricerca_username(temp_username, elenco_utenti);
					if(is_empty_user(temp_user3) == 0){
						printf(BOLD_GREEN UP_LINE"\r\t\t\t\t\tUtente trovato\n"STANDARD_WHITE);
						printf("\tPassword: ");
						inserimento_da_tastiera(temp_password);
						if(login(temp_user3, temp_password) == 1){
							printf(BOLD_GREEN UP_LINE"\r\t\t\t\t\tLogin effettuato\n"STANDARD_WHITE);
							logged1 = 1;
						}
						else
							printf(BOLD_RED UP_LINE"\r\t\t\t\t\tPassword errata\n"STANDARD_WHITE);
					}
					else
						printf(BOLD_RED UP_LINE"\r\t\t\t\t\t   Utente non trovato!\n"STANDARD_WHITE);

					if(!logged1){
						printf(BOLD_YELLOW"\n\tNon ancora registrato?"STANDARD_WHITE"\n\tInserisci "BOLD_WHITE"1"STANDARD_WHITE" per andare alla pagina di registrazione.\n\tIserisci "BOLD_WHITE"2"STANDARD_WHITE" per riprovare a fare il log in.\n");
						do{
							printf("\tScelta: ");
							scanf("%d",&choice);
							while((ch=getchar()) != '\n' && ch != EOF);
							if(choice < 1 || choice > 2) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
						}while(choice < 1 || choice > 2);
					}
					if(choice == 1)
						break;
				}


				if(choice == 1){
					choice = 2;
					menu = 5;
					break;
				}

				printf("\n\t\t"BOLD_WHITE"Log in secondo utente"STANDARD_WHITE);
				if(logged2){
					get_username(temp_user2, temp_username);
					printf(BOLD_GREEN"\n\tLogin effettuato come:\t"STANDARD_WHITE BOLD_PURPLE"%s\n"STANDARD_WHITE, temp_username);
				}
				while(!logged2){
                                        printf("\n\tUsername: ");
					get_username(temp_user3, temp_username2);
					do{
						inserimento_da_tastiera(temp_username);
						if(strcmp(temp_username, temp_username2) == 0)
							printf(UP_LINE BOLD_RED"\t\t\t\t\b\b\bIl secondo utente non puo' coincidere con il primo"STANDARD_WHITE"\n\n\tUsername: ");
					}while(strcmp(temp_username, temp_username2) == 0);

					temp_user2 = ricerca_username(temp_username, elenco_utenti);

                                        if(is_empty_user(temp_user2) == 0){
						printf(BOLD_GREEN UP_LINE"\r\t\t\t\t\tUtente trovato\n"STANDARD_WHITE);
                                                printf("\tPassword: ");
                                                inserimento_da_tastiera(temp_password);
                                                if(login(temp_user2, temp_password) == 1){
                                                        printf(BOLD_GREEN UP_LINE"\r\t\t\t\t\tLogin effettuato\n"STANDARD_WHITE);
                                                        logged2 = 1;
                                                }
                                                else
                                                        printf(BOLD_RED UP_LINE"\r\t\t\t\t\tPassword errata\n"STANDARD_WHITE);
                                        }
					else
                                                printf(BOLD_RED UP_LINE"\r\t\t\t\t\t   Utente non trovato!\n"STANDARD_WHITE);
					if(!logged2){
                                                printf(BOLD_YELLOW"\n\tNon ancora registrato?"STANDARD_WHITE"\n\tInserisci "BOLD_WHITE"1"STANDARD_WHITE" per andare alla pagina di registrazione.\n\tIserisci "BOLD_WHITE"2"STANDARD_WHITE" per riprovare a fare il log in.\n");
						do{
							printf("\tScelta: ");
                                                        scanf("%d",&choice);
							while((ch=getchar()) != '\n' && ch != EOF);
							if(choice < 1 || choice > 2) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                                }while(choice < 1 || choice > 2);
                                        }
                                        if(choice == 1)
                                                break;
                                }

				if(choice == 1){
					choice = 2;
					menu = 5;
					break;
				}
				result = inizializza_partita(&partita, temp_user3, temp_user2);
				if(result == 1){
					printf(BOLD_PURPLE"\n\t\t\tPartita correttamente inizializzata\n"STANDARD_WHITE);
					turno = 1;
					choice = 1;
					options = 3;
					menu = 9;
				}
				else{
					printf(BOLD_RED"\n\t\t\tERRORE NELL'INIZIALIZZAZIONE DELLA PARTITA\n"STANDARD_WHITE);
					exit(1);
				}
				printf("\n\t\t\tPremi "BOLD_RED"INVIO"STANDARD_WHITE" per avviare la partita\n");
				stop();
				break;

			case 5:
				printf(BOLD_WHITE"\n\t\t\t\tRegistra utente\n\n"STANDARD_WHITE);
				printf(BOLD_WHITE"\t\tInserisci dati utente"STANDARD_WHITE"\n\n\t\t\t\t\t(Max 8 caratteri)\r\t\tUsername: ");
				inserimento_da_tastiera(temp_username);
				temp_user = ricerca_username(temp_username, elenco_utenti);
				if(is_empty_user(temp_user) == 1){
					printf("\t\t\t\t\t(Max 8 caratteri)\r\t\tPassword: ");
					inserimento_da_tastiera(temp_password);
					temp_user = crea_user(temp_username, temp_password, 0);
					result = inserisci_utente_elenco(temp_user, &elenco_utenti);
					if(!result)
						result = 0;
				}
				else
					result = 0;
				if(result == 1)
					result = inserisci_utente_leaderboard(temp_user, &leaderboard);

				if(result == 1)
					printf(BOLD_GREEN "\n\t\t\t\t Utente creato!\n"STANDARD_WHITE"\n\n\n");
				else
					printf(BOLD_RED "\n\t\t\t     Utente gia' esistente!\n"STANDARD_WHITE"\n\n\n");

				//Torno al menu precedente
				if(choice == 1){
					menu = 3;
					printf("\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
				}
				else{
					menu = 4;
					printf("\n\t\t       Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al login\n");
				}
				stop();
				break;

			case 6:
				printf(BOLD_WHITE"\n\t\t\t\t\bModifica username\n\n"STANDARD_WHITE);
				get_username(temp_user3, temp_username);
				printf("\n\n\t\tUsername attuale: "BOLD_WHITE"%s"STANDARD_WHITE, temp_username);
				printf("\n\t\t\t\t\t\t\t(Max 8 caratteri)\r\t\tNuovo username: ");
                                inserimento_da_tastiera(temp_username);
				result = modifica_username(temp_user3, temp_username);
				if(result == 1)
					result = aggiorna_utente_elenco(temp_user3, &elenco_utenti);
				else
					printf(BOLD_RED"\n\t\t\t\t    Errore"STANDARD_WHITE);

				if(result == 1)
					printf(BOLD_GREEN"\n\t\t\t\t    Successo!"STANDARD_WHITE);
				else
					printf(BOLD_RED"\n\t\t\t\t    Errore"STANDARD_WHITE);

				printf("\n\n\n\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
				stop();
				menu = 10;
				break;

			case 7:
				printf(BOLD_WHITE"\n\t\t\t\tElimina utente\n\n"STANDARD_WHITE);
				if(admin){
	                                printf(BOLD_WHITE"\t\tInserisci i dati dell'utente che vuoi eliminare"STANDARD_WHITE"\n\n\t\tUsername: ");
	                                inserimento_da_tastiera(temp_username);
					temp_user = ricerca_username(temp_username, elenco_utenti);
				}
				else{
					temp_user = temp_user3;

				}

				if(temp_user == temp_user3){
					printf(BOLD_WHITE"\n\t\tSei sicuro di voler "STANDARD_WHITE BOLD_RED"eliminare"STANDARD_WHITE BOLD_WHITE" il tuo account?\n");
					printf(BOLD_WHITE"\t\t    Questa operazione non e' reversibile.\n\n"STANDARD_WHITE);
					printf("\n\t\tInserisci "BOLD_WHITE"1"STANDARD_WHITE" per confermare, "BOLD_WHITE"2"STANDARD_WHITE" per annullare\n");
					do{
                                                printf("\t\tScelta: ");
                                                scanf("%d", &choice);
                                                while((ch=getchar()) != '\n' && ch != EOF);
                                                if(choice < 1 || choice > 2) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                        }while(choice < 1 || choice > 2);
					if(choice == 2){
						menu = 10;
						break;
					}
					else
						logged = 0;
				}

				if(is_empty_user(temp_user) == 0){
					result = rimuovi_utente_elenco(temp_user, &elenco_utenti);
					if(result == 1)
						result = rimuovi_utente_leaderboard(temp_user, &leaderboard);
				}
				else
					result = 0;


				if(result == 1){
                                	clear_user(&temp_user);
				        printf(BOLD_GREEN"\n\t\t\t\t   Successo!\n"STANDARD_WHITE"\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
                                }
				else
                                        printf(BOLD_RED UP_LINE"\r\t\t\t\t\t   Utente non trovato!\n"STANDARD_WHITE"\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
                                stop();
				if(logged)
					menu = 10;
				else
					menu = 3;
                                break;

			case 8:
                                printf(BOLD_WHITE"\n\t\t\t\tCerca utente\n\n"STANDARD_WHITE);
                                printf(BOLD_WHITE"\t\tInserisci i dati dell'utente che vuoi trovare"STANDARD_WHITE"\n\n\t\tUsername: ");
                                inserimento_da_tastiera(temp_username);
				temp_user = ricerca_username(temp_username, elenco_utenti);
				if(is_empty_user(temp_user) == 0){
                                        printf(BOLD_GREEN UP_LINE"\r\t\t\t\t\t\tTrovato!\n"STANDARD_WHITE);
					printf(BOLD_WHITE"\n\t\t\tUsername:\t\tPunteggio\n\t\t\t"STANDARD_WHITE);
					stampa_utente(temp_user);
					printf(BOLD_WHITE"\n\n\t\t(1) Rendi admin\t\t    (2) Torna indietro\n\n");
					do{
                                        	printf("\t\tScelta: ");
                                        	scanf("%d", &choice);
                                        	while((ch=getchar()) != '\n' && ch != EOF);
                                        	if(choice < 1 || choice > 2) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                	}while(choice < 1 || choice > 2);
					if(choice == 1){
						result = make_admin(temp_user);
						if(result == 1)
							printf(BOLD_GREEN"\n\t\t\t\t    Successo!"STANDARD_WHITE"\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
						else
							printf(BOLD_PURPLE"\n\t\t\t\t\b\b\b\b\bUtente gia' amministatore"STANDARD_WHITE"\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
					}
				}
                                else
                                        printf(BOLD_RED UP_LINE"\r\t\t\t\t\t   Utente non trovato!\n"STANDARD_WHITE"\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");

				if(choice != 2)
					stop();
				menu = 10;
                                break;

			case 9:
				//In game
				do{
					//Stampa campo da gioco
					stampa_campo(partita, turno, 1);
					printf(BOLD_WHITE"\n\t\t\t\t\t\t\t  Inserisci 0 per uscire\r"UP_LINE"\tScelta: "STANDARD_WHITE);
					//Scelta mossa
					do{
        	                                scanf("%d", &choice);
        	                                while((ch=getchar()) != '\n' && ch != EOF);
        	                                if(choice < 0 || choice > options){
							stampa_campo(partita, turno, 1);
							printf(BOLD_RED UP_LINE"\n\t\t\t\tScelta non valida\n"STANDARD_WHITE BOLD_WHITE"\t\t\t\t\t\t\t  Inserisci 0 per uscire\r"UP_LINE"\tScelta: "STANDARD_WHITE);
	                                	}
					}while(choice < 0 || choice > options);
					if(choice != 0){
						//Eseguo la mossa e aggiorno il numero di opzioni
						result = esegui_mossa(partita, turno, choice);
						if(result == 0)		break;
						//Stampo il campo aggioranato dopo la mossa
						stampa_campo(partita, turno, 0);
						printf(BOLD_WHITE"\n\t\t\t\t\t\t\t  Inserisci 0 per uscire\r"UP_LINE "\t\tPremi INVIO per continuare\r"STANDARD_WHITE);
						stop();
						//Aggiorno il tavolo
						options = aggiorna_tavolo(partita, &turno);
					}
				}while(choice != 0 && options != 0);

				system("@cls||clear");
				printf(BOLD_WHITE"\n\t\t\t\t  Stato partita\n\n"STANDARD_WHITE);

				//Salvo la partita su file se mazzo e terra non sono vuoti (quindi se non e' conclusa)
				result = salva_partita(partita, turno, options);

				//Partita salvata correttamente
				if(result == 1)
					printf(BOLD_GREEN"\n\n\n\t\t\t Partita salvata correttamente\n"STANDARD_WHITE);
				//Partita conclusa, non serve salvare
				else if(result == 2){
					printf(BOLD_PURPLE"\n\n\t\t\t\tPartita conclusa\n"STANDARD_WHITE);
					result = get_winner(partita, temp_username, &choice);
					if(result == 1)
						printf(BOLD_WHITE"\n\t\t\t\b\b\bIl vincitore e': "STANDARD_WHITE BOLD_GREEN"%s"STANDARD_WHITE BOLD_WHITE" con "STANDARD_WHITE BOLD_YELLOW"%d"STANDARD_WHITE BOLD_WHITE" punti!"STANDARD_WHITE, temp_username, choice);
					else if(result == 2)
						printf(BOLD_BLUE"\n\t\t\t\t    Pareggio"STANDARD_WHITE);
					else
						printf(BOLD_RED"\n\t\t\t\t     ERRORE"STANDARD_WHITE);
					//Se la partita e' conclusa, aggiorno i punteggi
					update_points(partita, &leaderboard);
				}
				//Partita non salvata
				else
					printf(BOLD_RED"\n\n\n\t\t     Errore nel salvataggio della partita\n"STANDARD_WHITE);
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t  Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al Menu Principale\n");
				stop();

				//Pulisco la struct partita e sloggo gli utenti
				clear_partita(&partita);
				logged1 = 0;
				logged2 = 0;
				//Torno al menu principale
				choice = 1;
				menu = 1;
				break;

			case 10:
				printf(BOLD_WHITE"\n\t\t\t\t    Menu Utenti\n\n"STANDARD_WHITE);
				admin = is_admin(temp_user3);
				options = admin?6:4;

				//Mostro le opzini disponibili in base ai permessi
				printf("\t\t(1) Modifica username\t    (2) Modifica password\n\n");
				if(!admin)
					printf("\t\t(3) Elimina utente\t    (4) Log out\n\n");
				else{
					printf("\t\t(3) Elimina un utente\t    (4) Elenco utenti\n\n");
					printf("\t\t(5) Ricerca un utente\t    (6) Log out\n\n");
				}
				do{
                                	printf("\t\tScelta: ");
                 	        	scanf("%d", &choice);
					while((ch=getchar()) != '\n' && ch != EOF);
					if(choice < 1 || choice > options) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                }while(choice < 1 || choice > options);

				if(!admin && choice == 4)
					choice = 6;

				switch(choice){
					case 1:
						menu = 6;
						break;
					case 2:
						menu = 11;
						break;
					case 3:
						menu = 7;
						break;
					case 4:
						system("@cls||clear");
						printf(BOLD_BLUE"\n\t\t\t\t Elenco utenti\n\n"STANDARD_WHITE);
						printf(BOLD_WHITE"\t\t\tUsername\t\tPunteggio\n"STANDARD_WHITE);
						stampa_elenco_utenti(elenco_utenti);
						printf("\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
						stop();
						break;
					case 5:
						menu = 8;
						break;
					case 6:
						menu = 3;
						logged = 0;
						break;
				}
				break;
			case 11:
				printf(BOLD_WHITE"\n\t\t\t\tModifica password\n\n\n"STANDARD_WHITE);
				choice = 1;
                                do{
					printf("\n\t\tPassword attuale: "STANDARD_WHITE);
					inserimento_da_tastiera(temp_password);
					if(login(temp_user3, temp_password) == 0){
						printf(UP_LINE BOLD_RED"\t\t\t\t\t\t\b\b\bPassword errata\n"STANDARD_WHITE);
						printf(BOLD_YELLOW"\n\tPassword dimenticata?\n"STANDARD_WHITE);
						printf("\tInserisci "BOLD_WHITE"1"STANDARD_WHITE" per riprovare, "BOLD_WHITE"2"STANDARD_WHITE" per uscire\n");
						do{
                                        		printf("\tScelta: ");
                                        		scanf("%d", &choice);
                                        		while((ch=getchar()) != '\n' && ch != EOF);
                                        		if(choice < 1 || choice > 2) printf(UP_LINE BOLD_RED"\t\t\t\t\tScelta non valida, riprova\n"STANDARD_WHITE);
                                		}while(choice < 1 || choice > 2);
					}
				}while(login(temp_user3, temp_password) == 0 && choice != 2);
				if(choice == 2){
					menu = 10;
					break;
				}
                                printf("\n\t\t\t\t\t\t\t(Max 8 caratteri)\r\t\tNuova password: ");
                                inserimento_da_tastiera(temp_password);
                                result = modifica_password(temp_user3, temp_password);
                                if(result == 1)
                                        printf(BOLD_GREEN"\n\t\t\t\t    Successo!"STANDARD_WHITE);
                                else
                                        printf(BOLD_RED"\n\t\t\t\t    Errore"STANDARD_WHITE);

                                printf("\n\n\n\n\n\t\t    Premi "BOLD_RED"INVIO"STANDARD_WHITE" per tornare al menu Utenti\n");
                                stop();
                                menu = 10;
                                break;
		}


		//Richiedo un INVIO prima di aggiornare il menu
//		printf("\n\n\t\t\t\t\t     Premi "BOLD_BLUE"INVIO"STANDARD_WHITE" per aggiornare il menu\r");
//		stop();

	}while(choice != 0);

	//All'uscita salvo su file l'elenco utenti e la leaderboard
	salva_elenchi(elenco_utenti, leaderboard);

	//Svuoto la memoria
	clear_lista_utenti(&elenco_utenti, &leaderboard);
	system("@cls||clear");
}

