#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VER "1.5"

/*-----------------------------------------------------NOTES-----------------------------------------------------------
* 1 - Pour une raison inconnue il est impossible d'envoyer argv[0], argv[1], argv[2] etc... pour l'ouverture/l'écriture d'un fichier
* alors j'ai fais des allocations dynamiques où j'ai copier les argv dedans et ensuite envoyé ces tableaux alloués pour l'ouverture
* de fichiers.
* 2 - Pour une raison inconnue quand un tableau est transféré dans une fonction, on ne peut pas récupérer sa taille correctement
* sieof(tableau) n'affiche pas lâ même valeur dans une fonction et en dehors. Alors dans chaque fonction qui traite un tableau
* j'envoie la taille du tableau afin de traiter manuellement chaque case.
*/

/*--------------------------------------------LES TYPES D'ERREUR D'ARRET DU PROGRAMME------------------------------
* Les différents types de code erreur :
* 0 - Fonctionnement normal du programme.
* 1 - Manque de paramètres en entrée du programme pour un fonctionnement minmimum (-h).
* 2 - Mode/option inconnue.
* 3 - Fichier impossible à lire ou impossible d'écrire les fichiers (pas de fichier à lire ou de destination).
* 4 - Fichier impossible à créer et/ou ecrire dedans.
* 5 - S'il y a un manque de mémoire alors le programme s'arrête avec cette erreur (allocation dynamique).
*/

//-----------------------------------------PROTOTYPE DES FONCTIONS-------------------------------------------------//
void afficherCommandes(const char nameFile[]);
void verifNbParametresSuffisant(const int* nbParam, const int nbParamMin, const char nomFichier[]);
void allocationDynamiqueTest(char* tableau);

void ouvreFichierLectureEtTest(FILE** fichierLecture, const char adresseEntree[]);
void ouvreFichierEcritureEtTest(FILE** fichierEcriture, const char adresseEcriture[], const char nomFichiers[], const int* numFichier, const char extension[]);

void affichageCodeHexaAvecInterface(const int* nbOctetParLigne, char adresseFichierLu[], FILE* fichierLu);
void afficherInterfaceCodeHexa(const long* nbOctetParLigne, const char adresseFichier[]);
void afficherCodeHexa(FILE* fichierLu, const int* nbOctetParLigne);

void rechercherFichiersDansFichier(FILE* fichierLu, const char adresseEcritureFichiers[], const unsigned char chaineEnTete[], const int tailleChaineEnTete, const unsigned char chaineEnFin[], const int tailleChaineEnFin, const char nomFichiers[], const char extension[]);

int compareTableaux(const unsigned char tab[], const unsigned char tabLue[], const int tailleTableaux);
void pileDecalage(unsigned char tab[], unsigned char* nouvelleValeur, int tailleTab);


