#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// coucou je suis un commentaire 

#define VER "1.0"

/*-----------------------------------------------------NOTES-----------------------------------------------------------
* 1 - Pour une raison inconnue il est impossible d'envoyer argv[0], argv[1], argv[2] etc... pour l'ouverture/l'�criture d'un fichier
* alors j'ai fais des allocations dynamiques o� j'ai copier les argv dedans et ensuite envoy� ces tableaux allou�s pour l'ouverture
* de fichiers.
* 2 - Pour une raison inconnue quand un tableau est transf�r� dans une fonction, on ne peut pas r�cup�rer sa taille correctement
* sieof(tableau) n'affiche pas l� m�me valeur dans une fonction et en dehors. Alors dans chaque fonction qui traite un tableau
* j'envoie la taille du tableau afin de traiter manuellement chaque case.
*/

/*--------------------------------------------LES TYPES D'ERREUR D'ARRET DU PROGRAMME------------------------------
* Les diff�rents types de code erreur :
* 0 - Fonctionnement normal du programme.
* 1 - Manque de param�tres en entr�e du programme pour un fonctionnement minmimum (-h).
* 2 - Mode/option inconnue.
* 3 - Fichier impossible � lire ou impossible d'�crire les fichiers (pas de fichier � lire ou de destination).
* 4 - Fichier impossible � cr�er et/ou ecrire dedans.
* 5 - S'il y a un manque de m�moire alors le programme s'arr�te avec cette erreur (allocation dynamique).
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

	//argc est le nombre de param�tre en entr�e du programme
	//argv est un tableau de char qui � chaque case contient un des param�tres en entr�e

	const int nbOctetParLigne = 16;	//Nombre d'octet affich� par ligne
	FILE* fichierLu = NULL;			//Cr�ation d'un objet de typer ficher 



	//-------------------------------------En-t�te et en-fin des types de fichier � extraire------------------------------------------//

	//Fichiers sans probl�me d'extraction :
	const unsigned char chainePngEnTete[] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,'\0' };	// 89 50 4E 47 0D 0A 1A 0A
	const unsigned char chainePngEnFin[] = { 0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82, '\0' };	// 49 45 4E 44 AE 42 60 82

	//Fichiers avec probl�mes d'extraction :
	const unsigned char chaineJpgEnTete[] = { 0xFF,0xD8,0xFF,'\0' };							// FF D8 FF
	const unsigned char chaineJpgEnFin[] = { 0xFF,0xD9, '\0' };									// FF D9

	const unsigned char chaineGifEnTete[] = { 0x47,0x49,0x46,0x38,'\0' };						// 47 49 46 38
	const unsigned char chaineGifEnFin[] = { 0x00,0x00,0x3B,'\0' };								// 00 00 3B



	//----------------------------Code pour le test du programme sans envoyer de param�tres en entr�e-----------------------//
	////////TEST
	//openFileReadingBinaryModeAndTest(&fichierLu, "C:\\Users\\Quent\\Downloads\\T�l�chargement\\poubelle\\test.gif");//Ouverture fichier
	//rechercherFichiersDansFichier(fichierLu, "C:\\Users\\Quent\\Downloads\\T�l�chargement", chaineGifEnTete,sizeof(chaineGifEnTete) ,chaineGifEnFin,sizeof(chaineGifEnFin), "gifFile", ".gif");
	////////TEST



	//------------------------------------------------------D�but de traitement--------------------------------------------------------------//

	verifNbParametresSuffisant(&argc, 2, argv[0]);//Verification du nombre de param�tre en entr�e sup�rieur ou �gal � 2

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
			//Affiche le code hexad�cimale du code analys� uniquement

			//V�rfie que le nombre de param�tre est sup�rieur ou �gal � 3
			verifNbParametresSuffisant(&argc, 3, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copi� de argv[2]
			char* adresseFichier_d = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier_d);
			strcpy(adresseFichier_d, argv[2]);

			ouvreFichierLectureEtTest(&fichierLu, adresseFichier_d);//Ouverture fichier � lire
			affichageCodeHexaAvecInterface(&nbOctetParLigne, adresseFichier_d, fichierLu);//Affichage du code hexa du fichier lu
			fclose(fichierLu);//Fertemture du fichier lu

			//Lib�raiton de la m�moire allou�e
			free(adresseFichier_d);

			break;

		case ('D'):
			//Affiche le code hexad�ciamle du code analys� et effectue l'extraction des fichiers.

			//V�rfie que le nombre de param�tre est sup�rieur ou �gal � 4
			verifNbParametresSuffisant(&argc, 4, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copi� de argv[2]
			char* adresseFichier_D = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier_D);
			strcpy(adresseFichier_D, argv[2]);

			//Allocation d'un tableau dynamique qui contient l'adresse de destination d'ou �crire les fichiers trouv� dans le code du fichier lu
			//La donn�e est copi� de argv[3] dans adressseDestination_D
			char* adresseDestination_D = malloc((strlen(argv[3]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseDestination_D);
			strcpy(adresseDestination_D, argv[3]);


			ouvreFichierLectureEtTest(&fichierLu, adresseFichier_D);//Ouverture fichier
			affichageCodeHexaAvecInterface(&nbOctetParLigne, adresseFichier_D, fichierLu);//Affichage du code hexa du fichier

			//Recherche des fichiers connus
			rechercherFichiersDansFichier(fichierLu, adresseDestination_D, chainePngEnTete, sizeof(chainePngEnTete), chainePngEnFin, sizeof(chainePngEnFin), "pngFile", ".png");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_D, chaineJpgEnTete, sizeof(chaineJpgEnTete), chaineJpgEnFin, sizeof(chaineJpgEnFin), "jpgFile", ".jpg");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_D, chaineGifEnTete, sizeof(chaineGifEnTete), chaineGifEnFin, sizeof(chaineGifEnFin), "gifFile", ".gif");

			//Fermerture du fichier lu (inspect�)
			fclose(fichierLu);

			//Lib�raiton de la m�moire allou�e
			free(adresseFichier_D);
			free(adresseDestination_D);

			break;

		case ('e'):
			//Effectue une extraction du fichier donn�

			//V�rfie que le nombre de param�tre est sup�rieur ou �gal � 4
			verifNbParametresSuffisant(&argc, 4, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copi� de argv[2]
			char* adresseFichier_e = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier_e);
			strcpy(adresseFichier_e, argv[2]);

			//Allocation d'un tableau dynamique qui contient l'adresse de destination d'ou �crire les fichiers trouv� dans le code du fichier lu
			//La donn�e est copi� de argv[3] dans adressseDestination_e
			char* adresseDestination_e = malloc((strlen(argv[3]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseDestination_e);
			strcpy(adresseDestination_e, argv[3]);


			ouvreFichierLectureEtTest(&fichierLu, adresseFichier_e);//Ouverture fichier

			//Recherche des fichiers connus
			rechercherFichiersDansFichier(fichierLu, adresseDestination_e, chainePngEnTete, sizeof(chainePngEnTete), chainePngEnFin, sizeof(chainePngEnFin), "pngFile", ".png");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_e, chaineJpgEnTete, sizeof(chaineJpgEnTete), chaineJpgEnFin, sizeof(chaineJpgEnFin), "jpgFile", ".jpg");
			rechercherFichiersDansFichier(fichierLu, adresseDestination_e, chaineGifEnTete, sizeof(chaineGifEnTete), chaineGifEnFin, sizeof(chaineGifEnFin), "gifFile", ".gif");

			//Fermerture du fichier lu (inspect�)
			fclose(fichierLu);

			//Lib�raiton de la m�moire allou�e
			free(adresseFichier_e);
			free(adresseDestination_e);

			break;

		default:
			//Cas o� l'option donn�e est inconnue
			printf("Error option %c unknown.\n", argv[1][1]);
			afficherCommandes(argv[0]);
			exit(2);
			break;
		}
	}
	else
	{
		//Dans le cas o� l'option est mal �crite on affiche la liste des commandes et l'usage
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
	* Fonction qui affiche un message d'erreur et l'aide dans le cas o� le nombre de param�tre n'est pas suffisant
	* 
	* Si le nombre de param�tre est insuffisant l'aide s'affiche et le programme s'arr�te avec le message d'erreur 2
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
	* Fonction qui test le bon fonctionnement dela cr�ation d'un tableau dynamique
	* 
	* Si le tableau ne s'est pas cr�� correctement alors le programme s'arr�te avec le code erreur 3
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
	* Le programme re�oit en entr�e l'adresse du fichier � lire et tente de l'ouvrir et l'attribue � l'objet fichierLecture
	* Si l'ouverture di fichier ne s'est pas faite correctement alors le programma s'arr�te avec le message d'erreur 2
	*/
	*fichierLecture = fopen(adresseEntree, "rb");//Tente d'ouvrir le fichier en lecture binaire
	if (*fichierLecture == NULL)//test si le fichier a �t� ouvert correctement
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
	* Fonction de cr�ation d'un fichier et du d�marrage de l'�criture en binaire
	*
	* La fonction recoit en entr�e ce qui d�fini le fichier � cr�er, l'adresse, le nom, le num�ro du fichier, et son extension.
	* Une concat�nation de ses donn�es est faite afin de les mettres dans un seul tableau et enfin on cr�e un fichier avec.
	*/

	int numFichierTemp = *numFichier;
	int nombreChiffre = 0;

	//Le while ci-dessous permet de calculer le nombre de chiffres qu'il y a dans numFichierTemp
	while (numFichierTemp != 0) 
	{
		numFichierTemp = numFichierTemp / 10;
		nombreChiffre++;
	}
	
	//En fonction du nombre de chiffre du num�ro du fihcier calculer juste avant don cr�e un tableau
	//de char qui aura autant de case que de chiffres � le num�ro de fichier
	char* numFichierChar = malloc((nombreChiffre+1) * sizeof(char));

	//Appel de la fonction de test du focntionnement correcte de l'allocation dynamique
	allocationDynamiqueTest(numFichierChar);
	

	//Conversion entier en char[]
	////Converti l'entier fournis en char[] dans la tableau numFichierChar en base 10.
	//itoa(*numFichier, numFichierChar, 10);//Fonctionne mais pas sur linux car pas dans la biblioth�que standart
	sprintf(numFichierChar, "%d", *numFichier);//Fonctionne sous windows et linux


	//On d�fini un tableau de char qui dont on d�fini la taille en fonction de la somme de la taille de tous les autres tableaux.
	/*
	* Pour numFichierChar on est oblig� de se baser sur la taille du tableau avec sizeof et non ce qu'il contient avec strlen
	* car l'allocation dynamique avec malloc ne permet pas bien l'utilisation de strlen, qui r�sulterait � un d�passement de m�moire.
	*/
	char* adresseNomNumExtensionFichier = malloc((strlen(adresseEcriture)+ strlen(nomFichier) + sizeof(numFichierChar) + strlen(extension)) * sizeof(char));

	//Appel de la fonction de test du focntionnement correcte de l'allocation dynamique
	allocationDynamiqueTest(adresseNomNumExtensionFichier);

	adresseNomNumExtensionFichier[0] = '\0';
	strcat(adresseNomNumExtensionFichier,adresseEcriture);

	/*
	* Le if ci-dessous ajouter un '\' en fin du tableau adresseNomBumExtensionFichier si celui-ci n'en a pas.
	* afin d'�viter que le dernier dossier envoyer ne soit pas compris comme un dossier mais comme une partie
	* du fichier � cr�er.
	*/
	if (adresseNomNumExtensionFichier[strlen(adresseNomNumExtensionFichier)-1] != '\\')
	{
		adresseNomNumExtensionFichier[strlen(adresseNomNumExtensionFichier) + 1] = '\0';
		adresseNomNumExtensionFichier[strlen(adresseNomNumExtensionFichier)] = '\\';
	}

	//Concat�nation de toute les donn�es dans un tableau de char (adresse, nom, num�ro et extension de fichier).
	strcat(adresseNomNumExtensionFichier, nomFichier);
	strcat(adresseNomNumExtensionFichier, numFichierChar);
	strcat(adresseNomNumExtensionFichier, extension);

	//Cr�ation et d�but d'�dition du fichier en binaire avec le param�tre tableau qui contient (adresse, nom, num�ro et extension de fichier).
	*fichierEcriture = fopen(adresseNomNumExtensionFichier, "wb");

	//V�rifie la bonne cr�ation et �dition du fichier en binaire. 
	//Si une erreur survient alors le programme s'arr�te avec l'erreur 4
	if (fichierEcriture == NULL)
	{
		free(adresseNomNumExtensionFichier);
		printf("Output file %s\n"
			"Error: No such file\n"
			"\n", adresseEcriture);
		exit(4);
	}

	//Lib�ration des m�moire allou�es dynamiquement
	free(numFichierChar);
	free(adresseNomNumExtensionFichier);
}




