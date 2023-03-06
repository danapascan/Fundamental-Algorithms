#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include "Profiler.h"

Profiler p("bfs");

using namespace std;

int time1=0;
enum{WHITE, GRAY, BLACK};

typedef struct nod
{
	int val;
	struct nod* next;
}nod;

typedef struct lista
{
	nod* first;
	nod* last;
}lista;

typedef struct graf
{
	int n;
	lista* l;
	int* d;
	int* f;
	int* parent;
	int* color;

}graf;


void DFS_visit(graf* g, int start,  int * vec, Operation* op)
{
	time1++;
	op->count(2);
    g->d[start] = time1;
	g->color[start] = GRAY;
	printf("%d ", start);

	nod* p = (nod*)malloc(sizeof(nod));

	for (p = g->l[start].first; p != NULL; p = p->next)
	{
		if (g->color[p->val] == GRAY)
		{
			vec[start] = 1;
		}
		op->count();
		if (g->color[p->val] == WHITE)
		{
			op->count();
			g->parent[p->val] = start;
			DFS_visit(g, p->val, vec, op);
		}
	}
	op->count(2);
	g->color[start] = BLACK;
	time1++;
	g->f[start] = time1;

}

void DFS(graf * g, int * vec, int n)
{
	Operation op = p.createOperation("bfs-edges", n);

	for (int i = 0; i < g->n; i++)
	{
		op.count(2);
		g->color[i] = WHITE;
		g->parent[i] = NULL;
	}

   int time = 0;
	 
	for (int i = 0; i < g->n; i++)
	{
		op.count();
		if (g->color[i] == WHITE)
		{
			DFS_visit(g, i, vec, &op);
		}
	}
	
}

void insert_first(lista* l, int val)
{
	nod* p = (nod*)malloc(sizeof(nod));
	p->val = val;
	p->next = NULL;

	if (l->first == NULL)
	{
		l->first = p;
		l->last = p;

	}
	else
	{
		p->next = l->first;
		l->first = p;
	}
}

void add_muchie(graf* g, int u, int v)
{
	insert_first(&(g->l[u]), v);
}

graf* create_graf(int nr)
{
	graf* g = (graf*)malloc(sizeof(graf));
	g->n = nr;
	int* d = (int*)calloc(nr, sizeof(int));
	int* f = (int*)calloc(nr, sizeof(int));
	int* parent = (int*)calloc(nr, sizeof(int));
	int* color=(int*)calloc(nr, sizeof(int));
	lista* l = (lista*)malloc(nr * sizeof(lista));
	int i;
	for (i = 0; i < nr; i++)
	{
		l[i].first = NULL;
		l[i].last = NULL;
	}
	g->l = l;
	g->d = d;
	g->f = f;
	g->parent = parent;
	g->color =color;
	return g;
}

void show_lista(lista* l)
{
	nod* curent;
	curent = l->first;
	while (curent != NULL)
	{
		printf("%d ", curent->val);
		curent = curent->next;
	}
}
void show_graf(graf* g)
{
	for (int i = 0; i < g->n; i++)
	{
		printf("%d.", i);
		show_lista(&(g->l[i]));
		printf("\n");
	}
}

void selectionsort(int * v, int n, int * vector)
{
	for (int i = 0; i < n; i++)
	{
		vector[i] = i;
	}
	int poz;
	int aux;
	for (int i = 0; i < n - 1; i++)
	{
		int poz = i;
		for (int j = i + 1; j < n; j++)
		{
			if (v[j] < v[poz])
			{
				poz = j;
			}
		}
		if (poz != i)
		{
			aux = v[i];
			v[i] = v[poz];
			v[poz] = aux;

			aux = vector[i];
			vector[i] = vector[poz];
			vector[poz] = aux;
		}

	}
}

void sortare_topologica(graf* g)
{
	int n;
	int* vec = (int*)calloc(g->n, sizeof(int));
	int* aux = (int*)calloc(g->n, sizeof(int));
	int* sorted=(int*)malloc(g->n*sizeof(int));
	DFS(g ,vec,0);
	int ok = 1;
	for (int i = 0; i < g->n; i++)
	{
		if (vec[i] == 1)
		{
			ok = 0;
		}
		printf("%d ", vec[i]);
	}
	printf("\n");
	printf("%d ", ok);
	printf("\n");
	if (ok != 0)
	{
		for (int i = 0; i < g->n; i++)
		{
			printf("nodul %d: d:%d, f:%d\n ",i,g->d[i], g->f[i]);
			aux[i] = g->f[i];
		}
		selectionsort(aux, g->n,sorted);
		printf("sortare:\n");
		for (int i = g->n-1; i >=0; i--)
		{
			printf("%d ",sorted[i]);
		}
	}

}

int index2 = 0;
int nrcomponents = 0;
stack< int> S;

