#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
	int codGIOCATORE;
	int codSQUADRA;
	char Tipo[11];
	int Costo;
} TipoOperazione;

typedef struct {
	int codSQUADRA;
	char Nome[21];
	char Paese[21];
} TipoSquadra;


char NFSQUADRA[] = "SQUADRA.dat";
char NFOPERAZIONE[] = "OPERAZIONE.dat";


void CreaFileSQUADRA() {
	FILE* FS;


	TipoSquadra S[6] = {
		{ 1, "JUVENTUS", "ITALIA" },
		{ 2, "NAPOLI", "ITALIA" },
		{ 3, "PSG", "FRANCIA" },
		{ 4, "REAL MADRID", "SPAGNA" },
		{ 5, "BARCELLONA", "SPAGNA" },
		{ 6, "CHELSEA", "INGHILTERRA" }
	};


	FS = fopen(NFSQUADRA, "wb");
	fwrite(S, sizeof(TipoSquadra), 6, FS);
	fclose(FS);
}

void CreaFileOPERAZIONE() {
	FILE* FO;

	TipoOperazione O[10] = {
		{ 101, 6, "ACQUISTO", 200 },
		{ 102, 6, "ACQUISTO", 400 },
		{ 103, 3, "PRESTITO", 70 },
		{ 104, 4, "ACQUISTO", 150 },
		{ 105, 1, "PRESTITO", 50 },
		{ 106, 3, "SVINCOLATO", 20 },
		{ 107, 1, "ACQUISTO", 300 },
		{ 108, 2, "PRESTITO", 100 },
		{ 109, 5, "PRESTITO", 40 },
		{ 110, 6, "SVINCOLATO", 10 }
	};


	FO = fopen(NFOPERAZIONE, "wb");
	fwrite(O, sizeof(TipoOperazione), 10, FO);
	fclose(FO);
}


void mostraSQUADRA() {
	FILE* FS;
	TipoSquadra S;

	FS = fopen(NFSQUADRA, "rb");
	printf("LISTA SQUADRE\n");
	while (fread(&S, sizeof(TipoSquadra), 1, FS) > 0)
		printf("%5d %20s %20s\n", S.codSQUADRA, S.Nome, S.Paese);

	fclose(FS);
}


void mostraOPERAZIONE() {
	FILE* FO;
	TipoOperazione O;

	FO = fopen(NFOPERAZIONE, "rb");
	printf("LISTA OPERAZIONI\n");
	while (fread(&O, sizeof(TipoOperazione), 1, FO) > 0)
		printf("%5d %5d %10s %5d\n", O.codGIOCATORE, O.codSQUADRA, O.Tipo, O.Costo);

	fclose(FO);
}

//MIE FUNZIONI

void MostraAcquistiPaese(char Paese[]);

void MostraSquadra(char Tipo[]);

TipoOperazione *VettoreDinamico(int CostoBase, int *DimVD);

//void StampVD(int indirizzoVD);//per la stampa del vettore

void ModificaOperazione(char TipoPiu[], char TipoTogli[]);



//COSTRUISCO LE MIE FUNZIONI

void MostraAcquistiPaese(char Paese[])
{
	//dato un paese stampo per ogni squadra del paese dato il numero totale delleoperazioni di tipo ACQUISTO 
	int NumAcquisti = 0;
	int FirstPrintBOOL = 1;

	FILE *FO, *FS;

	TipoOperazione Operazione;
	TipoSquadra Squadra;

	FO = fopen(NFOPERAZIONE, "rb");
	FS = fopen(NFSQUADRA, "rb");

	//controllo tutte lke squadre in cerca della nazione giusta

	while (fread(&Squadra, sizeof(TipoSquadra), 1, FS) > 0)
	{
		if (strcmp(Squadra.Paese, Paese) == 0)//se trovo del paese giusto
		{
			while (fread(&Operazione, sizeof(TipoOperazione), 1, FO) > 0)
			{
				if ((Operazione.codSQUADRA == Squadra.codSQUADRA) && (strcmp(Operazione.Tipo, "ACQUISTO") == 0))
				{
					//se trovo una operazione di tipo acquisto dalla squadra giusta allora incremento il contatore
					NumAcquisti++;
				}
			}
			//una volta scansionato tutto il file operazioni riparto da capo per un eventuale altro controllo
			fseek(FO, 0, SEEK_SET);
			//ora eventualmente stampo
			if ((FirstPrintBOOL) && (NumAcquisti != 0))
			{
				printf("\n\ncodSQUADRA   NumAcquisti");
				//dato che lo devo stampare una sola volta mi chiudo fuori
				FirstPrintBOOL = 0;
			}
			if ((!FirstPrintBOOL))
			{
				printf("\n%d            %d", Squadra.codSQUADRA, NumAcquisti);
			}
			NumAcquisti = 0;
		}
	}
	fclose(FO);
	fclose(FS);
}

