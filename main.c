#include <stdlib.h>
#include <stdio.h>

#include <err.h>
#include <string.h>
#include <time.h>

#define NUMBER_DE 5

/*
	
	gcc -Wall -c main.c
	gcc -o main main.o
	./main

*/

/* **************************************  STRUCTURE DE DONNEES ************************************** */

typedef struct De De;

struct De
{
    int nombre;
    De *suivant;
};

typedef struct Liste Liste;

struct Liste
{
    De *premier;
};

/* **************************************  LISTE CHAINEE ************************************** */


/* Initialisation d'une liste chainee */
static Liste * construction_chaine_de (void) {

	Liste *liste = malloc(sizeof(*liste));
	De *de = malloc(sizeof(*de));

	if (liste == NULL || de == NULL)
		errx(EXIT_FAILURE, "erreur: impossible de creer une liste chainee");

	de->nombre = 0;
	de->suivant = NULL;
	liste->premier = de;

	return liste;

}

/* Insertion des elements dans la liste */
static void insertion(Liste *liste) {

    /* Création du nouvel élément */
    De * nouveau = malloc(sizeof(*nouveau));

    if (liste == NULL || nouveau == NULL)
		errx(EXIT_FAILURE, "erreur: impossible de creer une liste chainee");

    nouveau->nombre = 0;
    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;

}

/* Affichage de la liste chainée pour tester */
static void afficherListe(Liste *liste) {

    if (liste == NULL)
    	exit(EXIT_FAILURE);
    

    De *actuel = liste->premier;

    while (actuel != NULL) {

        printf("%d - ", actuel->nombre);
        actuel = actuel->suivant;

    }

    printf("\n");
}

/* Constuction d'une liste chainée à 5 éléments */
static void complete_liste (Liste *liste) {

	/* Initialisation d'un compteur pour faire exactement 5 elements */
	int counter = 1;

	while (counter < NUMBER_DE) {
		insertion(liste);
		counter++;
	}

}

/* **************************************  FONCTION DE JEU  ************************************** */

static void lancer_random (Liste *liste) {

	if (liste == NULL)
		errx(EXIT_FAILURE, "erreur: aucune liste");

	srand(time(NULL));

	/* Création du nouvel élément */
    De * current = liste->premier;

	while (current != NULL) {

		current->nombre = (rand() % 6) + 1;
		current = current->suivant; 

	}


}

static int lancer_de (Liste *liste, int de) {

	if (de < 1 || de > 5)
		return 1;

	if (liste == NULL)
		errx(EXIT_FAILURE, "erreur: aucune liste");

	srand(time(NULL));

	/* Création du nouvel élément */
    De * current = liste->premier;
    int counter = 0;

    while (counter != (de-1)) {
    	current = current->suivant;
    	counter++;
    }

    current->nombre = (rand() % 6) + 1;


    return 0;

}

/* **************************************  FONCTIONS REGLES ************************************** */

static int already_present (int * tab_joueur, int * index, int choice) {

	int length = index[0];

	int value = 0;

	for (int i = 0; i < length; i++)
		if (i%2 == 1) {
			if (tab_joueur[i] == choice)
				value = 1;
		} 

	if (value == 1)
		printf("Valeur déjà présente pour ce cas\n");

	return value;
}

