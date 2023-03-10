#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"

Profiler p("lab");
#define NR_TESTE 5
#define MAX_SIZE 10000
#define STEP_SIZE 100

typedef struct node
{
	int val;
	struct node* left;
	struct node* right;
	struct node* parent;
	int size;

}node;

node* create_node()
{

	node* p = (node*)malloc(sizeof(node));
	p->val = 0;
	p->left = NULL;
	p->right = NULL;
	p->parent = NULL;
	p->size = 0;
	return p;
	
}


node* create_tree (int  n, int m)
{
	if (n <= m)
	{
		node* root = create_node();
		//aici dam ca radacina nodul din mijloc
		root->val = (m + n + 1) / 2;
		//vom apela in mod recursiv functia in preordine
		root->size = 1;
		root->left = create_tree(n,root->val - 1);
		root->right = create_tree(root->val+1,m);
		//aici facem size ul pentru fiecare nod 
		if (root->left!= NULL)
		{
			root->left->parent = root;
			root->size = root->size + root->left->size;
		}
		if (root->right != NULL)
		{
			root->right->parent = root;
			root->size = root->size + root->right->size;
		}
		return root;
	}
	else return NULL;
}

node * os_select(node* nod, int i,int n)
{
	Operation opComp = p.createOperation("comp_select", n);
	Operation opAttr = p.createOperation("atr_select", n);
	int rang;
	opComp.count();
	//daca am ajuns la o frunza ii dam rangul 1
	if (nod->left == NULL)
	{
		rang = 1;
	}
	else  rang = nod->left->size + 1; //altfel dam rangul nodului de pe parteaa stanga plus 1
	opComp.count();
	//daca avem rangul egal cu i atunci returnam nodul la care am ajuns
	if (rang == i)
	{
		return nod;
	}
	else
	{
		opComp.count();
		//daca i e mai mic ca rangul la care am ajuns atunci cautam pe partea stanga
		if (i < rang)
		{
			return os_select(nod->left, i, n);
		}
		//altfel cautam pe paretea dreapta 
		//nodul pe care il caut e al i lea -rang
		else
		{
			return os_select(nod->right, i - rang, n);
		}
	}

}

node* search(node* root)
{
	if (root->left == NULL)
		return root;
	return search(root->left);
}

node* os_delete(node* root, int key,int n)
{
	Operation opComp = p.createOperation("comp_delete", n);
	Operation opAttr = p.createOperation("atr_delete", n);

	// daca nu avem nici o radacinaa inseamna ca nu avem noduri deci vom returna NULL
	opComp.count();
	if (root == NULL)
	{
		return root;
	}
	//daca cheia pe care o cautam este mai mica decat cheia nodului radacina vom continua cautarea pe partea stanga
	opComp.count();
	if (key < root->val)
	{
		opAttr.count();
		root->size--;
		root->left = os_delete(root->left, key,n);
		
	}
	else
	{	//altfel cheia pe care o cautam se afla pe partea dreapta
		opComp.count();
		if (key > root->val)
		{  
			root->size--;
			opAttr.count();
			root->right = os_delete(root->right, key, n);
			
		}
		//altfel cheia cautata este egala cu cheia radacinii
		else
		{
			opComp.count(2);
			root->size--;
			//daca nodul cu cheia cautata este frunza atunci eliberam nodul
			if (root->left == NULL && root->right == NULL)
			{
				free(root);
				return NULL;

			}
			opComp.count();
			//daca nodul cu cheia cautata nu are copii pe partea dreapta atunci se elimina si pe locul lui se pune primul copil stang
			if (root->right == NULL)
			{
				node* p;
				opAttr.count();
				p = root->left;
				free(root);
				return p;

			}
			opComp.count();
			//daca nodul cu cheia cautata nu are copii pe partea stanga atunci se elimina si oe locul lui se pune primul copil drept
			if (root->left == NULL)
			{
				node* p;
				p = root->right;
				opAttr.count();
				free(root);
				return p;

			}

			//la final daca are pe ambele parti ciopii, se elimina si se pune cel mai mic copil de pe partea dreapta
			opAttr.count(3);
			node* succ = search(root->right);
			//asta e operatia din functia search
			opComp.count();
			root->val = succ->val;
			root->right = os_delete(root->right, succ->val, n);
		}
	}
	return root;
}


void print_inordine(node* radacina) //afiseaza crescator
{
	if (radacina != NULL)
	{
		print_inordine(radacina->left);
		printf("%d ", radacina->val);
		print_inordine(radacina->right);
	}
}

void demo_create()
{
	node* root = NULL;
	root = create_tree(1, 10);
	print_inordine(root);
}

//void demo_os_select()
//{
//	int k;
//	for (int n = STEP_SIZE; n <= MAX_SIZE; n+=STEP_SIZE)
//	{
//		for (int i = 0; i < NR_TESTE; i++)
//		{
//			node* root=NULL;
//			root = create_tree(1, n);
//			node* rez = os_select(root, n/2,k);
//			//printf("%d", rez->val);
//		}
//	}
//	p.divideValues("atr_select", NR_TESTE);
//	p.divideValues("comp_select", NR_TESTE);
//	p.showReport();
//
//}

void demo_os_delete()
{
	for (int n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int i=0;i<NR_TESTE;i++)
		{
			node* root = NULL;
			root = create_tree(1, n);
			node* rez = os_select(root, n / 2,n);
			root=os_delete(root, n/2,n);
			//print_inordine(root);
		}
	}
	p.divideValues("atr_delete", NR_TESTE);
	p.divideValues("comp_delete", NR_TESTE);
	p.divideValues("atr_select", NR_TESTE);
	p.divideValues("comp_select", NR_TESTE);
	p.addSeries("delete", "atr_delete", "comp_delete");
	p.addSeries("os_select", "atr_select", "comp_select");
	p.showReport();
}

int main()
{
	demo_os_delete();
	return 0;
}