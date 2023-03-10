#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("lab");

#define STEP_SIZE 100
#define MAX_SIZE 10000
#define NR_TESTE 5
#define NR_TESTE2 100

typedef struct node
{
	int val;
	struct node* left;
	struct node* right;

}node;

typedef struct stack
{
	struct node* n;
	struct stack* next;
}stack;

node* create_node()
{

	node* p = (node*)malloc(sizeof(node));
	p->val = 0;
	p->left = NULL;
	p->right = NULL;
	return p;

}

node* create_tree(int  n, int m)
{
	if (n <= m)
	{
		node* root = create_node();
		root->val = (m + n + 1) / 2;
		root->left = create_tree(n, root->val - 1);
		root->right = create_tree(root->val + 1, m);
		return root;
	}
	else return NULL;
}

void preordine(node* radacina, int n)
{
	Operation op = p.createOperation("operatie_pre", n);
	if (radacina != NULL)
	{	
		printf("%d ", radacina->val);
		op.count();
		preordine(radacina->left,n);
		preordine(radacina->right,n);
	}
}

void postordine(node* radacina, int n)
{
	Operation op = p.createOperation("operatie_post", n);
	if (radacina != NULL)
	{
		postordine(radacina->left,n);
		postordine(radacina->right,n);
		printf("%d ", radacina->val);
		op.count();
	}
}

void inordine(node* radacina, int n)
{
	Operation op = p.createOperation("operatie_in", n);
	if (radacina != NULL)
	{
		inordine(radacina->left,n);
		printf("%d ", radacina->val);
		op.count();
		inordine(radacina->right,n);
	}
}

//void pretty_p(node* root, int level)
//{
//	if (root == NULL)
//		return;
//	pretty_p(root->right, level + 1);
//	for (int i = 0; i < level; ++i)
//		printf("\t");
//	printf("%d\n", root->val);
//	pretty_p(root->left, level + 1);
//
//}

//functie pentru a verifica daca stack ul mai are noduri
int empty_stack(stack* s)
{
	if ((s) == NULL)
		return 1;
	else return 0;
}


node * pop_stack(stack** s)
{
	stack* aux;
	node* rez;
	if (!empty_stack(*s))
	{
		aux = (*s); // in aux vom lua pointer la stack
		rez = aux->n; //in rez vom lua primul nod din stack
		(*s) = aux->next; // capul stack ului va pointa acum la urmatorul element
		return rez;// returnam primul element
	}
}


void push_stack(stack ** s,node* nod)
{
	if (nod != NULL)
	{
		//aloc nod nou
		stack* elem = (stack*)malloc(sizeof(stack));
		//punem datele in stack 
		elem->n = nod;
		//facem ca primul element nou sa pointeze la fostul prim element
		elem->next = (*s);
		//elem devine acum capul stack ului
		(*s) = elem;
	}

}

void postorder_iterative(node* root, int n)
{
	Operation op = p.createOperation("operatie_post_it", n);
	node* current_node = root;
	node* previous_node = NULL;
	stack* s = NULL;
	while (!empty_stack(s) || current_node != NULL)
	{
		if (current_node != NULL)
		{
			push_stack(&s, current_node);
			current_node = current_node->left;
		}
		else
		{
			node* previous_node = s->n->right;

			if (previous_node == NULL)
			{
				previous_node = s->n;
				node* aux = pop_stack(&s);
				op.count();
				printf("%d ", previous_node->val);
				while (!empty_stack(s) && previous_node == s->n->right)
				{
					previous_node = s->n;
					node* aux = pop_stack(&s);
					op.count();
					printf("%d ", aux->val);

				}
			}
			else {
				current_node = previous_node;
			}
		}
	}
}


void inorder_iterative(node* root, int n)
{
	Operation op = p.createOperation("operatie_in_it", n);
	//alocam un nod cu care parcurgem stack ul
	node* current_node = root;
	stack* s = NULL;
	int finish = 0;
	//cat timp nu am terminat cu nodurile
	while (!finish)
	{
		//daca nodul curent e diferit de null ii vom da push pe stack si vom trece la urmatorul nod de pe partea stanga
		if (current_node != NULL)
		{
			push_stack(&s, current_node);
			current_node = current_node->left;
		}
		//daca nodul curent e NULL inseamna ca am ajuns pana la final pe partea stanga deci vom da pop la ultimul nod
		//il vom afisa si dupa vom continua pe partea dreapta daca stiva nu e goala
		else
			if (!empty_stack(s))
			{
				current_node = pop_stack(&s);
				op.count();
				printf("%d ", current_node->val);
				current_node = current_node->right;
			}
			else
				//stiva e goala deci am terminat
				finish = 1;
	}

}


