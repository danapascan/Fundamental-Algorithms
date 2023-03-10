#include <stdlib.h>
#include<stdio.h>
#include "Profiler.h"


Profiler p("second");
#define MAX_SIZE 10000
#define STEP_SIZE 100
//#define NR_TEST 5


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


void worst_case()
{
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		int aux[MAX_SIZE];
		FillRandomArray(v, n, 10, 50000, false, 2);
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
	p.createGroup("worst-case-total","total-bub", "total-sel", "total-ins");
	p.createGroup("worst-case-comp","comp-bub", "comp-sel", "comp-ins");
	p.createGroup("worst-case-atr", "atr-bub", "atr-ins");
	p.showReport();
}
int main()
{
	worst_case();
	return 0;
}