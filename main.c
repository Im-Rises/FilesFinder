#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VER "1.0"

/*--------------------------------------------LES TYPES D'ERREUR D'ARRET DU PROGRAMME------------------------------
* Les différents types de code erreur :
* 0 - Fonctionnement normal du programme.
* 1 - Manque de paramètres en entrée du programme pour un fonctionnement minmimum (-h).
* 2 - Mode/option inconnue.
* 3 - Fichier impossible à lire ou impossible d'écrire les fichiers (pas de fichier à lire ou de destination).
* 4 - Fichier impossible à créer et/ou ecrire dedans.
* 5 - S'il y a un manque de mémoire alors le programme s'arrête avec cette erreur (allocation dynamique).
*/


/*-----------------------------------------------FONCTION MAIN-------------------------------------------------------------------*/

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

	verifNbParametresSuffisant(&argc, 2, argv[0],VER);//Verification du nombre de paramètre en entrée supérieur ou égal à 2

	//Le code suivant s'occupe de la gestion des options
	if (argv[1][0] == '-')
	{
		switch (argv[1][1])
		{
		case ('h'):
			//Affiche l'aide du programme et le mode d'emploi
			afficherCommandes(argv[0],VER);
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
			afficherCommandes(argv[0], VER);
			exit(2);
			break;
		}
	}
	else
	{
		//Dans le cas où l'option est mal écrite on affiche la liste des commandes et l'usage
		afficherCommandes(argv[0], VER);
	}
	return 0;
}