//-----------------------------------------------FONCTION MAIN-------------------------------------------------------------------//
int main(int argc, char* argv[])
{
	//----------------------------------------------Initalisation------------------------------------------//

	//argc est le nombre de paramètre en entrée du programme
	//argv est un tableau de char qui à chaque case contient un des paramètres en entrée

	const int nbOctetParLigne = 16;	//Nombre d'octet affiché par ligne
	FILE* fichierLu = NULL;			//Création d'un objet de typer ficher 



	//-------------------------------------En-tête et en-fin des types de fichier à extraire------------------------------------------//

	//Fichiers sans problème d'extraction :
	const unsigned char chainePngEnTete[] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,'\0' };	// 89 50 4E 47 0D 0A 1A 0A
	const unsigned char chainePngEnFin[] = { 0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82, '\0' };	// 49 45 4E 44 AE 42 60 82

	//Fichiers avec problèmes d'extraction :
	const unsigned char chaineJpgEnTete[] = { 0xFF,0xD8,0xFF,'\0' };							// FF D8 FF
	const unsigned char chaineJpgEnFin[] = { 0xFF,0xD9, '\0' };									// FF D9

	const unsigned char chaineGifEnTete[] = { 0x47,0x49,0x46,0x38,'\0' };						// 47 49 46 38
	const unsigned char chaineGifEnFin[] = { 0x00,0x00,0x3B,'\0' };								// 00 00 3B



	//----------------------------Code pour le test du programme sans envoyer de paramètres en entrée-----------------------//
	////////TEST
	//openFileReadingBinaryModeAndTest(&fichierLu, "C:\\Users\\Quent\\Downloads\\Téléchargement\\poubelle\\test.gif");//Ouverture fichier
	//rechercherFichiersDansFichier(fichierLu, "C:\\Users\\Quent\\Downloads\\Téléchargement", chaineGifEnTete,sizeof(chaineGifEnTete) ,chaineGifEnFin,sizeof(chaineGifEnFin), "gifFile", ".gif");
	////////TEST



	//------------------------------------------------------Début de traitement--------------------------------------------------------------//

	verifNbParametresSuffisant(&argc, 2, argv[0]);//Verification du nombre de paramètre en entrée supérieur ou égal à 2

	//Le code suivant s'occupe de la gestion des options
	if (argv[1][0] == '-')
	{
		switch (argv[1][1])
		{
		case ('h'):
			//Affiche l'aide du programme et le mode d'emploi
			afficherCommandes(argv[0]);
			break;

		case ('d'):
			//Affiche le code hexadécimale du code analysé uniquement

			//Vérfie que le nombre de paramètre est supérieur ou égal à 3
			verifNbParametresSuffisant(&argc, 3, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copié de argv[2]
			char* adresseFichier_d = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier_d);
			strcpy(adresseFichier_d, argv[2]);

			ouvreFichierLectureEtTest(&fichierLu, adresseFichier_d);//Ouverture fichier à lire
			affichageCodeHexaAvecInterface(&nbOctetParLigne, adresseFichier_d, fichierLu);//Affichage du code hexa du fichier lu
			fclose(fichierLu);//Fertemture du fichier lu

			//Libéraiton de la mémoire allouée
			free(adresseFichier_d);

			break;

		case ('D'):
			//Affiche le code hexadéciamle du code analysé et effectue l'extraction des fichiers.

			//Vérfie que le nombre de paramètre est supérieur ou égal à 4
			verifNbParametresSuffisant(&argc, 4, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copié de argv[2]
			char* adresseFichier_D = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier_D);
			strcpy(adresseFichier_D, argv[2]);

			//Allocation d'un tableau dynamique qui contient l'adresse de destination d'ou écrire les fichiers trouvé dans le code du fichier lu
			//La donnée est copié de argv[3] dans adressseDestination_D
			char* adresseDestination_D = malloc((strlen(argv[3]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseDestination_D);
			strcpy(adresseDestination_D, argv[3]);


			ouvreFichierLectureEtTest(&fichierLu, adresseFichier_D);//Ouverture fichier
			affichageCodeHexaAvecInterface(&nbOctetParLigne, adresseFichier_D, fichierLu);//Affichage du code hexa du fichier

			//Recherche des fichiers connus
			rechercherFichiersDansFichier(fichierLu, adresseDestination_D, chainePngEnTete, sizeof(chainePngEnTete), chainePngEnFin, sizeof(chainePngEnFin), "pngFile", ".png");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_D, chaineJpgEnTete, sizeof(chaineJpgEnTete), chaineJpgEnFin, sizeof(chaineJpgEnFin), "jpgFile", ".jpg");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_D, chaineGifEnTete, sizeof(chaineGifEnTete), chaineGifEnFin, sizeof(chaineGifEnFin), "gifFile", ".gif");

			//Fermerture du fichier lu (inspecté)
			fclose(fichierLu);

			//Libéraiton de la mémoire allouée
			free(adresseFichier_D);
			free(adresseDestination_D);

			break;

		case ('e'):
			//Effectue une extraction du fichier donné

			//Vérfie que le nombre de paramètre est supérieur ou égal à 4
			verifNbParametresSuffisant(&argc, 4, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copié de argv[2]
			char* adresseFichier_e = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier_e);
			strcpy(adresseFichier_e, argv[2]);

			//Allocation d'un tableau dynamique qui contient l'adresse de destination d'ou écrire les fichiers trouvé dans le code du fichier lu
			//La donnée est copié de argv[3] dans adressseDestination_e
			char* adresseDestination_e = malloc((strlen(argv[3]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseDestination_e);
			strcpy(adresseDestination_e, argv[3]);


			ouvreFichierLectureEtTest(&fichierLu, adresseFichier_e);//Ouverture fichier

			//Recherche des fichiers connus
			rechercherFichiersDansFichier(fichierLu, adresseDestination_e, chainePngEnTete, sizeof(chainePngEnTete), chainePngEnFin, sizeof(chainePngEnFin), "pngFile", ".png");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_e, chaineJpgEnTete, sizeof(chaineJpgEnTete), chaineJpgEnFin, sizeof(chaineJpgEnFin), "jpgFile", ".jpg");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_e, chaineGifEnTete, sizeof(chaineGifEnTete), chaineGifEnFin, sizeof(chaineGifEnFin), "gifFile", ".gif");

			//Fermerture du fichier lu (inspecté)
			fclose(fichierLu);

			//Libéraiton de la mémoire allouée
			free(adresseFichier_e);
			free(adresseDestination_e);

			break;

		default:
			//Cas où l'option donnée est inconnue
			printf("Error option %c unknown.\n", argv[1][1]);
			afficherCommandes(argv[0]);
			exit(2);
			break;
		}
	}
	else
	{
		//Dans le cas où l'option est mal écrite on affiche la liste des commandes et l'usage
		afficherCommandes(argv[0]);
	}
	return 0;
}



