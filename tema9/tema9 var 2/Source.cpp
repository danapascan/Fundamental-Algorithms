#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("lab");

#define STEP_SIZE 100
#define MAX_SIZE 10000

typedef struct node
{
	struct node* parent;
	int rank;
	int key;
}node;

typedef struct muchie
{
	node* nod1;
	node* nod2;
	int pondere;
}muchie;

typedef struct graf
{
	node** noduri;
	muchie** muchii;
}graf;

void MAKE_SET(node* x, Operation * op)
{
	op->count(2);
	x->parent = x;
	x->rank = 0;
}

void LINK(node* x, node* y, Operation * op)
{
	op->count();
	if (x->rank > y->rank)
	{
		op->count();
		y->parent = x;
	}
	else
	{
		op->count();
		x->parent = y;
		op->count();
		if (x->rank == y->rank)
		{
			op->count();
			y->rank = y->rank + 1;
		}
	}
}

node* FIND_SET(node* x, Operation * op)
{
	op->count();
	if (x != x->parent)
	{
		op->count();
		x->parent = FIND_SET(x->parent, op);
	}
	return x->parent;
}

void UNION(node* x, node* y, Operation *op)
{
	LINK(FIND_SET(x, op), FIND_SET(y, op), op);
}

node* create_node(int key)
{
	node* n = (node*)malloc(sizeof(node));
	n->key = key;
	return n;
}

muchie* create_muchie(node* x, node* y, int pondere)
{
	muchie* m = (muchie*)malloc(sizeof(muchie));
	m->nod1 = x;
	m->nod2 = y;
	m->pondere = pondere;
	return m;
}

void demo()
{
	Operation* op;
	int arr[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	node** arr_node = (node**)malloc(17 * sizeof(node*));
	for (int i = 0; i < 16; i++)
	{
		arr_node[i] = create_node(arr[i]);
		MAKE_SET(arr_node[i], op);
	}
	for (int i = 0; i < 15; i += 2)
	{
		UNION(arr_node[i], arr_node[i + 1], op);
	}
	for (int i = 0; i < 16; i++)
	{
		printf("%d are ca parinte: %d", arr_node[i]->key, FIND_SET(arr_node[i], op)->key);
		printf("\n");
	}
}

graf* create_graf(int nr_muchii, int nr_noduri)
{
	graf* g = (graf*)malloc(sizeof(graf));
	g->muchii = (muchie**)malloc(nr_muchii * sizeof(muchie*));
	g->noduri = (node**)malloc(nr_noduri * sizeof(node*));
	return g;
}

void selectionsort(muchie** v, int n, Operation * op)
{
	int poz = 0;
	muchie* aux = (muchie*)malloc(sizeof(muchie));
	for (int i = 0; i < n; i++)
	{
		poz = i;
		for (int j = i + 1; j < n; j++)
		{
			if (v[j]->pondere < v[poz]->pondere)
			{
				poz = j;
			}
		}
		if (poz != i)
		{

			aux = v[i];
			v[i] = v[poz];
			v[poz] = aux;
		}

	}
}

muchie** MST_KRUSKAL(graf* g, int nr_noduri, int nr_muchii)
{
	Operation op = p.createOperation("operatii", nr_muchii);

	muchie** m = (muchie**)malloc(nr_muchii*sizeof(muchie*));

	for (int i = 0; i <nr_noduri; i++)
	{
		MAKE_SET(g->noduri[i], &op);
	}
	
	selectionsort(g->muchii, nr_muchii, &op);

	int k = 0;
	int i = 0;

	while (k<(nr_noduri-1 ))
	{
		muchie* next_muchie = g->muchii[i++];
		node* x =FIND_SET(next_muchie->nod1, &op);
		node* y =FIND_SET(next_muchie->nod2, &op);

		if (x != y)
		{
			m[k++] = next_muchie;
			UNION(x, y, &op);
		}
		
	}
	return m;
}


void demo_kruskal()
{
	//int n ;
	int nr_muchii = 13;
	int nr_noduri = 14;
	graf* g = create_graf(nr_muchii, nr_noduri);
	for (int i = 0; i < nr_noduri; i++)
	{
		g->noduri[i] = create_node(i);
	}

	g->muchii[0] = create_muchie(g->noduri[0], g->noduri[1], 5);
	g->muchii[1] = create_muchie(g->noduri[1], g->noduri[2], 4);
	g->muchii[2] = create_muchie(g->noduri[2], g->noduri[3], 7);
	g->muchii[3] = create_muchie(g->noduri[3], g->noduri[4], 3);
	g->muchii[4] = create_muchie(g->noduri[4], g->noduri[5], 9);
	g->muchii[5] = create_muchie(g->noduri[5], g->noduri[6], 2);
	g->muchii[6] = create_muchie(g->noduri[6], g->noduri[7], 1);
	g->muchii[7] = create_muchie(g->noduri[7], g->noduri[8], 10);
	g->muchii[8] = create_muchie(g->noduri[8], g->noduri[9], 4);
	g->muchii[9] = create_muchie(g->noduri[9], g->noduri[10], 12);
	g->muchii[10] = create_muchie(g->noduri[10], g->noduri[11], 10);
	g->muchii[11] = create_muchie(g->noduri[11], g->noduri[12], 5);
	g->muchii[12] = create_muchie(g->noduri[12], g->noduri[13], 7);


	muchie** rezultat=(muchie**)malloc(nr_muchii*sizeof(muchie*));
	rezultat = MST_KRUSKAL(g,nr_noduri, nr_muchii); 

	for(int i=0;i<nr_noduri-1;i++)
	{
		printf("Intre nodul %d si nodul %d exista o muchie cu ponderea %d",rezultat[i]->nod1->key, rezultat[i]->nod2->key, rezultat[i]->pondere);
		printf("\n");
	}

}

void demo_kruskal_operatii()
{
	for (int n = STEP_SIZE; n < MAX_SIZE; n+=STEP_SIZE)
	{
			int nr_noduri=n;
			int nr_muchii=4*n;
			int *arr=(int*)malloc(nr_muchii*sizeof(int));

			graf* g = create_graf(nr_muchii, nr_noduri);

			for (int i = 0; i < nr_noduri; i++)
			{
				g->noduri[i] = create_node(i);
			}

			FillRandomArray(arr, nr_muchii,0,100,false,0);

			int a = 0;
			int b = 0;
			for (int i = 0; i < nr_muchii; i++) {
				
				g->muchii[i] = create_muchie(g->noduri[a], g->noduri[b], arr[i]);
				
				if (a == n) {
					a = 0;
				}
				if (b == n) {
					b = 0;
				}
				if (a == b)
				{
					b++;
				}
				if (i % 3 == 0 || i % 3 == 1)
				{
					a++;
				}
				else b++;

				
			}

		muchie** rezultat = (muchie**)malloc(nr_muchii * sizeof(muchie*));
		rezultat = MST_KRUSKAL(g, nr_noduri, nr_muchii);
		/*for (int i = 0; i < nr_noduri - 1; i++)
		{
			printf("Intre nodul %d si nodul %d exista o muchie cu ponderea %d", rezultat[i]->nod1->key, rezultat[i]->nod2->key, rezultat[i]->pondere);
			printf("\n");
		}*/
	}
	p.showReport();

}

int main()
{
	//demo();
	//demo_kruskal();
	demo_kruskal_operatii();
	return 0;
}