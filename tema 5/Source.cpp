#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Profiler.h"

Profiler p("lab");

typedef struct node
{
	int val;
	struct node* next;

}node;
typedef struct lista
{
	node* first;
	node* last;

}lista;

void insert_last(lista* l, int key)
{
	node* p = (node*)malloc(sizeof(node));
	p->val = key;
	p->next = NULL;

	if (l->first == NULL)
	{
		l->first = p;
		l->last = p;
	}
	else
	{
		l->last->next = p;
		l->last = p;
	}

}

//aici generam lunigimile listelor
void generate_random_numbers(int* v, int k, int n)
{
	//pornim de la k pentru ca deja am pus k elemente in liste 
	//pentru ca listele trebuie sa aiba cel putin un element
	for (int i = 0; i < k; i++)
	{
		v[i] = 1;
	}
	srand(time(0));

	for (int i = k; i < n; i++)
	{
		v[rand() % k]++;
	}
}

void print_list(lista* l)
{
	node* aux = l->first;
	while (aux != NULL)
	{
		printf("%d ", aux->val);
		aux = aux->next;
	}
	printf("\n");
}

lista** random_lists(int *v,int k, int n)
{
	//alocam vector de liste
	lista** l = (lista**)malloc(k * sizeof(lista*));
	for (int i = 0; i < k; i++)
	{
		l[i] = (lista*)malloc(sizeof(lista));
		l[i]->first = NULL;
		l[i]->last = NULL;
	}

	//initializam un vector
	int* arr = (int*)malloc(n * sizeof(int));

	//for pt numarul de liste
	for (int i = 0; i < k; i++)
	{
		//generam un vector ordonat de dim v[i]
		FillRandomArray(arr, v[i], 0, 100, false, ASCENDING);

		//mutam elementele din vector in lista
		for (int j = 0; j < v[i]; j++)
		{
			insert_last(l[i], arr[j]);
		}
	    //print_list(l[i]);
	}
	return l;
}

void show_lists(lista** l, int k)
{
	for (int i = 0; i < k; i++)
	{
		print_list(l[i]);
	}
}

void swap(lista ** x, lista ** y)
{
	lista*  aux;
	aux = *x;
	*x = *y;
	*y = aux;
}

void heapify(lista** l, int n, int i, Operation* opComp, Operation* opAttr)
{
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int root = i;
	opComp->count(2);
	if (left < n && l[left]->first->val < l[root]->first->val)
	{
		root = left;
	}
	if (right < n && l[right]->first->val < l[root]->first->val)
	{
		root = right;
	}
	if (root != i)
	{
		swap(&l[root], &l[i]);
		opAttr->count(3);
		heapify(l, n, root, opComp, opAttr);

	}
}

//bottom-up 

void bottom_up(lista ** l , int n, Operation* opComp, Operation* opAttr)
{
	for (int i = (n / 2) - 1; i >= 0; i--)
	{
		heapify(l, n, i, opComp, opAttr);
	}
	
}

lista* interclasare(lista** l, int k)
{
	Operation opComp = p.createOperation("comp", k);
	Operation opAttr = p.createOperation("atr", k);
	//heap-ul are initial lungimea k
	int heap_size = k;
	//alocam lista in care vom avea cele k siruri interclasate
	lista* Lout=(lista*)malloc(sizeof(lista));
	Lout->first = NULL;
	Lout ->last = NULL;

	//creem heapul tinand cont de primele elemente din fiecare lista
	bottom_up(l, k, &opComp, &opAttr);

	//cat timp mai exista liste cu elemente
	while (heap_size > 0)
	{
		//punem primul element din heap in lista rezultat
		insert_last(Lout, l[0]->first->val);
		opAttr.count(3);

		//trecem la urmatorul element din lista in care am gasit cel mai mic element
		l[0]->first = l[0]->first->next;
		opComp.count();

		//daca lista a ramas goala scadem dimensiunea
		//heapului si mutam ultima lista din heap pe prima pozitie

		if (l[0]->first == NULL)
		{
			opAttr.count();
			l[0]= l[heap_size-1];
			heap_size = heap_size - 1;
		}

		//rearanjam heap-ul sa fie min heap
		heapify(l,heap_size,0, &opComp, &opAttr);
	}
	return Lout;
	print_list(Lout);
	
}

