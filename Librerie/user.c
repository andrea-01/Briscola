#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_empty_user(Player lista_utenti){
	if(lista_utenti == NULL)
		return 1;
	else
		return 0;
}

Player crea_user(char username[], char password[], int admin){
	Player user = (Player)malloc(sizeof(struct utente));
	if(user == NULL)
		return NULL;
	strcpy(user->username, username);
	strcpy(user->password, password);
	user->punteggio = 0;
	user->admin = admin;
	user->mano = crea_lista_carte();
	user->prese = crea_lista_carte();
	user->next = NULL;
	user->next_l = NULL;
	return user;
}

Player crea_lista_utenti(){
	return NULL;
}

void clear_user(Player* my_utente){
	if(is_empty_user(*my_utente) == 0){
		clear_lista_carte(&((*my_utente)->mano));
		clear_lista_carte(&((*my_utente)->prese));
		free(*my_utente);
		(*my_utente) = NULL;
	}
}

void clear_lista_utenti(Player* lista_utenti, Player* leaderboard){
	if(is_empty_user(*lista_utenti) == 1) return;
	Player i;
	for(i = (*lista_utenti); i != NULL; i = (*lista_utenti)){
        (*lista_utenti) = (*lista_utenti)->next;
		clear_user(&i);
	}
	*leaderboard = NULL;
}

int inserisci_utente_elenco(Player my_utente, Player* lista_utenti){
	Player r = *lista_utenti;
	Player q = *lista_utenti;
	while(q != NULL && strcmp(q->username, my_utente->username) < 0){
		r = q;
		q = q->next;
	}
	if(q == (*lista_utenti)){
		my_utente->next = (*lista_utenti);
		(*lista_utenti) = my_utente;
	}
	else{
		r->next = my_utente;
		my_utente->next = q;
	}
	return 1;
}

int inserisci_utente_leaderboard(Player my_utente, Player* leaderboard){
	Player r = *leaderboard;
	Player q = *leaderboard;
	while(q != NULL && q->punteggio >= my_utente->punteggio){
		r = q;
		q = q->next_l;
	}
	if(q == (*leaderboard)){
		my_utente->next_l = (*leaderboard);
		(*leaderboard) = my_utente;
	}
	else{
		r->next_l = my_utente;
		my_utente->next_l = q;
	}
	return 1;
}

int rimuovi_utente_elenco(Player my_utente, Player* lista_utenti){
	Player q = *lista_utenti;
	Player p = *lista_utenti;
	while(is_empty_user(p) == 0 && my_utente != p){
		q = p;
		p = p->next;
	}
	if(p == my_utente){
		if(p == *lista_utenti){
			*lista_utenti = p->next;
			return 1;
		}
		q->next = p->next;
		return 1;
	}
	else
		return 0;
}

int rimuovi_utente_leaderboard(Player my_utente, Player* leaderboard){
	Player q = *leaderboard;
	Player p = *leaderboard;
	while(p != NULL && my_utente != p){
		q = p;
		p = p->next_l;
	}
	if(p == my_utente){
		if(p == *leaderboard){
			*leaderboard = p->next_l;
			return 1;
		}
		q->next_l = p->next_l;
		return 1;
	}
	else
		return 0;
}

int aggiorna_utente_elenco(Player my_utente, Player* lista_utenti){
	if(is_empty_user(my_utente) == 1) return 0;
	int result = rimuovi_utente_elenco(my_utente, lista_utenti);
	if(result == 1)
		result = inserisci_utente_elenco(my_utente, lista_utenti);
	return result;
}

int aggiorna_utente_leaderboard(Player my_utente, Player* leaderboard){
	if(is_empty_user(my_utente) == 1) return 0;
	int result = rimuovi_utente_leaderboard(my_utente, leaderboard);
	if(result == 1)
		inserisci_utente_leaderboard(my_utente, leaderboard);
	return result;
}

Player ricerca_username(char username[], Player lista_utenti){
	if(is_empty_user(lista_utenti) == 1)
		return NULL;
	while(lista_utenti != NULL && strcmp(lista_utenti->username, username) < 0){
		lista_utenti = lista_utenti->next;
	}
	if(lista_utenti != NULL && strcmp(lista_utenti->username, username) == 0)
		return lista_utenti;
	else
		return NULL;
}

