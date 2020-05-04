// SimpleNeh.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <queue> 
#include <vector>
#include <chrono>

using namespace std;
struct zadanie
{
	int id;
	int mach[99];
	int waga;
};

int compare_quick(const void* a, const void* b)
{

	if ((*(zadanie*)a).waga < (*(zadanie*)b).waga) return 1;
	if ((*(zadanie*)a).waga == (*(zadanie*)b).waga)
	{
		if ((*(zadanie*)a).id < (*(zadanie*)b).id) return -1;
		else return 1;
	}
	if ((*(zadanie*)a).waga > (*(zadanie*)b).waga) return -1;
}


int policzWage(zadanie zad, int m)
{
	int temp = 0;
	for (int j = 0; j < m+1; ++j)
	{
		temp += zad.mach[j];
	}
	return temp;
}


//moment zakonczenia i-tego zadania k-tej na maszynie dla kolejnosci pi
int C(int i, int k, int pi[], zadanie zadania[], int c_hist[][21])
{
	if (i == 0 || k == 0)
		return 0;
	if (c_hist[i][k] != 0)
		return c_hist[i][k];
	
	c_hist[i][k] = max(C(i - 1, k, pi, zadania, c_hist), C(i, k - 1, pi, zadania, c_hist)) + zadania[pi[i - 1]].mach[k];
	return c_hist[i][k];
}





int main()
{
	int n,m;

	zadanie tab[1000];
	zadanie tab_sort[1000];
	int kolejnosc[1000];
	int kolejnosc_temp[1000];
	int kolejnosc_best_temp[1000];
	int best_cost = 0;
	int temp_cost=INT_MAX;
	int temp;
	int c_hist[501][21];

	auto temp_dur=0;

	for (int i = 0; i < 1000; ++i)
	{
		kolejnosc[i] = 0;
		kolejnosc_temp[i] = 0;
	}


	for (int g = 0; g < 121; g++)
	{
		auto t3 = std::chrono::high_resolution_clock::now();
		ifstream data("neh.data.txt");
		string nazwa;
		string s = "";
		if (g < 10)
			nazwa = "data.00" + to_string(g) + ":";
		else if (g < 100)
			nazwa = "data.0" + to_string(g) + ":";
		else if (g < 1000)
			nazwa = "data." + to_string(g) + ":";
		while (s != nazwa)
		{
			data >> s;
		}
		data >> n;
		data >> m;

		for (int j = 0; j < m + 1; ++j)
		{
			tab[0].mach[j] = 0;
		}


		for (int i = 0; i < n + 1; ++i)
		{
			tab[i].mach[0] = 0;
		}
		
		tab[0].id = 0;
		tab[0].waga = 0;
		
		for (int i = 1; i < n+1; ++i)
		{
			tab[i].id = i;
			for (int j=1; j < m+1; j++)
			{
				data >> tab[i].mach[j];
			}

			tab[i].waga = policzWage(tab[i], m);
		}


		for (int i = 0; i < 1000; i++)
		{
			tab_sort[i] = tab[i];
		}


		//posortowanie tablicy zadan wedlug wagi
		qsort(tab_sort, n+1, sizeof(zadanie), compare_quick);

		//w tej petli bedziemy znajdowac na jakie miejsce wlozyc nasze zadanie
		//dla kazdego zadania
		for (int i = 0; i < n; ++i)
		{
			
			//dla kazdej z mozliwych pozycji do wlozenia
			for (int j = 0; j < i + 1; ++j)
			{
				//teraz sprawdzmy ile wynosi koszt uszeregowania dla takiej tablicy

				//wrzucamy nasze zadanie na miejsce j
				kolejnosc_temp[j] = tab_sort[i].id;
				//dopelniamy tablice

				for (int k=0,z = 0; k < i + 1; ++k, ++z)
				{
					if (k == j)
						++k;
					kolejnosc_temp[k] = kolejnosc[z];
				}
				//auto t1 = std::chrono::high_resolution_clock::now();
				temp = C(i + 1, m, kolejnosc_temp, tab, c_hist);
				//auto t2 = std::chrono::high_resolution_clock::now();
				//temp_dur += chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
				


				for (int i = 0; i < 501; i++)
				{
					for (int j = 0; j < 21; j++)
						c_hist[i][j] = 0;
				}
		
				
				if (temp < temp_cost)
				{
					for (int h = 0; h < 1000; h++)
					{
						kolejnosc_best_temp[h] = kolejnosc_temp[h];
					}

					best_cost = temp;
					temp_cost = temp;
				}
			
			}
			//auto t4 = std::chrono::high_resolution_clock::now();

			//cout << "Dur:" << temp_dur << endl;
			//cout << "Dur_2: " << chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << endl;
			temp_dur = 0;


			for (int h = 0; h < 1000; h++)
			{
				kolejnosc[h] = kolejnosc_best_temp[h];
			}

			temp_cost = INT_MAX;
		}
		
		auto t4 = std::chrono::high_resolution_clock::now();

		cout << nazwa  << endl;
		cout << "Czas trwania: " << chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count()<<"ms" << endl;
		cout << "neh:" << endl;
		cout << best_cost<< endl;

		for (int h = 0; h < n; ++h)
		{
			cout << kolejnosc[h]<< " ";
		}
		cout << endl << endl;
	}
}
// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