static int regles (Liste *liste, int * tab_joueur, int * index_choix) {

	int index_current = index_choix[0];

	De * current = liste->premier;

	int * tableau = malloc (sizeof(int) * NUMBER_DE);

	if (!tableau)
		errx(EXIT_FAILURE, "erreur: Probleme d'allocation");

	int index = 0;

	while (current != NULL) {
		tableau[index] = current->nombre;
		current = current->suivant;
		index++;
	}

	FILE *file = fopen("regles.txt", "r");

	if (!file)
		errx(EXIT_FAILURE, "erreur: Probleme lecture de fichier");

	int charactere;
	int counter = 0;

	while((charactere=fgetc(file)) != EOF) {
		if (charactere == '\n') {
			counter++;
		}
	}

	rewind(file);

	printf("\n------------------ REGLES ------------------\n");


	for (int i = 0; i < counter+1; i++) {

		while ((charactere=fgetc(file)) != '\n' && charactere != EOF) {

			printf("%c", charactere);

		}

		printf("\n");

	}

	printf("\n");

	int value_user;
	int continuer = 1;

	int choice = 0;
	int point = 0;

	do {

		printf("Votre choix : ");
		scanf("%d", &value_user);

		int choice_test = already_present(tab_joueur, index_choix, value_user);

		if (choice_test == 1)
			continue;

		/* Choix basique*/
		if (value_user >= 1 && value_user <= 6) {

			int counter = 0;

			for (int i = 0; i < NUMBER_DE; i++) {

				if (tableau[i] == value_user)
					counter += value_user;

			}

			tab_joueur[index_current] = value_user;
			tab_joueur[index_current + 1] = counter;

			index_current += 2;

			index_choix[0] = index_current;

			continuer = 0;

		}

		if (continuer == 0)
			continue;

		/* Autres choix */

		int boolean = 0;
		int tmp = 0;
		int combien = 0;

		switch (value_user) {
			case 7:

				for (int i = 0; i < NUMBER_DE; i++) {
					tmp = 0;
					for (int j = 0; j < NUMBER_DE; j++) {
						if (tableau[i] == tableau[j]) {
							tmp++;
						}
					}
				}

				if (tmp == 3) {
					choice = 7;
					continuer = 0;
				}

				break;
			case 8:
				
				tmp = 1;
				for (int val = 1; val < 6; val++) {
					combien = 0;
					for (int i = 0; i < NUMBER_DE; i++) {
						if (val == tableau[i])
							combien++;
					}

					if (combien != 1)
						boolean = 5;
				}

				if (boolean != 5) {
					choice = 8;
					point = 15;
					continuer = 0;
				}

				break;

			case 9:

				tmp = 1;

				for (int tmp = 2; tmp < 7; tmp++) {
					combien = 0;
					for (int i = 0; i < NUMBER_DE; i++) {
						if (tmp == tableau[i])
							combien++;
					}

					if (combien != 1)
						boolean = 5;
				}

				if (boolean != 5) {
					choice = 9;
					point = 20;
					continuer = 0;
				}
				
				break;

			case 10:
			case 11:

				for (int i = 0; i < NUMBER_DE; i++) {
					tmp = 0;
					for (int j = 0; j < NUMBER_DE; j++) {
						if (tableau[i] == tableau[j]) {
							tmp++;
						}
					}

					if (tmp == 4) {
						choice = 11;
						point = 40;
						continuer = 0;
						break;
					}
				}

				break;

			case 12:

				for (int i = 0; i < NUMBER_DE-1; i++) 
					if (tableau[i] != tableau[i+1])
						boolean = 1;

				if (boolean == 0) {
					choice = 12;
					point = 50;
					continuer = 0;
				}

				break;
		}

	} while (continuer);

	if (choice != 0) {
		tab_joueur[index_current] = choice;
		tab_joueur[index_current+1] = point;

		index_current += 2;

		index_choix[0] = index_current;
	}

	fclose(file);
	free(tableau);

	return 0;

}

/* **************************************  JEU YAMS ************************************** */

static void write_result (int joueur, int * tab1, int * index1, int * tab2, int * index2) {

	FILE *file = fopen("result_joueur1.txt", "wa");

	if (!file)
		errx(EXIT_FAILURE, "erreur: Impossible d'ouvrir le fichier");

  FILE *file1 = fopen("result_joueur2.txt", "wa");

  if (!file1)
    errx(EXIT_FAILURE, "erreur: Impossible d'ouvrir le fichier");

	int length1 = index1[0];
	int length2 = index2[0];


	for (int i = 0; i < length1; i=i+2) {

		fprintf(file, "%d - %d\n", tab1[i], tab1[i+1]);

	}


	for (int i = 0; i < length2; i=i+2) {

		fprintf(file1, "%d - %d\n", tab2[i], tab2[i+1]);

	}

	fclose(file);
  fclose(file1);

}



