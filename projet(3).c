#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

enum { LONG_PRENOM_MAX = 30, NB_ETU_MAX = 100, NB_ABS_MAX = 8000, NB_JUSTIFICATIF_MAX = 50 };

typedef struct {
	int identifiant, groupe, absence;
	char prenom[LONG_PRENOM_MAX];
} Etudiant;

typedef struct {
	int id, jour;
	char demi_jour[3];
	char motif[NB_JUSTIFICATIF_MAX];
	int etat_motif; //0 c'est non-justifiée, 1 c'est en cours de traitement, 2 justifiée


}Absence;

//C0---------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

void sortie(char requete[]) {
	if (strcmp(requete, "exit") == 0) {
		exit(0);
	}
}

//C1---------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

int inscription(Etudiant tab[NB_ETU_MAX], int i) {
	char prenom[LONG_PRENOM_MAX];
	int groupe;

	scanf("%s", prenom); //limite taille saisie

	if (strlen(prenom) < LONG_PRENOM_MAX) {
		strcpy(tab[i].prenom, prenom);
		scanf("%d", &groupe);
		for (int j = 0; j < i; ++j) {
			if (groupe == tab[j].groupe && strcmp(prenom, tab[j].prenom) == 0) {
				printf("Nom incorrect\n");
				return 0;
			}
		}
		tab[i].groupe = groupe;
		tab[i].identifiant = i + 1;
		tab[i].absence = 0;
		printf("Inscription enregistree (%d)\n", tab[i].identifiant);
		return 1;
	}
	else {
		printf("Nom trop long\n");
		return 0;
	}
}

//C2---------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

void absence(Etudiant tab[NB_ETU_MAX], Absence tab2[NB_ABS_MAX], int* nb_absence, int nb_etudiant) {
	int id, jour;
	char demi_jour[3];

	scanf("%d", &id);

	if (id > 0 && id <= nb_etudiant) {
		scanf("%d", &jour);

		if (jour >= 1 && jour <= 40) {
			scanf("%s", demi_jour);
			if ((strcmp(demi_jour, "am") == 0) || (strcmp(demi_jour, "pm") == 0)) {

				for (int j = 0; j < *nb_absence; ++j) {
					if (jour == tab2[j].jour && strcmp(demi_jour, tab2[j].demi_jour) == 0 && tab2[j].id == id) {
						printf("Absence deja connue\n");
						return;
					}
				}

				tab2[*nb_absence].id = id;
				tab2[*nb_absence].jour = jour;
				strcpy(tab2[*nb_absence].demi_jour, demi_jour);
				tab2[*nb_absence].etat_motif = 0;
				(*nb_absence)++;

				printf("Absence enregistree [%d]\n", *nb_absence);
			}
			else {
				printf("Demi-journee incorrecte\n");
			}
		}
		else {
			printf("Date incorrecte\n");
		}
	}
	else {
		printf("Identifiant incorrect\n");
	}
}

//C3---------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

int cmp_etu(const void* etu1, const void* etu2) {
	// Conversion
	Etudiant* etudiant1 = (const Etudiant*)etu1;
	Etudiant* etudiant2 = (const Etudiant*)etu2;

	if (etudiant1->groupe < etudiant2->groupe) {
		return -1;
	}
	if (etudiant1->groupe > etudiant2->groupe) {
		return 1;
	}
	return strcmp(etudiant1->prenom, etudiant2->prenom);
}

void etudiants(const Etudiant etu[], int nb_etu, const Absence abs[], int nb_abs) {
	int jour;
	scanf("%d", &jour);
	if (nb_etu == 0) {
		printf("Aucun inscrit\n");
		return;
	}
	if (jour <= 0) {
		printf("Date incorrecte\n");
		return;
	}
	qsort(etu, nb_etu, sizeof(etu[0]), cmp_etu);
	for (int i = 0; i < nb_etu; ++i) {
		unsigned int compt_abs = 0;
		for (int j = 0; j < nb_abs; j++) {
			if (abs[j].id == etu[i].identifiant && abs[j].jour <= jour)
				compt_abs++;
		}
		printf("(%u) %s %u %u\n", etu[i].identifiant, etu[i].prenom, etu[i].groupe, compt_abs);
	}
}

//C4---------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------

void saisieLigne(char* ligne, int max) {
	int c = getc(stdin);
	while (c == ' ' || c == '\t')
		c = getc(stdin);
	ungetc(c, stdin);
	fgets(ligne, max, stdin);
	ligne[strlen(ligne) - 1] = '\0';
}


void justification(Absence tab2[], int nb_abs, int nb_jus) {
	int id, jour, compteur = 0;
	char motif[NB_JUSTIFICATIF_MAX];

	scanf("%d", &id);
	--id;

	if (id > nb_abs || id < 0) {
		printf("Identifiant incorrect\n");
		return;
	}

	scanf("%d", &jour);

	if (jour < tab2[id].jour) {
		printf("Date Incorrect\n");
		return;
	}
	if (tab2[id].etat_motif == 1 || tab2[id].etat_motif == 2) {
		printf("Justificatif deja connu\n");
		return;
	}

	if ((tab2[id].jour + 3) >= jour) {
		tab2[id].etat_motif = 1;
		++nb_jus;
	}
	saisieLigne(motif, NB_JUSTIFICATIF_MAX);
	strcpy(tab2[id].motif, motif);
	printf("Justificatif enregistre\n", tab2[id].etat_motif);
}

//C5---------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------

void validations(Absence tab2[], int nb_abs, Etudiant tab[], int nb_etu, int nb_jus) {
	if (nb_jus == 0) {
		printf("Aucune validation en attente\n");
		return;
	}
	for (int i = 0; i < nb_abs; ++i) {
		if (tab2[i].etat_motif == 1) {
			printf("[%d] (%d) %s %d %d/%s (%s)\n", i, tab2[i].id,  tab[tab2[i].id].prenom, tab[tab2[i].id].groupe, tab2[i].jour, tab2[i].demi_jour, tab2[i].motif);
		}
	}
}

//C6---------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------


//Int Main---------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

int main() {
	char requete[100];
	Etudiant tab[NB_ETU_MAX];
	Absence tab2[NB_ABS_MAX];
	int nb_etu = 0;
	int nb_abs = 0;
	int nb_jus = 0;
	while (1) {
		scanf("%s", requete);
		sortie(requete);

		if (strcmp(requete, "inscription") == 0) {
			if (nb_etu >= NB_ETU_MAX) {
				printf("Limite d'inscription atteinte\n");
			}
			else {
				if (inscription(tab, nb_etu)) {
					nb_etu++;
				}
			}
		}
		else if (strcmp(requete, "absence") == 0) {
			absence(tab, tab2, &nb_abs, nb_etu);
		}
		else if (strcmp(requete, "etudiants") == 0) {
			etudiants(tab, nb_etu, tab2, nb_abs);
		}
		else if (strcmp(requete, "justificatif") == 0) {
			justification(tab2, nb_abs, nb_jus);
		}
		else if (strcmp(requete, "validations") == 0) {
			validations(tab2, nb_abs, tab, nb_etu, nb_jus);
		}

	}
}