void stampa_utente(Player my_utente){
	if(is_empty_user(my_utente) == 0){
		printf("%s\t\t",my_utente->username);
        if(strlen(my_utente->username) < 8)
            printf("\t");
        printf("%d\n",my_utente->punteggio);
    }
}

void stampa_elenco_utenti(Player lista_utenti){
	if(is_empty_user(lista_utenti) == 0){
		for(int k = 1; lista_utenti != NULL; lista_utenti = lista_utenti->next){
			printf("\t\t    %d)\t",k++);
			stampa_utente(lista_utenti);
		}
	}
	else
		printf("\n\t\t\tLa lista di utenti e' vuota\n");
}

void stampa_leaderboard(Player leaderboard){
	if(is_empty_user(leaderboard) == 0){
		for(int k = 1; leaderboard != NULL; leaderboard = leaderboard->next_l){
			printf("\t\t    %d)\t",k++);
			stampa_utente(leaderboard);
		}
	}
	else
		printf("\n\t\t\tLa lista di utenti e' vuota\n");
}

int modifica_username(Player my_utente, char new_username[]){
	if(is_empty_user(my_utente) == 0){
		strcpy(my_utente->username, new_username);
		return 1;
	}
	else
		return 0;
}

int modifica_password(Player my_utente, char new_password[]){
	if(is_empty_user(my_utente) == 0){
                strcpy(my_utente->password, new_password);
                return 1;
        }
        else
                return 0;
}

int salva_elenchi(Player lista_utenti, Player leaderboard){
	FILE* f = fopen("Elenco", "w");
	if(f == NULL) return 0;
	Player temp = lista_utenti;
	while(is_empty_user(temp) == 0){
		fprintf(f, "%s %s %d %d\n", temp->username, temp->password, temp->punteggio, temp->admin);
		temp = temp->next;
	}
	fclose(f);

	f = fopen("Leaderboard", "w");
	if(f == NULL) return 0;
	temp = leaderboard;
	while(is_empty_user(temp) == 0){
		fprintf(f, "%s\n", temp->username);
		temp = temp->next_l;
	}
	fclose(f);
	return 1;
}

int carica_elenchi(Player* lista_utenti, Player* leaderboard){
	FILE* f = fopen("Elenco", "r");
	if(f == NULL) return 0;
	Player temp_utente;
	char username[9], password[9];
	int punteggio, admin;

//	Utilizzare una pila di appoggio / puntatore tail
	while(fscanf(f, "%s %s %d %d\n", username, password, &punteggio, &admin) != EOF){
		temp_utente = crea_user(username, password, admin);
		temp_utente->punteggio = punteggio;
		inserisci_utente_elenco(temp_utente, lista_utenti);
	}
	fclose(f);

	f = fopen("Leaderboard", "r");
	if(f == NULL) return 0;
	while(fscanf(f,"%s", username) != EOF){
		temp_utente = ricerca_username(username, *lista_utenti);
		if(is_empty_user(temp_utente) == 0)
			inserisci_utente_leaderboard(temp_utente, leaderboard);
	}
	fclose(f);
	return 1;
}

Carta* get_mano(Player my_utente){
	if(is_empty_user(my_utente) == 1)
		return NULL;
	return &(my_utente->mano);
}

Carta* get_prese(Player my_utente){
	if(is_empty_user(my_utente) == 1)
                return NULL;
	return &(my_utente->prese);
}

int* get_punteggio(Player my_utente){
	if(is_empty_user(my_utente) == 1)
                return NULL;
	return &my_utente->punteggio;
}

int get_password(Player my_utente, char password[]){
	if(is_empty_user(my_utente) == 1)
                return 0;
	strcpy(password, my_utente->password);
	return 1;
}

int get_username(Player my_utente, char username[]){
	if(is_empty_user(my_utente) == 1)
                return 0;
	strcpy(username, my_utente->username);
	return 1;
}

int login(Player my_utente, char password[]){
	if(is_empty_user(my_utente) == 1)
		return 0;
	if(strcmp(my_utente->password, password) == 0)
		return 1;
	else
		return 0;
}

int is_admin(Player my_utente){
	if(is_empty_user(my_utente) == 1)
		return 0;
	else
		return my_utente->admin;
}

int make_admin(Player my_utente){
	if(is_empty_user(my_utente) == 1 || my_utente->admin == 1)
                return 0;
	my_utente->admin = 1;
	return 1;
}
