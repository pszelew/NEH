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


int utworz_graf_prawo(int i, int k, int pi[], zadanie zadania[], int prawo[][21])
{
	if (i == 0 || k == 0)
		return 0;
	if (prawo[i][k] != 0)
		return prawo[i][k];
	prawo[i][k] = max(utworz_graf_prawo(i - 1, k, pi, zadania, prawo), utworz_graf_prawo(i, k - 1, pi, zadania, prawo)) + zadania[pi[i - 1]].mach[k];
	return prawo[i][k];
}



int utworz_graf_lewo(int x, int y, int i, int k, int pi[], zadanie zadania[], int lewo[][21])
{
	if (x == i || y == k)
		return 0;
	if (lewo[x][y] != 0)
		return lewo[x][y];
	
	lewo[x][y] = max(utworz_graf_lewo(x+1, y, i, k, pi, zadania, lewo), utworz_graf_lewo(x, y+1, i, k, pi, zadania, lewo)) + zadania[pi[x]].mach[y+1];
	return lewo[x][y];
}



int tnij_graf(int prawo[][21], int lewo[][21], int pos, zadanie zad, int m)
{
	int temp[21];
	int max_temp=0;
	temp[0] = 0;
	for (int i = 1; i < m+1; i++)
	{
		temp[i] = max(temp[i - 1], prawo[pos][i]) + zad.mach[i];
	}

	for (int i = 0; i < m; i++)
	{
		max_temp = max(temp[i+1]+lewo[pos][i], max_temp);
	}

	return max_temp;
}



int main()
{
	int n,m;

	zadanie tab[1000];
	zadanie tab_sort[1000];
	int kolejnosc[1000];
	int kolejnosc_temp[1000];
	int best_cost = INT_MAX;
	int best_pos;
	int temp_cost=INT_MAX;
	int chosen_cost = INT_MAX;
	int temp=0;
	int c_hist[501][21];
	int graf_prawo[501][21];
	int graf_lewo[501][21];
	int x = 0, y = 0;

	auto temp_dur=0;

	for (int i = 0; i < 501; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			graf_prawo[i][j] = 0;
			graf_lewo[i][j] = 0;
		}
	}



	for (int i = 0; i < 1000; ++i)
	{
		kolejnosc[i] = 0;
		kolejnosc_temp[i] = 0;
	}


	for (int g = 0; g < 121; g++)
	{
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

		auto t3 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < n; ++i)
		{
			//dla kazdej z mozliwych pozycji do wlozenia
			for (int j = 0; j < i + 1; ++j)
			{
				//sprawdzmy wszystkie mozliwe pozycje i wybierzmy najlepsza dla nas

				temp_cost = tnij_graf(graf_prawo, graf_lewo, j, tab_sort[i],m);

				if (temp_cost < chosen_cost)
				{
					chosen_cost = temp_cost;
					best_pos = j;
				}
			
			}

			//wstawiam na serio tam gdzie bylo najlepiej 	
			kolejnosc_temp[best_pos] = tab_sort[i].id;
			//dopelniamy tablice
			for (int k=0,z = 0; k < i + 1; ++k, ++z)
			{
				if (k == best_pos)
					++k;
				kolejnosc_temp[k] = kolejnosc[z];
			}

			for (int h = 0; h < 1000; h++)
			{
				kolejnosc[h] = kolejnosc_temp[h];
			}


			//tutaj zerujemy tylko odpowiednie wartosci dla prawej
			for (int i = best_pos; i < n+1; i++)
			{
				for (int j = 0; j < m+1; j++)
				{
					graf_prawo[i][j] = 0;
				}
			}

			//tutaj zerujemy tylko odpowiednie wartosci dla lewej
			for (int i = 0; i <= best_pos; i++)
			{
				for (int j = 0; j < m+1; j++)
				{
					graf_lewo[i][j] = 0;
				}
			}


			//utworzenie grafow, dla naszej kolejnosci
			utworz_graf_prawo(i + 1, m, kolejnosc, tab, graf_prawo);
			utworz_graf_lewo(x, y, i + 1, m, kolejnosc, tab, graf_lewo);

			best_cost = chosen_cost;
			chosen_cost = INT_MAX;
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