//------------------------------------------------------FONCTION D'AFFICHAGE AIDE-----------------------------------------//

void afficherCommandes(const char nameFile[])
{
	/*
	* Fonction qui affiche l'aide du programme
	*/
	printf("\n"
		"filesFinder " VER "\n"
		"by Im-Rises\n"
		"This application is just a test.\n"
		"\n\n"
		"Usage: %s <mode> <input file> <output directory>\n"
		"\n"
		"Options:\n"
		"-h \tDisplay the list of command and manual (no input and output needed).\n"
		"-d \tDisplay the hexadecimal code of the file scanned (No ouput needed).\n"
		"-D \tDisplay the hexadecimail code of the file scanned and extract it's data.\n"
		"-e \tExtract the data of the file.\n"
		"\n", nameFile);
}


/*------------------------------------------------------FONTIONS TEST BON FONCTIONNEMENT---------------------------------------*/

void verifNbParametresSuffisant(const int* nbParam, const int nbParamMin, const char nomFichier[])
{
	/*
	* Fonction qui affiche un message d'erreur et l'aide dans le cas où le nombre de paramètre n'est pas suffisant
	* 
	* Si le nombre de paramètre est insuffisant l'aide s'affiche et le programme s'arrête avec le message d'erreur 2
	*/
	if (*nbParam < nbParamMin)
	{
		printf("Not enough parameters.\n");
		afficherCommandes(nomFichier);
		exit(1);
	}
}

void allocationDynamiqueTest(char* tableau)
{
	/*
	* Fonction qui test le bon fonctionnement dela création d'un tableau dynamique
	* 
	* Si le tableau ne s'est pas créé correctement alors le programme s'arrête avec le code erreur 3
	*/
	if (tableau == NULL)
	{
		printf("Memory issue %s\n"
			"Error: More memory needed.\n"
			"\n", tableau);
		exit(5);
	}
}



/*--------------------------------------------------FONCTION DE TENTATIVE DE LECTURE/ECRITURE FICHIER-----------------------------------*/

void ouvreFichierLectureEtTest(FILE** fichierLecture,const char adresseEntree[])
{
	/*
	* Fonction d'ouverture d'un fichier en lecture binaire et du test du bon fonctionnement
	* 
	* Le programme reçoit en entrée l'adresse du fichier à lire et tente de l'ouvrir et l'attribue à l'objet fichierLecture
	* Si l'ouverture di fichier ne s'est pas faite correctement alors le programma s'arrête avec le message d'erreur 2
	*/
	*fichierLecture = fopen(adresseEntree, "rb");//Tente d'ouvrir le fichier en lecture binaire
	if (*fichierLecture == NULL)//test si le fichier a été ouvert correctement
	{
		printf("Input file %s\n"
			"Error: No such file\n"
			, adresseEntree);
		exit(3);
	}
}

