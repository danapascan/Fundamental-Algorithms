#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"

Profiler p("heap");
#define MAX_SIZE 100
#define STEP_SIZE 10
#define NR_TESTE 5

void swap(int *x, int *y)
{
	int aux;
	aux = *x;
	*x = *y;
	*y = aux;
}

void heapify(int* arr, int n, int i)
{
	Operation opComp1 = p.createOperation("comp-bu", n);
	Operation opAttr1 = p.createOperation("atr-bu", n);
	
	    int left = 2 * i+1;
		int right = 2 * i + +2;
		int root = i;
		opComp1.count(2);
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
			opAttr1.count(3);
			heapify(arr, n, root);

		}
}

//bottom-up 

void bottom_up(int* arr, int n)
{
	for (int i = (n / 2) - 1; i >= 0; i--)
	{
		heapify(arr, n, i);
	}
}

//top_down

void increase_key(int* arr, int i, int key, int n)
{
	Operation opComp1 = p.createOperation("comp-td", n);
	Operation opAttr1 = p.createOperation("atr-td", n);
	opComp1.count();
	if (key < arr[i])
	{
		return;
	}
	opAttr1.count();
	arr[i] = key;
	opComp1.count();
	while (i > 0 && arr[(i - 1) / 2] < arr[i])
	{
		opComp1.count();
		opAttr1.count(3);
		swap(&arr[i], &arr[(i - 1) / 2]);
		i = (i - 1) / 2;
	}

}

void heap_insert(int* arr2, int *arr,int key, int &heapsize, int n)
{
	heapsize = heapsize + 1;
	arr2[heapsize - 1] = -99999999;
	increase_key(arr2, heapsize - 1, key,n);

}

void build_heap(int* arr,int *arr2, int n)
{
	int heapsize = 0;
	for (int i = 0; i <n; i++)
	{
		heap_insert(arr2,arr,arr[i], heapsize,n);
	}
}

void heapsort(int* arr, int n)
{
	bottom_up(arr, n);

	for (int i = n - 1; i >= 0; i--)
	{
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}

}

void printarr(int* arr, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", arr[i]);
}



void worst_case()
{
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		FillRandomArray(v, n, 10, 50000, false, ASCENDING);
		//am facut al doulea vector pentru topdown
		int v2[MAX_SIZE];
		for (int i = 0; i < n; i++)
		{
			v2[i] = v[i];
		}
		bottom_up(v, n);
		build_heap(v2, aux, n);
	}
	p.createGroup("comparations_worst", "comp-td", "comp-bu");
	p.createGroup("attributions_worst", "atr-td", "atr-bu");
	p.showReport();
}

void average_case()
{
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (int i = 0; i < NR_TESTE; i++)
		{
			FillRandomArray(v, n, 10, 50000, false, 0);
			//am facut al doulea vector pentru topdown
			int v2[MAX_SIZE];
			for (int i = 0; i < n; i++)
			{
				v2[i] = v[i];
			}
			bottom_up(v, n);
			build_heap(v2, aux, n);
		}
	}
	p.divideValues("comp-bu", NR_TESTE);
	p.divideValues("atr-bu", NR_TESTE);
	p.divideValues("comp-td", NR_TESTE);
	p.divideValues("atr-td", NR_TESTE);
	p.createGroup("comparations_average", "comp-td", "comp-bu");
	p.createGroup("attributions_average", "atr-td", "atr-bu");
	p.showReport();
}

int main()
{
	//int arr[] = {1,2,3,4,7,8,9,10,14,16};
	//int n = sizeof(arr) / sizeof(arr[0]);
	//int* arr2 = (int*)malloc(n * sizeof(int));
	//bottom_up(arr, n);
	//build_heap(arr,arr2,n);
	//heapsort(arr, n);
	//worst_case();
	average_case();
	//printarr(arr2, n);
	return 0;
}
/*
* In codul de mai sus am verificat complexitatea pentru doi algoritmi de construire a heapului.
Primul algoritm construieste heapul ButtomUp este de complexitate O(n) si foloseste functia heapify.
Al doilea algoritm construieste heapul TopDown, avand complexitate O(n* log n) si foloseste mai multe functii 
pentru a insera pe rand cate un element in heap si a mentine la fiecare pas max heap.
Din tabelele de complexitate rezultate putem vedea ca atat in cazun nefavorabil cat si in  cazul average,
in care am facut 5 teste, modul de construire BottomUp este mult mai eficient.
Am implementat si functie pentru sortarea heapului de complexitate O(n* log n).
*/