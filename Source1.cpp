#include <stdlib.h>
#include<stdio.h>
#include "Profiler.h"


Profiler p("second");
#define MAX_SIZE 10000
#define STEP_SIZE 100
 

//insertion sort
void insertionsort(int* v, int n)
{
	Operation opComp1 = p.createOperation("comp-ins", n);
	Operation opAttr1 = p.createOperation("atr-ins", n);

	int num;
	for (int i = 1; i < n; i++)
	{
		opAttr1.count();
		num = v[i];
		int j = i - 1;

		while (j >= 0 && v[j] > num)
		{
			opComp1.count();
			opAttr1.count();
			v[j + 1] = v[j];
			j = j - 1;
		}
		opComp1.count();
		opAttr1.count();
		v[j + 1] = num;
	}
}

//selection sort
void selectionsort(int* v, int n)
{
	Operation opComp2 = p.createOperation("comp-sel", n);
	Operation opAttr2 = p.createOperation("atr-sel", n);
	int poz;
	int aux;
	for (int i = 0; i < n - 1; i++)
	{
		int poz = i;
		for (int j = i + 1; j < n; j++)
		{
			opComp2.count();
			if (v[j] < v[poz])
			{
				poz = j;
			}
		}
		if (poz != i)
		{
			opAttr2.count(3);
			aux = v[i];
			//opAttr.count();
			v[i] = v[poz];
			///opAttr.count();
			v[poz] = aux;
		}

	}
}

//boubble sort
void boubblesort(int* v, int n)
{

	Operation opComp3 = p.createOperation("comp-bub", n);
	Operation opAttr3 = p.createOperation("atr-bub", n);

	for (int i = 0; i < n - 1; i++)
	{
		boolean sorted = true;
		for (int j = 0; j < n - 1; j++)
		{
			opComp3.count();
			if (v[j] > v[j + 1])
			{
				int aux;
				opAttr3.count();
				aux = v[j];
				opAttr3.count();
				v[j] = v[j + 1];
				opAttr3.count();
				v[j + 1] = aux;
				sorted = false;
			}
		}
		if (sorted)
			break;
	}
}

void best_case()
{
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		int aux[MAX_SIZE];
		FillRandomArray(v, n, 10, 50000, false, 1);
		for (int i = 0; i < n; i++)
		{
			aux[i] = v[i];
		}
		boubblesort(aux, n);
		for (int i = 0; i < n; i++)
		{
			aux[i] = v[i];
		}
		selectionsort(aux, n);
		insertionsort(v, n);

	}
	p.addSeries("total-bub", "comp-bub", "atr-bub");
	p.addSeries("total-sel", "comp-sel", "atr-sel");
	p.addSeries("total-ins", "comp-ins", "atr-ins");
	//p.createGroup("best-case-total","total-bub", "total-sel", "total-ins");
	//p.createGroup("best-case-comp","comp-bub", "comp-sel", "comp-ins");
	p.createGroup("best-case-atr","atr-bub", "atr-sel", "atr-ins");
	p.showReport();
}
int main()
{
	best_case();
	return 0;
}

/*Dupa ce am facut graficele pentru best case atribuiri, comparatii si total
 am observat ca se suprapun in graficul pentru comparatii liniile de la selection si boubble
 asa ca a trebuit sa le pun pe grafice separate pentru a arata rezultatele corecte.
 La fel si pentru graficul de best total.

 Putem observa ca cel mai eficient algoritm de sortare dintre cele trei, in cel 
 mai bun caz, din punct de vedere al atribuirilor este selection sort si boubble sort, avand fiecare 0 atribuiri.

 Din punct de vedere al comparatilor, boublle si insertion sunt la fel de eficiente.
 La final, din totalul de la best case putem deduce ca boubble sort este cel mai eficient algoritm de sortare 
 in cel mai bun caz.

 Pentru cel mai rau caz la atribuiri si la comparatii dar si in total, algoritmul pentru selection sort este cel mai bun.
 La acest caz a trebuit si sa separam atribuirile pentru selection sort pentru a returna rezultatul corect in grafic

 Pentru cazul mediu, tot selection sort este cel mai bun.

 */