void ouvreFichierEcritureEtTest(FILE** fichierEcriture, const char adresseEcriture[], const char nomFichier[], const int *numFichier, const char extension[])
{
	/*
	* Fonction de création d'un fichier et du démarrage de l'écriture en binaire
	*
	* La fonction recoit en entrée ce qui défini le fichier à créer, l'adresse, le nom, le numéro du fichier, et son extension.
	* Une concaténation de ses données est faite afin de les mettres dans un seul tableau et enfin on crée un fichier avec.
	*/

	int numFichierTemp = *numFichier;
	int nombreChiffre = 0;

	//Le while ci-dessous permet de calculer le nombre de chiffres qu'il y a dans numFichierTemp
	while (numFichierTemp != 0) 
	{
		numFichierTemp = numFichierTemp / 10;
		nombreChiffre++;
	}
	
	//En fonction du nombre de chiffre du numéro du fihcier calculer juste avant don crée un tableau
	//de char qui aura autant de case que de chiffres à le numéro de fichier
	char* numFichierChar = malloc((nombreChiffre+1) * sizeof(char));

	//Appel de la fonction de test du focntionnement correcte de l'allocation dynamique
	allocationDynamiqueTest(numFichierChar);
	
	//Converti l'entier fournis en char[] dans la tableau numFichierChar en base 10.
	itoa(*numFichier, numFichierChar, 10);

	//On défini un tableau de char qui dont on défini la taille en fonction de la somme de la taille de tous les autres tableaux.
	/*
	* Pour numFichierChar on est obligé de se baser sur la taille du tableau avec sizeof et non ce qu'il contient avec strlen
	* car l'allocation dynamique avec malloc ne permet pas bien l'utilisation de strlen, qui résulterait à un dépassement de mémoire.
	*/
	char* adresseNomNumExtensionFichier = malloc((strlen(adresseEcriture)+ strlen(nomFichier) + sizeof(numFichierChar) + strlen(extension)) * sizeof(char));

	//Appel de la fonction de test du focntionnement correcte de l'allocation dynamique
	allocationDynamiqueTest(adresseNomNumExtensionFichier);

	adresseNomNumExtensionFichier[0] = '\0';
	strcat(adresseNomNumExtensionFichier,adresseEcriture);

	/*
	* Le if ci-dessous ajouter un '\' en fin du tableau adresseNomBumExtensionFichier si celui-ci n'en a pas.
	* afin d'éviter que le dernier dossier envoyer ne soit pas compris comme un dossier mais comme une partie
	* du fichier à créer.
	*/
	if (adresseNomNumExtensionFichier[strlen(adresseNomNumExtensionFichier)-1] != '\\')
	{
		adresseNomNumExtensionFichier[strlen(adresseNomNumExtensionFichier) + 1] = '\0';
		adresseNomNumExtensionFichier[strlen(adresseNomNumExtensionFichier)] = '\\';
	}

	//Concaténation de toute les données dans un tableau de char (adresse, nom, numéro et extension de fichier).
	strcat(adresseNomNumExtensionFichier, nomFichier);
	strcat(adresseNomNumExtensionFichier, numFichierChar);
	strcat(adresseNomNumExtensionFichier, extension);

	//Création et début d'édition du fichier en binaire avec le paramètre tableau qui contient (adresse, nom, numéro et extension de fichier).
	*fichierEcriture = fopen(adresseNomNumExtensionFichier, "wb");

	//Vérifie la bonne création et édition du fichier en binaire. 
	//Si une erreur survient alors le programme s'arrête avec l'erreur 4
	if (fichierEcriture == NULL)
	{
		free(adresseNomNumExtensionFichier);
		printf("Output file %s\n"
			"Error: No such file\n"
			"\n", adresseEcriture);
		exit(4);
	}

	//Libération des mémoire allouées dynamiquement
	free(numFichierChar);
	free(adresseNomNumExtensionFichier);
}




/*-------------------------------------------------------FONCTION AFFICHAGE CODE FICHIER LU---------------------------------------------------------*/