void preorder_iterative(node* root, int n)
{
	Operation op = p.createOperation("operatie_pre_it", n);
	stack* s = NULL;
	node* current_node = root;
	push_stack(&s, root);
	while (!empty_stack(s))
	{
		current_node = pop_stack(&s);
		op.count();
		printf("%d ", current_node->val);	
		

		if (current_node->right != NULL)
			push_stack(&s, current_node->right);
		if (current_node->left != NULL)
			push_stack(&s, current_node->left);
	}

}

void insertionsort(int* v, int n, int u, Operation * op)
{

	int num;
	for (int i = u+1; i <n+1; i++)
	{
		op->count();
		num = v[i];
		int j = i - 1;

		while (j >= u && v[j] > num)
		{
			op->count();
			op->count();
			v[j + 1] = v[j];
			j = j - 1;
		}
		op->count();
		op->count();
		v[j + 1] = num;
	}
}

void swap(int* a, int* b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

int partition(int* arr, int p, int r, Operation * op)
{
	op->count();
	int x = arr[r];
	int i = p - 1;
	for (int j = p; j <= r - 1; j++)
	{
		op->count();
		if (arr[j] < x)
		{
			i++;
			op->count(3);
			swap(&arr[i], &arr[j]);
		}
	}
	op->count(3);
	swap(&arr[i + 1], &arr[r]);
	return (i + 1);

}

void quickSort(int* arr, int k, int r, int n)
{
	Operation op = p.createOperation("op_quick", n);
	if (k < r)
	{
		int q = partition(arr, k, r, &op);
		quickSort(arr, k, q - 1,n);
		quickSort(arr, q + 1, r,n);
	}
}

void quicksort_hibrid(int *arr, int n,int m, int tot)
{
	Operation op2 = p.createOperation("operatii", tot);
	while (n < m)
	{
		if (m-n + 1 <30)
		{
			insertionsort(arr,m,n,&op2);
			break;
		}
		else
		{
			int pivot;
			pivot = partition(arr, n, m,&op2);
			if (pivot - n < m - pivot)
			{
				quicksort_hibrid(arr, n, pivot - 1,tot);
				n = pivot + 1;
			}
			else
			{
				quicksort_hibrid(arr, pivot + 1, m,tot);
				m = pivot - 1;
			}
		}
	}
}

void demo_quicksort_op()
{
	
	//int* v2;
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{

		for (int i = 0; i < NR_TESTE; ++i)
		{
			int* v = (int*)malloc(n*sizeof(int));
			int* v2 = (int*)malloc(n * sizeof(int));
			FillRandomArray(v, n, 10, 50000, false, 0);
			
			for (int i = 0; i < n; i++)
			{
				v2[i] = v[i];
			}
			quicksort_hibrid(v, 0, n-1, n);
			quickSort(v2, 0, n - 1, n);
		}
	}
	p.createGroup("comp", "operatii", "op_quick");
	p.showReport();
}

void demo_quicksort_hibrid_time()
{
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		p.startTimer("quicksort_hibrid", n);
		for (int i = 0; i < NR_TESTE2; ++i)
		{
			int* v = (int*)malloc(n * sizeof(int));
			FillRandomArray(v, n, 10, 50000, false, 0);
			quicksort_hibrid(v, 0, n - 1, n);
		}
		p.stopTimer("quicksort_hibrid", n);
	}

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		p.startTimer("quickSort", n);
		for (int i = 0; i < NR_TESTE2; ++i)
		{
			int* v2 = (int*)malloc(n * sizeof(int));
			FillRandomArray(v2, n, 10, 50000, false, 0);
			quickSort(v2, 0, n - 1, n);
		}
		p.stopTimer("quickSort", n);
	}
	p.createGroup("timp_dana", "quicksort_hibrid", "quickSort");
	p.showReport();
}

void demo()
{
	for (int n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
			node* root = NULL;
			root = create_tree(1, n);
			preordine(root, n);
			inordine(root, n);
			postordine(root, n);
			preorder_iterative(root, n);
			postorder_iterative(root, n);
			inorder_iterative(root, n);

	}
	p.showReport();

}

int main()
{
	//node* root;
	//root = create_tree(1, 8);
	//preordine(root);
	//printf("\n");
	//inordine(root);
	//printf("\n");
	//postordine(root,n);
	//printf("\n");
	//inorder_iterative(root);
	//printf("\n");
	//preorder_iterative(root);
	//postorder_iterative(root);
	//pretty_p(root, 0);
	//demo();
	demo_quicksort_op();
	//demo_quicksort_hibrid_time();
	return 0;
}