void MostraSquadra(char Tipo[])
{
	//dato un tipo stampa a video tutte le quadre riportandone anche il costro totale cdelle operazioni del TIPO DATO e anche il costro totale degli altri tipi
	int CostTIPO = 0;
	int CostNOTIPO = 0;
	int FirstPrintBOOL = 1;
	int Moltiplica;

	FILE *FO, *FS;

	TipoOperazione Operazione;
	TipoSquadra Squadra;

	FO = fopen(NFOPERAZIONE, "rb");
	FS = fopen(NFSQUADRA, "rb");

	while (fread(&Squadra, sizeof(TipoSquadra), 1, FS) > 0)
	{
		{
			while (fread(&Operazione, sizeof(TipoOperazione), 1, FO) > 0)
			{
				if ((strcmp(Operazione.Tipo, Tipo) == 0) && (Operazione.codSQUADRA == Squadra.codSQUADRA))
				{
					//se trovo una operazione di tipo dato dalla squadra giusta allora incremento il contatore
					if (strcmp(Operazione.Tipo, "ACQUISTO")==0)
					{
						Moltiplica = 4;
					}
					if (strcmp(Operazione.Tipo, "PRESTITO") == 0)
					{
						Moltiplica = 2;
					}
					if (strcmp(Operazione.Tipo, "SVINCOLATO") == 0)
					{
						Moltiplica = 1;
					}

					CostTIPO = CostTIPO + (Operazione.Costo*Moltiplica);
				}
				if ((strcmp(Operazione.Tipo, Tipo) != 0) && (Operazione.codSQUADRA == Squadra.codSQUADRA))
				{
					//se trovo una operazione di tipo diverso da quello datro dalla squadra giusta allora incremento il contatore
					if (strcmp(Operazione.Tipo, "ACQUISTO") == 0)
					{
						Moltiplica = 4;
					}
					if (strcmp(Operazione.Tipo, "PRESTITO") == 0)
					{
						Moltiplica = 2;
					}
					if (strcmp(Operazione.Tipo, "SVINCOLATO") == 0)
					{
						Moltiplica = 1;
					}

					CostNOTIPO = CostNOTIPO + (Operazione.Costo*Moltiplica);
				}
			}
			//mi riporto all'inizio del file per la prossima squadra
			fseek(FO, 0, SEEK_SET);
			//e ora stampo
			if ((FirstPrintBOOL))
			{
				printf("\n\ncodSQUADRA  Nome          Paese         TotaleTipo           TotaleAltriTipi");
				//dato che lo devo stampare una sola volta mi chiudo fuori
				FirstPrintBOOL = 0;
			}
			if ((!FirstPrintBOOL))
			{
				printf("\n%d            %s            %s             %d                  %d", Squadra.codSQUADRA, Squadra.Nome, Squadra.Paese, CostTIPO, CostNOTIPO);
			}
			//Resetto i contatori per il prossima squadra
			CostTIPO = 0;
			CostNOTIPO = 0;
		}
	}
	fclose(FO);
	fclose(FS);
}

