#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define RATIO 0.5

int nextPerm(int * table, int n) //funkcja ustawia kolejna permutacje tablicy
{
	int Aindex = -1; //indeks elementu A
	for (int i = n - 2; i >= 0; --i) //szukamy od prawej strony pierwszego elementu A mniejszego niz elementy po prawej
	{
		if (table[i] < table[i + 1]) //jesli aktualny element jest mniejszy od elementu po prawej:
		{
			Aindex = i; //zapisz indeks w zmiennej
			break; //przerwij petle
		}
	}
	if (Aindex == -1) return 0; //element A nie zostal znaleziony - brak kolejnej permutacji
	
	int min = n;
	int Bindex;
	for (int i = Aindex + 1; i < n; i++) //gdy juz znajdziemy element A, idziemy od A na prawo i szukamy najmniejszego z wiekszych od A
	{
		if (table[i] > table[Aindex])
		{
			if (table[i] <= min)
			{
				Bindex = i;
				min = table[i];
			}
		}
	}
	//zamieniamy miejscami te dwa elementy:
	int temp = table[Bindex];
	table[Bindex] = table[Aindex];
	table[Aindex] = temp;

	//odwroc kolejnosc na prawo od Aindex
	int span = (n - 1 - Aindex + 1) / 2; //span = odleglosc miedzy elementem A a prawym koncem tablicy
	for (int i = 0; i < span; i++)
	{
		//zamiana miejscami:
		int temp = table[Aindex + 1 + i];
		table[Aindex + 1 + i] = table[n - 1 - i];
		table[n - 1 - i] = temp;
	}
	return 1; //zwracamy 1 bo udalo sie wygenerowac kolejna permutacje
}

void quicksort(int * table, int left, int right, int * compCounter) //zwykly quicksort ktory zlicza operacje porownan w zmiennej compCounter
{
	int i = left;
	int j = right;
	int x = table[(left + right) / 2]; //piwot na srodku
	do
	{
		while (table[i] < x)
		{
			i++;
			(*compCounter)++;
		}
		while (table[j] > x)
		{
			j--;
			(*compCounter)++;
		}
		if (i <= j)
		{
			int temp = table[i];
			table[i] = table[j];
			table[j] = temp;
			i++;
			j--;
			(*compCounter)++;
		}
		(*compCounter)++;
	} while (i <= j);
	if (left < j)
	{
		(*compCounter)++;
		quicksort(table, left, j, compCounter);
	}
	if (right > i)
	{
		(*compCounter)++;
		quicksort(table, i, right, compCounter);
	}
}

int quicksortTester(const int * const tab, const int n) //funkcja testujaca dana permutacje - zwraca 1 jesli nie udalo sie posortowac ponizej n2, 0 jesli sie udalo
{
	int * table = malloc(sizeof(int) * n); //kopiujemy tablice (zeby nie modyfikowac oryginalnej permutacji)
	for (int i = 0; i < n; i++)
		table[i] = tab[i];
	int compCounter = 0;
	int compLimit = n * n * RATIO; //obliczamy limit porownan
	quicksort(table, 0, n - 1, &compCounter); //odpalamy quicksort
	free(table); //usuwamy tablice
	printf("%d/%d porownan. ", compCounter, compLimit);
	if (compCounter > compLimit) return 1;
	else return 0;
}

int main()
{
	FILE * input;
	input = fopen("dane3.txt", "r"); //wczytujemy dane z pliku wejsciowego
	int n;
	if (input == NULL) //jesli plik nie zostal otwarty:
	{
		printf("Blad czytania pliku. Zamykam program.\n");
		return -1;
	}
	else //jesli plik zostal otwarty:
	{
		char * n_string = malloc(3 * sizeof(char));
		fgets(n_string, 3, input); //czytamy 2 cyfry z pliku
		fclose(input); //zamykamy plik
		n = atoi(n_string); //parsujemy cyfry na liczbe
		free(n_string); //usuwamy tablice cyfr
		if (n < 1 || n > 12) //jesli dostalismy niepoprawna liczbe:
		{
			printf("Zla wartosc n. Zamykam program.\n");
			return -2;
		}
		else //jesli dostalismy poprawna wartosc:
		{
			printf("Wartosc wczytana z pliku: %d.\n", n);
		}
	}
	int * Table = malloc(n * sizeof(int)); //tworzymy tablice
	for (int i = 0; i < n; i++) Table[i] = i + 1; //wypelniamy tablice pierwsza permutacja (1, 2, 3, 4, 5, ...)
	long int permutationNumber = 1;
	for (int i = 1; i <= n; i++) permutationNumber *= i; //obliczamy silnie z n
	printf("%ld permutacji zostanie sprawdzonych.\n", permutationNumber);
	if (n > 8) printf("UWAGA! Obliczenia moga trwac bardzo dlugo.");
	printf("Rozpoczynam obliczenia...\n");
	FILE * output;
	output = fopen("wynik3.txt", "w"); //tworzymy plik wynikowy
	if (output == NULL)
	{
		printf("Blad tworzenia pliku. Zamykam program.\n"); //jesli blad tworzenia pliku
		return -3;
	}
	else
	{
		for (long int i = 0; i < permutationNumber; i++) //w petli sprawdzamy wszystkie permutacje
		{
			printf("\tPermutacja %d ", i + 1);
			printf("z %ld: ", permutationNumber);
			if (quicksortTester(Table, n) == 0) //jesli znajdziemy permutacje z czasem sortowania PONIZEJ n^2:
			{
				printf("[KORZYSTNA PERMUTACJA: "); //drukuj permutacje na ekran
				for (int i = 0; i < n; i++)
					printf("%d ", Table[i]);
				printf("]\n");
			}
			else //jesli znajdziemy permutacje z czasem sortowania POWYZEJ n^2:
			{
				printf("\n");
				for (int i = 0; i < n; i++)
					fprintf(output, "%d ", Table[i]); //drukuj permutacje do pliku wynikowego
				fprintf(output, "\n");
			}
			nextPerm(Table, n); //wygeneruj nastepna permutacje
		}
		printf("...obliczenia zakonczone.");
		fclose(output); //zamknij plik
	}
	free(Table); //usun tablice
	return 0;
}