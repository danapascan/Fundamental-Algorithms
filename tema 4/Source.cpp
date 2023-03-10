#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("heap");
#define MAX_SIZE 1000
#define STEP_SIZE 10
#define NR_TESTE 5


void swap(int* a, int* b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

void heapify(int* arr, int n, int i, Operation* opComp2, Operation *opAttr2)
{
	int left = 2 * i + 1;
	int right = 2 * i +2;
	int root = i;
	opComp2->count(2);
	if (left < n && arr[left] > arr[root])
	{
		root = left;
	}
	if (right < n && arr[right] > arr[root])
	{
		root = right;
	}
	if (root != i)
	{
		swap(&arr[root], &arr[i]);
		opAttr2->count(3);
		heapify(arr, n, root,opComp2, opAttr2);

	}
}

void heapsort(int* arr, int n)
{
	Operation opComp2 = p.createOperation("comp-heap", n);
	Operation opAttr2 = p.createOperation("atr-heap", n);

	for (int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(arr, n, i,&opComp2, &opAttr2);
	}

	for (int i = n - 1; i >= 0; i--)
	{
		opAttr2.count(3);
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0, &opComp2, &opAttr2);
	}

}


int partition(int* arr, int l, int r, int n)
{
	Operation opComp1 = p.createOperation("comp-part", n);
	Operation opAttr1 = p.createOperation("atr-part", n);

	int x = arr[r];
	opAttr1.count();
	int i = l - 1;
	for (int j = l; j <= r - 1; j++)
	{
		opComp1.count();
		if (arr[j] < x)
		{
			i++;
			opAttr1.count(3);
			swap(&arr[i], &arr[j]);
		}
	}
	opAttr1.count(3);
	swap(&arr[i + 1], &arr[r]);
	return (i + 1);

}

void quickSort(int* arr, int l, int r, int n)
{
	if (l < r)
	{

		int q = partition(arr, l, r, n);
		quickSort(arr, l, q - 1,n);
		quickSort(arr, q + 1, r,n );
	}
}

void printarr(int* arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", arr[i]);
	}
}

void average_case_quick()
{
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (int i = 0; i < NR_TESTE; i++)
		{
			FillRandomArray(v, n, 10, 50000, false, 0);
			quickSort(v,0,n-1,n);
		}
	}
	p.divideValues("atr-part", NR_TESTE);
	p.divideValues("comp-part", NR_TESTE);
	p.showReport();
}

void worst_case_quick()
{
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		FillRandomArray(v, n, 10, 50000, false, ASCENDING);
		quickSort(v,0, n-1,n);
	}
	p.showReport();
}

void best_case_quick()
{
	int v[MAX_SIZE];
	int v2[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		FillRandomArray(v, n, 10, 50000, false,0);
		quickSort(v, 0, n/2, n);
	}
	p.showReport();
}


void average_case()
{
	int v[MAX_SIZE];
	int v2[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (int i = 0; i < NR_TESTE; i++)
		{
			FillRandomArray(v, n, 10, 50000, false, 0);
			for (int i = 0; i < n; i++)
			{
				v2[i] = v[i];
			}
			quickSort(v, 0, n - 1, n);
			heapsort(v2, n);
		}
	}
	p.divideValues("atr-part", NR_TESTE);
	p.divideValues("comp-part", NR_TESTE);
	p.divideValues("atr-heap", NR_TESTE);
	p.divideValues("comp-heap", NR_TESTE);
	p.addSeries("quicksort","atr-part", "comp-part");
	p.addSeries("heapsort", "atr-heap", "comp-heap");
	p.createGroup("atr_total", "atr-part", "atr-heap");
	p.createGroup("comp_total", "comp-part", "comp-heap");
	p.createGroup("operatii_total", "heapsort", "quicksort");
	p.showReport();
}


int main()
{
	//int arr[] = {1,2,3,4,5,6 };
	//int n = sizeof(arr) / sizeof(arr[0]);
	//quickSort(arr, 0, n-1,n);
	//printarr(arr, n);
	//average_case_quick();
	//worst_case_quick();
	best_case_quick();
	//average_case();
	return 0;
}

/*
Stim ca pentru quicksort bestcase trebuie sa avem vectorul generat aleator si trebuie sa punem limita functiei intre 0 si n/2 
pentru a ne genera un rezultat bun. Putem observa ca complexitatea algoritmului este O(n* log n).
Pentru worst case avem vectorul ordonat crescator doar ca pentru a putea rula algoritmul trebuie sa scadem dimensiunea vectorului.
Pentru average case avem din nou vectorul dat aleator si o complexitate de O(n* log n).

In final am facut si comparatia in cazul average intre heapsort si quicksort. Dupa cum se poate vedea si din graficele care au reiesit 
heapsort este mai bun in average case la numarul total de operatii.

*/