static int jouer (int mode) {

	/* Construction de la liste chainee pour les des */
	Liste * premier_de = construction_chaine_de();
	complete_liste(premier_de);

	int terminer = 0;

	/* Indication de qui joue */
	int user = 0;

	int choix_user;

	int * choix_resultat_joueur1 = malloc (sizeof(int) * 12 * 2);

	if (!choix_resultat_joueur1)
		errx(EXIT_FAILURE, "erreur : Probleme allocation");

	int * choix_resultat_joueur2 = malloc (sizeof(int) * 12 * 2);

	if (!choix_resultat_joueur2)
		errx(EXIT_FAILURE, "erreur : Probleme allocation");

	int * index1 = malloc(sizeof(int));
	int * index2 = malloc(sizeof(int));

	if (!index1 || !index2)
		errx(EXIT_FAILURE, "erreur : Probleme allocation");

	index1[0] = 0;
	index2[0] = 0;

	while (!terminer) {

		int counter_lancer = 3;
		int forfait = 0;

		printf("********************************************************************************\n");

		/* Joueur 1 */
		if (user%2 == 0)
			fprintf(stdout, "Joueur 1 : \n");

		/* Joueur 2 ou Ordinateur */
		if (user%2 == 1)
			fprintf(stdout, "Joueur 2 : \n");

		do {

			do {

				fprintf(stdout, "Voulez vous lancez les dés ? (1 ou 2)\n 1) Oui \n 2) Non \n\n Votre choix : ");

				scanf("%d", &choix_user);

				printf("\n");

				if (choix_user == 1 || choix_user == 2) {
					break;
				}

			} while (1);

			if (choix_user == 2) {

				int quitter;
				fprintf(stdout, "Voulez vous quitter la partie ? En faisant cela vous abandonnez ! (1 ou 2) \n 1) Abandon \n 2) Continuer \n\n Votre choix : ");

				scanf("%d", &quitter);

				printf("\n");

				if (quitter == 1) {

					terminer = 1;
					forfait = 1;
					break;

				}

			}

		} while (choix_user != 1);

		/* Si abandon */
		if (terminer && forfait) {

			int joueur = user%2;

			

			switch (joueur) {
				case 0:
					fprintf(stdout, "Le joueur 2 / Ordinateur gagne par forfait\n");
					write_result(2, choix_resultat_joueur1, index1, choix_resultat_joueur2, index2);
					break;
				case 1:
					fprintf(stdout, "Le joueur 1 gagne par forfait\n");
					write_result(1, choix_resultat_joueur1, index1, choix_resultat_joueur2, index2);
					break;

			}

			return 1;
		}

		/* Lancer de dé */

		int tmp;
		int nombre_de_des_a_relancer;
		int de_choisi;
		int stop_lancer = 1;

		lancer_random(premier_de);

		do {

			if(counter_lancer == 3)
				afficherListe(premier_de);

			counter_lancer--;

			printf("Voulez vous relancer des dés ? (1 ou 2)\n1) Oui\n2) Non\nVotre choix : ");
      
			scanf("%d", &tmp);

			if (tmp == 1) {

				printf("Combien de dés voulez vous relancer ? (1 - 5)\nVotre choix :");

				scanf("%d", &nombre_de_des_a_relancer);

				for (int i = 0; i < nombre_de_des_a_relancer; i++) {

					int val = 1;

					do {

						printf("%d er/ème dé à changer ? (1 - 5) \n", i+1);
						scanf("%d", &de_choisi);

						val = lancer_de(premier_de, de_choisi);

					} while (val == 1);


				}

				afficherListe(premier_de);

			} else if (tmp == 2){

				stop_lancer = 0;

			}


		} while (counter_lancer > 0 && stop_lancer == 1);


		int test_regles;

		if (user%2 == 0){
			test_regles = regles(premier_de, choix_resultat_joueur1, index1);

			if(index1[0] == (12 * 2)-1 && test_regles != 0) {
				write_result(1, choix_resultat_joueur1, index1, choix_resultat_joueur2, index2);
				return 1;
			}
		}
		else if (user%2 == 1) {
			test_regles = regles(premier_de, choix_resultat_joueur2, index2);

			if(index2[0] == (12 * 2)-1 && test_regles != 0) {
				write_result(2, choix_resultat_joueur1, index1, choix_resultat_joueur2, index2);
				return 1;
			}
		}

		user++;

	}

	return 0;

} 


/* Fonction yams indiquant à l'utilisateur le choix à faire */
static void yams (void) {

	int mode_de_jeu;
	int continuer = 0;

	fprintf(stdout, "************* Bienvenue au jeu du yams *************\n\n");
	fprintf(stdout, "Quel mode de jeu voulez vous ? (1 ou 2)\n 1) Joueur vs ordinateur \n 2) Joueur vs Joueur \n\n Votre choix : ");

	/* 2 modes de jeux possible */
	do {

		if (continuer == 1) {

			fprintf(stdout, "\n\nJe suis désolé, je n'ai pas compris votre choix :\n\n");
			fprintf(stdout, "Quel mode de jeu voulez vous ? (1 ou 2)\n 1) Joueur vs ordinateur \n 2) Joueur vs Joueur \n\n Votre choix : ");

		}

		scanf("%d", &mode_de_jeu);
		continuer = 1;

	} while (mode_de_jeu != 1 && mode_de_jeu != 2); 

	/* Indication du mode de jeu */
	switch (mode_de_jeu) {

		case 1:
			fprintf(stdout, "\n\nChoix mode : Joueur vs ordinateur\n\n");
			break;
		case 2:
			fprintf(stdout, "\n\nChoix mode : Joueur vs Joueur\n\n");
			break;

	}

	int gagner = 0;

	/* Boucle principale pour le jeu */
	while (gagner == 0) {

		gagner = jouer(mode_de_jeu);

	}
	

}

int main (int argc, char * argv[]) {

	yams();

	return EXIT_SUCCESS;
}