/*-------------------------------------------------------FONCTION AFFICHAGE CODE FICHIER LU---------------------------------------------------------*/

void affichageCodeHexaAvecInterface(const int *nbOctetParLigne,char adresseFichierLu[], FILE* fichierLu)
{
	/*
	* Fonction qui affiche l'interface et le code hexad�cimale du ficheir ouvert par l'appel de deux fonctions
	*/

	afficherInterfaceCodeHexa(nbOctetParLigne, adresseFichierLu);

	afficherCodeHexa(fichierLu, nbOctetParLigne);
}

void afficherInterfaceCodeHexa(const long *nbOctetParLigne, const char adresseFichier[])
{
	/*
	* Fontion qui affiche l'interface d'ecriture du d�but du code hexa
	*/
	printf("\nThe hexa code of the file %s is :\n\n", adresseFichier);
	printf("Offset (h)\t");

	for (int i = 0; i < *nbOctetParLigne; i++)//Ecrit le num�ro de chaque colonne en fonction du nombre d�finie en param�tre
	{
		printf("%02X ", i);
	}
	printf("\n\n");
}

void afficherCodeHexa(FILE* fichierLu, const int* nbOctetParLigne)
{
	/*
	* Fonction qui affiche le code hexd�cimale avec le num�ro de ligne du fichier lu envou� en param�tre de la fonction.
	*/

	long numeroLigne = 0;
	unsigned char octetLu;
	int nbOctetLigneEcrit = 0;

	printf("%00000008X\t", numeroLigne);//Affichage premi�re ligne

	while (!feof(fichierLu)) // On continue tant qu'on ai pas � la fin du fichier
	{
		//On affiche avant de lire le premier caract�re pour �viter d'�crire le caract�re de fin de fichier en hexa "FF" et en texte "�"
		octetLu = fgetc(fichierLu); // On lit le caract�re

		if (!feof(fichierLu))//Evite d'afficher l'octet de fin de fichier
		{
			if (nbOctetLigneEcrit >= *nbOctetParLigne)//Si on a attteint le bout de la ligne (nbOctetParLigne) alors on passe � la ligne
			{
				nbOctetLigneEcrit = 0;
				printf("\n");
				numeroLigne += *nbOctetParLigne;//Incr�mentation du num�ro de ligne par rapport au nombre d'octet par ligne
				printf("%00000008X\t", numeroLigne);
			}
			printf("%02X ", octetLu); // On l'affiche en hexad�cimale avec deux caract�res et en majuscule s'il y a des lettres
		}

		nbOctetLigneEcrit++;//Incr�mentation du nombre d'octet �crit � la ligne actuelle
		
	}
	printf("\n\n");
}