void affichageCodeHexaAvecInterface(const int *nbOctetParLigne,char adresseFichierLu[], FILE* fichierLu)
{
	/*
	* Fonction qui affiche l'interface et le code hexadécimale du ficheir ouvert par l'appel de deux fonctions
	*/

	afficherInterfaceCodeHexa(nbOctetParLigne, adresseFichierLu);

	afficherCodeHexa(fichierLu, nbOctetParLigne);
}

void afficherInterfaceCodeHexa(const long *nbOctetParLigne, const char adresseFichier[])
{
	/*
	* Fontion qui affiche l'interface d'ecriture du début du code hexa
	*/
	printf("\nThe hexa code of the file %s is :\n\n", adresseFichier);
	printf("Offset (h)\t");

	for (int i = 0; i < *nbOctetParLigne; i++)//Ecrit le numéro de chaque colonne en fonction du nombre définie en paramètre
	{
		printf("%02X ", i);
	}
	printf("\n\n");
}

void afficherCodeHexa(FILE* fichierLu, const int* nbOctetParLigne)
{
	/*
	* Fonction qui affiche le code hexdécimale avec le numéro de ligne du fichier lu envoué en paramètre de la fonction.
	*/

	long numeroLigne = 0;
	unsigned char octetLu;
	int nbOctetLigneEcrit = 0;

	printf("%00000008X\t", numeroLigne);//Affichage première ligne

	while (!feof(fichierLu)) // On continue tant qu'on ai pas à la fin du fichier
	{
		//On affiche avant de lire le premier caractère pour éviter d'écrire le caractère de fin de fichier en hexa "FF" et en texte "ÿ"
		octetLu = fgetc(fichierLu); // On lit le caractère

		if (!feof(fichierLu))//Evite d'afficher l'octet de fin de fichier
		{
			if (nbOctetLigneEcrit >= *nbOctetParLigne)//Si on a attteint le bout de la ligne (nbOctetParLigne) alors on passe à la ligne
			{
				nbOctetLigneEcrit = 0;
				printf("\n");
				numeroLigne += *nbOctetParLigne;//Incrémentation du numéro de ligne par rapport au nombre d'octet par ligne
				printf("%00000008X\t", numeroLigne);
			}
			printf("%02X ", octetLu); // On l'affiche en hexadécimale avec deux caractères et en majuscule s'il y a des lettres
		}

		nbOctetLigneEcrit++;//Incrémentation du nombre d'octet écrit à la ligne actuelle
		
	}
	printf("\n\n");
}



/*------------------------------------------------------------FONCTION DE RECHERCHE DE FICHIERS DANS UN FICHIER---------------------------------*/