void strong_connect(graf* g, int u, int* index, int* lowlink, bool* onstack, int* comp)
{
	index[u] = index2;
	lowlink[u] = index2;
	index2++;
	S.push(u);
	onstack[u] = true;
	nod* p;
	for (p = g->l[u].first; p != NULL; p = p->next)
	{
		if (index[p->val] == -1)
		{
			strong_connect(g, p->val, index, lowlink, onstack, comp);
			lowlink[u] = min(lowlink[u], lowlink[p->val]);
		}
		else
			if (onstack[p->val])
			{
				lowlink[u] = min(lowlink[u], index[p->val]);
			}
	}
	
	if (lowlink[u] == index[u])
	{
			nrcomponents++;
			printf("scc: ");
			int v;
			while (S.top()!=u)
			{
				v=S.top();
				printf("%d ", v);
				S.pop();
				onstack[v] = false;
				comp[v] = nrcomponents;
				
			}
			v = S.top();
			printf("%d ", v);
			onstack[v] = false;
			S.pop();
	}

}
void tarjan(graf* g, int * index, int* lowlink, bool* onstack, int* comp)
{
	for (int i = 0; i < g->n; i++)
	{
		index[i] = -1;
		lowlink[i] = -1;
		onstack[i] = false;
		comp[i] = 0;
	}
	for (int i = 0; i < g->n; i++)
	{
		if (index[i] == -1)
		{
			strong_connect(g, i, index, lowlink, onstack, comp);
		}
	}

}

void demo_tarjan()
{
	graf* g;
	g=create_graf(11);
	add_muchie(g, 0, 3);
	add_muchie(g, 1, 0);
	add_muchie(g, 1, 3);
	add_muchie(g, 1, 9);
	add_muchie(g, 1, 10);
	add_muchie(g, 0, 2);
	add_muchie(g, 2, 1);
	add_muchie(g, 2, 4);
	add_muchie(g, 3, 6);
	add_muchie(g, 4, 1);
	add_muchie(g, 4, 5);
	add_muchie(g, 4, 7);
	add_muchie(g, 6, 5);
	add_muchie(g, 7, 9);
	add_muchie(g, 8, 7);
	add_muchie(g, 9, 3);
	add_muchie(g, 9, 8);
	add_muchie(g, 10, 8);


	show_graf(g);

	int* index = (int*)malloc(g->n* sizeof(int));
	int* lowlink = (int*)malloc(g->n * sizeof(int));
	bool* onstack = (bool*)malloc(g->n * sizeof(bool));
	int* com = (int*)malloc(g->n * sizeof(int));
	tarjan(g, index, lowlink, onstack, com);
}

void demo_dfs()
{
	graf* g;
	g = create_graf(7);
	//int n;
	int* vec=(int *)calloc(g->n,sizeof(int));
	add_muchie(g, 1, 2);
	add_muchie(g, 2, 4);
	add_muchie(g, 2, 5);
	add_muchie(g, 1, 3);
	add_muchie(g, 3, 4);
	add_muchie(g, 3, 6);
	add_muchie(g, 4, 5);
	add_muchie(g, 4, 6);
	show_graf(g);
	printf("\n");
	DFS(g,vec, 0);

}

void demo_sortare_topologica()
{
	graf* g;
	g = create_graf(7);
	int* vec = (int*)calloc(g->n, sizeof(int));
	add_muchie(g, 1, 2);
	add_muchie(g, 2, 4);
	add_muchie(g, 2, 5);
	add_muchie(g, 1, 3);
	add_muchie(g, 3, 4);
	add_muchie(g, 3, 6);
	add_muchie(g, 4, 5);
	add_muchie(g, 4, 6);
	show_graf(g);

	sortare_topologica(g);
}

void demo1()
{
	int V = 100;
		graf* g;
		g = create_graf(V);
		int* vec=(int *)malloc(g->n* sizeof(int));
	for (int E = 1000; E <= 4500; E += 100)
	{
	
		for (int i = 0; i < V-1; i++)
		{
			for (int j = i; i < V; i++)
			{
				add_muchie(g, i, j);
			}
		}
		DFS(g, vec, E);
	}
	p.showReport();
}

void demo2()
{
	int E = 4500;
	for (int V = 100; V <= 200; V += 10)
	{
		graf* g;
		g = create_graf(V);
		int* vec = (int*)malloc(g->n * sizeof(int));

		for (int i = 0; i < V - 1; i++)
		{
			for (int j = i; i < V; i++)
			{
				add_muchie(g, i, j);
			}
		}
		DFS(g, vec, V);
	}
	p.showReport();
}

int main()
{
	//demo_dfs();
	//demo_sortare_topologica();
	demo_tarjan();
	//demo1();
	//demo2();
	return 0;
}