/*------------------------------------------------------------FONCTION DE RECHERCHE DE FICHIERS DANS UN FICHIER---------------------------------*/

void rechercherFichiersDansFichier(FILE* fichierLu, const char adresseEcritureFichiers[], const unsigned char chaineEnTete[], const int tailleChaineEnTete, const unsigned char chaineEnFin[], const int tailleChaineEnFin, const char nomFichiers[], const char extension[])
{
	unsigned char octetLu;//Octet de r�ception des donn�es lu dans fichierLu
	unsigned int numfichierPaterneTrouve = 0;//Num�ro du fichier � indiqu� dans le nom lors de sa cr�ation
	
	//Donn�e m�moire qui informe combien d'octet nous avons pass� apr�s la derni�re en-t�te afin 
	//de repartir en dessous une fois le pr�cdent fichier en train d'�tre cr�� finsi (le temps de trouv� l'en-fin)
	unsigned long nbOctetApresEnTete = 0;

	int finTrouve = 0;//Bool�en qui indique si l'en-fin du fichier dont on a pr�alablement trouv� l'en-t�te a �t� trouv�


	//unsigned char chaineLuChercheEnTete[sizeof(chaineEnTete)];
	unsigned char* chaineLuChercheEnTete = malloc(tailleChaineEnTete*sizeof(char));
	chaineLuChercheEnTete[tailleChaineEnTete - 1] = '\0';

	//unsigned char chaineLuChercheEnFin[sizeof(chaineEnFin)];
	unsigned char* chaineLuChercheEnFin = malloc(tailleChaineEnFin * sizeof(char));
	chaineLuChercheEnFin[tailleChaineEnFin - 1] = '\0';

	rewind(fichierLu);

	while (!feof(fichierLu)) // On continue tant qu'on ai pas � la fin du fichier
	{
		//On affiche avant de lire le premier caract�re pour �viter d'�crire le caract�re de fin de fichier en hexa "FF" et en texte "�"
		octetLu = fgetc(fichierLu); // On lit le caract�re
		pileDecalage(chaineLuChercheEnTete, &octetLu, tailleChaineEnTete);// Rajoute en fin de pile le dernier octet lu

		if (0 == compareTableaux(chaineLuChercheEnTete, chaineEnTete, tailleChaineEnTete))//Si la chaine d'en-t�te est trouv� dans le fichier lu
		{
			numfichierPaterneTrouve++;//Incr�mentation du nombre de fichier de ce type trouv�

			FILE* fichierEcriture = NULL;//Cr�ation d'un objet de type fichier
			
			//Cr�ation d'un fichier et d�but d'�criture en binaire dedans
			ouvreFichierEcritureEtTest(&fichierEcriture, adresseEcritureFichiers, nomFichiers, &numfichierPaterneTrouve, extension);

			//Ecriture de l'en-t�te dans le ficheir cr��
			fputs(chaineEnTete, fichierEcriture);

			//Le while ci-dessous �crit chaque octet lu dans fichierLu dans fichierEcriture jusqu'� trouv� l'en-fin
			while (!feof(fichierLu) && finTrouve == 0)//Tant qu'on est pas � la fin du fichier et que finTrouve=0
			{
				octetLu = fgetc(fichierLu); // On lit le caract�re

				nbOctetApresEnTete++;//Incr�mentation du nombre de caracteres pass�s apr�s l'en-t�te pr�c�demment trouv�e

				fputc(octetLu, fichierEcriture);//Ecrit l'octet lu dans le fichierEcrtiture

				pileDecalage(chaineLuChercheEnFin, &octetLu, tailleChaineEnFin);//Rajoute en fin de pile le dernier octet lu

				if (0 == compareTableaux(chaineLuChercheEnFin, chaineEnFin,tailleChaineEnFin))//Si l'en-fin est trouv�e
				{
					finTrouve = 1;//finTrouve est mis � 1 afin de quitter le while
				}
			}
			finTrouve = 0;
			fclose(fichierEcriture);//fertmeture du fichier �criture
			fseek(fichierLu, (-1) * nbOctetApresEnTete, SEEK_CUR);//Place le curseur apr�s l'en-t�te pr�c�demment trouv�e
			nbOctetApresEnTete = 0;//r�initialisation de la valeur de replacement car on vient de se replacer apr�s l'en-t�te du fichier pr�c�dent
		}
	}
	rewind(fichierLu);//Remet le curseur au d�but du fichier

	//Lib�ration de la m�moire allou�e dynamiquement
	free(chaineLuChercheEnTete);
	free(chaineLuChercheEnFin);

	printf("Number of %s file extracted : %d\n", extension, numfichierPaterneTrouve);
}


/*------------------------------------------------------------FONCTION AUXILIAIRES------------------------------------------------------*/

int compareTableaux(const unsigned char tab1[], const unsigned char tab2[], const int tailleTableaux)
{
	//Pour les fichiers avec un 0x00 il y a un probl�me car c'est compris comme un \0 donc la fin d'un tableau.
	//Pour palier ce probl�me une fonction manuelle qui compare en-tete avec l'en-tete lue et l'en-fin et l'en-fin lue
	//en utilisant la taille des tableaux et non la fin du tableau avec \0 que fait strcmp
	//Comme �a tous les caract�res y compris les \0 seront compar�s

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
	* Fonction qui parcours la tableau et d�cale les valeurs des plus 2 dans 1, 3 dans 2 etc... (du haut au bas de la pile)
	* Ensuite la nouvelle valeur est mise tout en haut de la pile.
	*/

	for (int i = 0; i < tailleTab - 2; i++)
	{
		tab[i] = tab[i + 1];
	}
	tab[tailleTab - 2] = *nouvelleValeur;
}