void rechercherFichiersDansFichier(FILE* fichierLu, const char adresseEcritureFichiers[], const unsigned char chaineEnTete[], const int tailleChaineEnTete, const unsigned char chaineEnFin[], const int tailleChaineEnFin, const char nomFichiers[], const char extension[])
{
	unsigned char octetLu;//Octet de réception des données lu dans fichierLu
	unsigned int numfichierPaterneTrouve = 0;//Numéro du fichier à indiqué dans le nom lors de sa création
	
	//Donnée mémoire qui informe combien d'octet nous avons passé après la dernière en-tête afin 
	//de repartir en dessous une fois le précdent fichier en train d'être créé finsi (le temps de trouvé l'en-fin)
	unsigned long nbOctetApresEnTete = 0;

	int finTrouve = 0;//Booléen qui indique si l'en-fin du fichier dont on a préalablement trouvé l'en-tête a été trouvé


	//unsigned char chaineLuChercheEnTete[sizeof(chaineEnTete)];
	unsigned char* chaineLuChercheEnTete = malloc(tailleChaineEnTete*sizeof(char));
	chaineLuChercheEnTete[tailleChaineEnTete - 1] = '\0';

	//unsigned char chaineLuChercheEnFin[sizeof(chaineEnFin)];
	unsigned char* chaineLuChercheEnFin = malloc(tailleChaineEnFin * sizeof(char));
	chaineLuChercheEnFin[tailleChaineEnFin - 1] = '\0';

	rewind(fichierLu);

	while (!feof(fichierLu)) // On continue tant qu'on ai pas à la fin du fichier
	{
		//On affiche avant de lire le premier caractère pour éviter d'écrire le caractère de fin de fichier en hexa "FF" et en texte "ÿ"
		octetLu = fgetc(fichierLu); // On lit le caractère
		pileDecalage(chaineLuChercheEnTete, &octetLu, tailleChaineEnTete);// Rajoute en fin de pile le dernier octet lu

		if (0 == compareTableaux(chaineLuChercheEnTete, chaineEnTete, tailleChaineEnTete))//Si la chaine d'en-tête est trouvé dans le fichier lu
		{
			numfichierPaterneTrouve++;//Incrémentation du nombre de fichier de ce type trouvé

			FILE* fichierEcriture = NULL;//Création d'un objet de type fichier
			
			//Création d'un fichier et début d'écriture en binaire dedans
			ouvreFichierEcritureEtTest(&fichierEcriture, adresseEcritureFichiers, nomFichiers, &numfichierPaterneTrouve, extension);

			//Ecriture de l'en-tête dans le ficheir créé
			fputs(chaineEnTete, fichierEcriture);

			//Le while ci-dessous écrit chaque octet lu dans fichierLu dans fichierEcriture jusqu'à trouvé l'en-fin
			while (!feof(fichierLu) && finTrouve == 0)//Tant qu'on est pas à la fin du fichier et que finTrouve=0
			{
				octetLu = fgetc(fichierLu); // On lit le caractère

				nbOctetApresEnTete++;//Incrémentation du nombre de caracteres passés après l'en-tête précédemment trouvée

				fputc(octetLu, fichierEcriture);//Ecrit l'octet lu dans le fichierEcrtiture

				pileDecalage(chaineLuChercheEnFin, &octetLu, tailleChaineEnFin);//Rajoute en fin de pile le dernier octet lu

				if (0 == compareTableaux(chaineLuChercheEnFin, chaineEnFin,tailleChaineEnFin))//Si l'en-fin est trouvée
				{
					finTrouve = 1;//finTrouve est mis à 1 afin de quitter le while
				}
			}
			finTrouve = 0;
			fclose(fichierEcriture);//fertmeture du fichier écriture
			fseek(fichierLu, (-1) * nbOctetApresEnTete, SEEK_CUR);//Place le curseur après l'en-tête précédemment trouvée
			nbOctetApresEnTete = 0;//réinitialisation de la valeur de replacement car on vient de se replacer après l'en-tête du fichier précédent
		}
	}
	rewind(fichierLu);//Remet le curseur au début du fichier

	//Libération de la mémoire allouée dynamiquement
	free(chaineLuChercheEnTete);
	free(chaineLuChercheEnFin);

	printf("Number of %s file extracted : %d\n", extension, numfichierPaterneTrouve);
}


/*------------------------------------------------------------FONCTION AUXILIAIRES------------------------------------------------------*/

int compareTableaux(const unsigned char tab1[], const unsigned char tab2[], const int tailleTableaux)
{
	//Pour les fichiers avec un 0x00 il y a un problème car c'est compris comme un \0 donc la fin d'un tableau.
	//Pour palier ce problème une fonction manuelle qui compare en-tete avec l'en-tete lue et l'en-fin et l'en-fin lue
	//en utilisant la taille des tableaux et non la fin du tableau avec \0 que fait strcmp
	//Comme ça tous les caractères y compris les \0 seront comparés

	int identique = 0;
	//Si tableaux identiques (return 0) sinon 1
	for (int i = 0; i < tailleTableaux; i++)
	{
		if (tab1[i] != tab2[i])
			identique = 1;
	}
	return identique;
}

void pileDecalage(unsigned char tab[], unsigned char* nouvelleValeur, int tailleTab)
{
	/*
	* Fonction qui parcours la tableau et décale les valeurs des plus 2 dans 1, 3 dans 2 etc... (du haut au bas de la pile)
	* Ensuite la nouvelle valeur est mise tout en haut de la pile.
	*/

	for (int i = 0; i < tailleTab - 2; i++)
	{
		tab[i] = tab[i + 1];
	}
	tab[tailleTab - 2] = *nouvelleValeur;
}