//void caz_mediu1()
//{
//	int k = 5;
//	for (int n = 100; n <= 1000; n += 100)
//	{
//		Operation opComp = p.createOperation("comp1", n);
//		Operation opAttr = p.createOperation("atr1", n);
//
//
//		int* v1 = (int*)malloc(k * sizeof(int));
//		generate_random_numbers(v1, k, n);
//		lista** l1 = random_lists(v1, k, n);
//		lista* rez1 = interclasare(l1, k);
//
//	}
//	int k = 10;
//	for (int n = 100; n <= 1000; n += 100)
//	{
//		Operation opComp = p.createOperation("comp2", n);
//		Operation opAttr = p.createOperation("atr2", n);
//
//		int* v2 = (int*)malloc(k * sizeof(int));
//		generate_random_numbers(v2, k, n);
//		lista** l2 = random_lists(v2, k, n);
//		lista* rez2 = interclasare(l2, k);
//	}
//	int k = 100;
//	for (int n = 100; n <= 1000; n += 100)
//	{
//		Operation opComp = p.createOperation("comp3", n);
//		Operation opAttr = p.createOperation("atr3", n);
//		
//		int* v3 = (int*)malloc(k * sizeof(int));
//		generate_random_numbers(v3, k, n);
//		lista** l3 = random_lists(v3, k, n);
//		lista* rez3 = interclasare(l3, k);
//
//	}
//	p.addSeries("k1", "atr1", "comp1");
//	p.addSeries("k2", "atr2", "comp2");
//	p.addSeries("k3", "atr3", "comp3");
//
//	p.showReport();
//
//}

void caz_mediu2()
{
	int n = 10000;
	
	for (int k = 10; k <= 500; k += 10)
	{
		int* v = (int*)malloc(k * sizeof(int));
		generate_random_numbers(v, k, n);
		lista** l = random_lists(v, k, n);
		lista* rez = interclasare(l, k);
	}

	p.addSeries("mediu", "atr", "comp");
	p.showReport();

}
int main()
{
	/*lista** l;
	int NR_LISTE=4, NR_ELEMENTE=20;
	int* v = (int*)calloc(NR_LISTE, sizeof(int));
	generate_random_numbers(v, NR_LISTE, NR_ELEMENTE);
	printf("\n");
	l=random_lists(v,NR_LISTE, NR_ELEMENTE);
	show_lists(l,NR_LISTE);
	lista* Lout;
	int n=20;
	Lout = interclasare(l,NR_LISTE);
	print_list(Lout);*/

	caz_mediu2();
	return 0;
}

/*Am facut in primul rand o functie pentru a genera un numar de k liste avand un numar de n elemente.Cu acea functie am generat listele
care sunt ordonate crescator. 
Am folosit operatiile pe heap pentru a pune fiecare prim element din fiecare lista in heap pentru a fi sortate cu ajutorul heapsort.
Dupa ce am sortat primele elemente, fiindca am folosit min heap stim ca in varf se afla elementul cel mai mic. 
Am extras acest element si am trecut la elementul urmator in acea lista, urmand ca elementul extras sa fie adaugat in lista finala de interclasare.
Dupa ce am format lista cu toate listele interclasate am afisat-o.
Algoritmul de interclasare are o complexitate de O(n*log k), pe care am testat o atat in cazul in care avem un numar fix de liste si numarul
de elemente se schimba cat si in cazul in care numarul de elemente e fix iar numarul de liste se schimba.
*/
