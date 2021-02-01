#include "gestionFichier.h"

//C'est moi qui dis des conneries pas toi !!!
/*------------------------------------------------------FONCTION D'AFFICHAGE AIDE-----------------------------------------*/

void afficherCommandes(const char nomFichier[], const char version[])
{
	/*
	* Fonction qui affiche l'aide du programme
	*/
	printf("\n"
		"filesFinder %s\n"
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
		"\n", version, nomFichier);
}




/*-------------------------------------------------------FONCTION AFFICHAGE CODE FICHIER LU---------------------------------------------------------*/

void affichageCodeHexaAvecInterface(const int* nbOctetParLigne, char adresseFichierLu[], FILE* fichierLu)
{
	/*
	* Fonction qui affiche l'interface et le code hexad�cimale du ficheir ouvert par l'appel de deux fonctions
	*/

	afficherInterfaceCodeHexa(nbOctetParLigne, adresseFichierLu);

	afficherCodeHexa(fichierLu, nbOctetParLigne);
}

void afficherInterfaceCodeHexa(const int* nbOctetParLigne, const char adresseFichier[])
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




/*------------------------------------------------------FONTIONS TEST BON FONCTIONNEMENT---------------------------------------*/

void verifNbParametresSuffisant(const int* nbParam, const int nbParamMin, const char nomFichier[], const char version[])
{
	/*
	* Fonction qui affiche un message d'erreur et l'aide dans le cas o� le nombre de param�tre n'est pas suffisant
	*
	* Si le nombre de param�tre est insuffisant l'aide s'affiche et le programme s'arr�te avec le message d'erreur 2
	*/
	if (*nbParam < nbParamMin)
	{
		printf("Not enough parameters.\n");
		afficherCommandes(nomFichier, version);
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

void ouvreFichierLectureEtTest(FILE** fichierLecture, const char adresseEntree[])
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

void ouvreFichierEcritureEtTest(FILE** fichierEcriture, const char adresseEcriture[], const char nomFichier[], const int* numFichier, const char extension[])
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
	char* numFichierChar = malloc((nombreChiffre + 1) * sizeof(char));

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
	char* adresseNomNumExtensionFichier = malloc((strlen(adresseEcriture) + strlen(nomFichier) + sizeof(numFichierChar) + strlen(extension)) * sizeof(char));

	//Appel de la fonction de test du focntionnement correcte de l'allocation dynamique
	allocationDynamiqueTest(adresseNomNumExtensionFichier);

	adresseNomNumExtensionFichier[0] = '\0';
	strcat(adresseNomNumExtensionFichier, adresseEcriture);

	/*
	* Le if ci-dessous ajouter un '\' en fin du tableau adresseNomBumExtensionFichier si celui-ci n'en a pas.
	* afin d'�viter que le dernier dossier envoyer ne soit pas compris comme un dossier mais comme une partie
	* du fichier � cr�er.
	*/
	if (adresseNomNumExtensionFichier[strlen(adresseNomNumExtensionFichier) - 1] != '\\')
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
	unsigned char* chaineLuChercheEnTete = malloc(tailleChaineEnTete * sizeof(char));
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

				if (0 == compareTableaux(chaineLuChercheEnFin, chaineEnFin, tailleChaineEnFin))//Si l'en-fin est trouv�e
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
