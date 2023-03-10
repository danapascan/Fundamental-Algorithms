#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"

Profiler p("lab");

#define NR_TESTE 5

typedef struct {
	int id;
	char name[30];
}entry;

int quadratic_probing(int k, int i, int n)
{
	return (k % n + i + (i * i)) % n;
}

entry* make_entry(int n)
{
	entry* table = (entry*)malloc(n * sizeof(entry));
	for (int i = 0; i < n; i++)
	{
		table[i].id = -1;
		strcpy(table[i].name, "NULL");
	}
	return table;
}

void insert_table(entry* t, int n, entry x)
{
	int i;
	i = 0;
	while (i != n)
	{
		int j = quadratic_probing(x.id, i, n);
		if (t[j].id == -1)
		{
			t[j].id = x.id;
			strcpy(t[j].name, x.name);
			break;
		}
		else i++;
	}
}

int search_table(entry* t, int n, entry x)
{
	int i = 0;
	int  j = quadratic_probing(x.id, i, n);
	while (t[j].id != -1 && i < n)
	{

		if (strcmp(t[j].name, x.name) == 0)
		{
			return j;
		}
		i++;
		j = quadratic_probing(x.id, i, n);
	}
	return -1;
}

void print_table(entry * t, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d %s", t[i].id, t[i].name);
		printf("\n");
	}
}

entry aloc(int id, char* name)
{
	entry x;
	x.id = id;
	strcpy(x.name, name);
	return x;
}
void demo_search()
{
	entry* aux;
	entry* table;
	int n = 13;
	char nume[13][7] = { "Joze","Colo","Puya","Gogo","Shely","Ian","Lari","Olga","Buni","Gelu","Cora","Abel","Cain"};

	int* arr = (int*)malloc(n * sizeof(int));

	// Am alocat un vector aleator pentru a adauga id urile in tabela

	FillRandomArray(arr, n, 10, 50000, true, 0);
	aux = make_entry(n);

	// Am facut o tabela aux in care am adaugat id urile din arr si numele din arr de nume

	for (int i = 0; i < n; i++)
	{
		aux[i].id = arr[i];
		strcpy(aux[i].name, nume[i]);
	}

	table = make_entry(n);

	//Am adaugat in tabela principala valorile din tabela aux

	for (int i = 0; i < n; i++)
	{
		insert_table(table, n, aux[i]);
	}
	print_table(table, n);
	printf("\n");

	//Aici am alocat unei variabile id ul si numele pe care vrem sa le cautam in tabela
	entry x;
	char nume_cauta[7] = "Puya";
	x = aloc(0, nume_cauta);

	for (int i = 0; i < n; i++)
	{
		if (strcmp(table[i].name, x.name) == 0)
		{
			x.id = table[i].id;
		}
	}

	//printf("%d %s", x.id, x.name);

	//Cautam in tabela variabila

	int rez = search_table(table, n, x);

	printf("%d", rez);
}

void delete_table(entry * table, entry x, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (table[i].id == x.id)
		{
			table[i].id = -9999;
			strcpy(table[i].name, "NULL");
		}
	}
}

void demo_95()
{
	int marime_tabela=13;
	double alfa = 0.95;
	int n = marime_tabela * alfa;
	printf("%d", n);
		 
	int* arr = (int*)malloc(n * sizeof(int));
	FillRandomArray(arr, n, 10, 50000, true, 0);

	entry* aux = (entry*)malloc(n * sizeof(entry));

	aux = make_entry(n);

	for (int i = 0; i < n; i++)
	{
		aux[i].id = arr[i];
		strcpy(aux[i].name, "NULL");
	}

	entry* table;
	table = make_entry(marime_tabela);

	for (int i = 0; i < n; i++)
	{
		insert_table(table, n, aux[i]);
	}

	print_table(table, marime_tabela);
	
}

int main()
{
	demo_search();
	//demo_95();

	entry* table;
	int n = 3;
	table = make_entry(n);
	char nume[3][7] = { "Iulia","Cora","Lala" };
	table[0].id = 5;
	strcpy(table[0].name, nume[0]);
	table[1].id = 8;
	strcpy(table[1].name, nume[1]);
	table[2].id = 9;
	strcpy(table[2].name, nume[2]);

	print_table(table, n);

	delete_table(table, table[2], n);

	print_table(table, n);

	return 0;
}