TipoOperazione *VettoreDinamico(int CostoBase)
{
	FILE *FO, *FT;//FT è il mio file temporaneo

	TipoOperazione Operazione;
	TipoOperazione Temp;

	FO = fopen(NFOPERAZIONE, "rb");
	FT = fopen("TEMP.TXT", "wb");//poi faccio il rename? NO!!! VEDI ESERCIZIO SUCCESSIVO, SERVE ORIGINALE!!!

	TipoOperazione *VD;
	int Dim=0;//dimensione vettore dinamico
	int i=0;

	//CALCOLO LA DIMENSIONE
	while (fread(&Operazione, sizeof(TipoOperazione), 1, FO) > 0)
	{
		if (Operazione.Costo > CostoBase)//se devo fare una decurtazione aumento la memoria di 1
		{
			Dim++;
		}
	}
	//mi sposto all'inizio per i prossimi cicli
	fseek(FO, 0, SEEK_SET);
	//ora ho la dimensione e quindi lo alloco
	VD = (TipoOperazione*)malloc((Dim) * sizeof(TipoOperazione));

	while (fread(&Operazione, sizeof(TipoOperazione), 1, FO) > 0)
	{
		if (Operazione.Costo <= CostoBase)//se non devo decurtare nulla
		{
			//copio semplicemente in TEMP.dat denza modificare nulla e senza scrivere nulla nel vettore dinamico
			Temp = Operazione;
			fwrite(&Temp, sizeof(TipoOperazione), 1, FT);
		}

		if (Operazione.Costo > CostoBase)//se devo decurtare il 20%
		{
			//prima di fare modifiche salvo il record in VD
			VD[i] = Operazione;
			//gia che ci sono stampo pure a video
			printf("\n\n VD--> %d  %d  %s  %d", VD[i].codGIOCATORE, VD[i].codSQUADRA, VD[i].Tipo, VD[i].Costo);
			i++;//avanzo di uno in modo da prepararmi per la prossima scrittura
			Temp = Operazione;
			Temp.Costo = Operazione.Costo - (Operazione.Costo / 100 * 20);//decurto il 20%
			//e ora lo scrivo nel file temporaneo
			fwrite(&Temp, sizeof(TipoOperazione), 1, FT);
		}
	}
	fclose(FO);
	fclose(FT);
	remove(NFOPERAZIONE);
	rename("TEMP.dat", NFOPERAZIONE);
	return VD;
}

void ModificaOperazione(char TipoPiu[], char TipoTogli[])
{
	//modifica il file operazione.dat incrementando di 100 il costo del record con tipopiu e cancellando il record tipotogli
	
	int salta = 0;

	FILE *FO, *FTT;//FTT è il secondo file temporaneo

	TipoOperazione Operazione;
	TipoOperazione Temp1;

	FO = fopen(NFOPERAZIONE, "rb");
	FTT = fopen("TEMP1.dat", "wb");
	
	while (fread(&Operazione, sizeof(TipoOperazione), 1, FO) > 0)
	{
		if ((strcmp(Operazione.Tipo, TipoTogli) != 0) && (strcmp(Operazione.Tipo, TipoPiu) != 0) && (!salta))//se non è nessuno dei 2 tipi copio e basta senza modifiche
		{
			fwrite(&Temp1, sizeof(TipoOperazione), 1, FTT);
			salta = 1;
		}
		
		if ((strcmp(Operazione.Tipo, TipoPiu) == 0) && (!salta))//se è tipopiu
		{
			//aggiungo 100 e poi stampo
			Temp1 = Operazione;
			Temp1.Costo += 100;
			fwrite(&Temp1, sizeof(TipoOperazione), 1, FTT);
			salta = 1;
		}
		
		if ((strcmp(Operazione.Tipo, TipoTogli) == 0) && (!salta))//se è tipotogli
		{
			//allora non copio nulla 
			salta = 1;//inutile
		}
		salta = 0;
	}
	fclose(FO);
	fclose(FTT);

	remove(NFOPERAZIONE);
	rename("Temp1.dat", NFOPERAZIONE);
}


main() {

	int indirizzo;
	int DimVD = 0;
	/* Creazione dei due file */
	CreaFileSQUADRA();
	CreaFileOPERAZIONE();

	mostraSQUADRA();
	mostraOPERAZIONE();

	/* INIZIO VOSTRO PROGRAMMA */

		MostraAcquistiPaese("ITALIA");

		MostraSquadra("PRESTITO");

		indirizzo = VettoreDinamico(150, &DimVD);
//	mostraOPERAZIONE();
	//CreaFileOPERAZIONE();
		ModificaOperazione("ACQUISTO", "SVINCOLATO");
	//mostraOPERAZIONE();
	
	getchar();
	getchar();
	system("PAUSE");


} /* END MAIN */

