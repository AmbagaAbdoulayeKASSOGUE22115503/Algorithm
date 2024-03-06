/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement *previous;
	struct s_LinkedElement *next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement * whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement *sentinel;
	int size;
};

/*répresentation de la structure subList*/
typedef struct _SubList{
	LinkedElement *head;
	LinkedElement *tail;
}*SubList;


/*-------crée une liste vide avec sentinelle----------------------------------------------------------*/

List *list_create() {
	List *l = malloc(sizeof(struct s_List));
	l->sentinel= malloc(sizeof(LinkedElement));
	l->size=0; l->sentinel->next=l->sentinel->previous=l->sentinel;
	return l;
}

/*------------ajoute un élément à la fin de la liste l-----------------------------------------------------*/

List *list_push_back(List *l, int v) {
	LinkedElement *val= malloc(sizeof(LinkedElement));
	val->value= v; 
	val->next= l->sentinel;
	val->previous= l->sentinel->previous;
	l->sentinel->previous->next= val;
	l->sentinel->previous= val;
	++(l->size);
	return l;
}

/*-----------efface et libère tous les éléments l ainsi que l à la fin------------------------------------------------------*/

void list_delete(ptrList *l) {
	for(LinkedElement *e=(*l)->sentinel->next; e!=(*l)->sentinel; e=e->next)
		free(e);
	free((*l)->sentinel);
	free(*l);
}

/*----------ajoute un élément au début de la liste l-------------------------------------------------------*/

List *list_push_front(List *l, int v) {
	LinkedElement *val= malloc(sizeof(LinkedElement));
	val->value= v;
	val->next= l->sentinel->next;
	val->previous= l->sentinel;
	l->sentinel->next->previous= val;
	l->sentinel->next= val;
	++(l->size);
	return l;
}

/*----------retourne la valeur de l'élément à la tête de la liste-------------------------------------------------------*/

int list_front(List *l) {
	assert(l->size!=0);
	return l->sentinel->next->value;
}

/*---------retourne la valeur du dernier élément de la liste--------------------------------------------------------*/

int list_back(List *l) {
	assert(l->size!=0);
	return l->sentinel->previous->value;
}

/*---------supprime l'élément en tête de la liste--------------------------------------------------------*/

List *list_pop_front(List *l) {
	assert(l->size!=0);
	LinkedElement *oldTop= l->sentinel->next;
	oldTop->next->previous= l->sentinel;
	l->sentinel->next= oldTop->next;
	free(oldTop);
	--(l->size);
	return l;
}

/*---------supprime l'élément à la fin de la liste--------------------------------------------------------*/

List *list_pop_back(List *l){
	assert(l->size!=0);
	LinkedElement *oldBack= l->sentinel->previous;
	oldBack->previous->next= l->sentinel;
	l->sentinel->previous= oldBack->previous;
	free(oldBack);
	--(l->size);
	return l;
}

/*------insère à la position p la valeur v dans la liste l-----------------------------------------------------------*/

List *list_insert_at(List *l, int p, int v) {
	LinkedElement *element= l->sentinel->next;
	for(;p;--p,element=element->next);

	LinkedElement *valToInsert= malloc(sizeof(LinkedElement));
	valToInsert->value=v;
	valToInsert->next=element;
	valToInsert->previous=element->previous;
	valToInsert->previous->next=valToInsert;
	valToInsert->next->previous=valToInsert;
	++(l->size);;
	return l;
}

/*-----supprime l'élément à la position p de la liste l------------------------------------------------------------*/

List *list_remove_at(List *l, int p) {
	LinkedElement *valToRemove= l->sentinel->next;
	for(;p;--p,valToRemove=valToRemove->next);

	valToRemove->previous->next= valToRemove->next;
	valToRemove->next->previous= valToRemove->previous;
	free(valToRemove);
	--(l->size);
	return l;
}

/*------retourne la valeur de l'élément à la position p de la liste l-----------------------------------------------------------*/

int list_at(List *l, int p) {
	LinkedElement *element= l->sentinel->next;
	for(;p;--p,element=element->next);
	return element->value;
}

/*------vérifie si une liste est vide ou pas-----------------------------------------------------------*/

bool list_is_empty(List *l) {
	return l->size==0;
}

/*-------retourne la taille de la liste----------------------------------------------------------*/

int list_size(List *l) {
	return l->size;
}

/*------remplace tous les éléments de la liste l par l'application de la fonction f sur chacun de ses éléments-----------------------------------------------------------*/

List * list_map(List *l, SimpleFunctor f) {
	for(LinkedElement *element=l->sentinel->next; element!=l->sentinel; element=element->next) element->value=f(element->value);
	return l;
}

/*-----fait une synthèse de la liste l en mettant la valeur de cette synthèse dans la valeur pointée par userData----------------------------------------------------------------*/
List *list_reduce(List *l, ReduceFunctor f, void *userData) {
	for(LinkedElement *element=l->sentinel->next; element!=l->sentinel; element=element->next) f(element->value,userData);
	return l;
}

/*-----divise la liste en deux sous liste de égale à 1 élément près------------------------------------------------------------*/
SubList list_split(SubList l)
{
	SubList listSub= malloc(sizeof(struct _SubList));
	listSub->head=l->head;
	listSub->tail=l->tail;
	/*vérification du cas où la liste serait vide*/
	if(listSub->head==listSub->tail)
	{
		listSub->tail=NULL;
		return listSub;
	}

	while (listSub->head->next!=listSub->tail)
	{
		listSub->head= listSub->head->next;
		if(listSub->tail->previous!=listSub->head)
			listSub->tail= listSub->tail->previous;
	}
	
	return listSub;
}

/*fonction qui ajoute un élément dans une liste doublement chainée sans sentinelle (fonction utiliteur)*/
SubList sub_list_add(SubList l, int val)
{
	LinkedElement **e= (l->head==NULL)? &(l->head): &(l->tail->next);
    *e= malloc(sizeof(LinkedElement)); (*e)->value=val;
    (*e)->next=NULL;
	(*e)->previous=l->tail;
	l->tail=*e;
	return l;
}


/*-----fusionne deux listes triées------------------------------------------------------------*/
SubList list_merge(SubList leftlist, SubList rightlist,OrderFunctor f)
 {
	int val1,val2;
	SubList listMerge= malloc(sizeof(struct _SubList));
	
	//le cas où les deux listes seraient vide
	if( leftlist->head==NULL && rightlist->head==NULL)
		return listMerge;
	
	while(leftlist->head!=NULL && rightlist->head!=NULL)
	{
		val1=leftlist->head->value; val2=rightlist->head->value;
		
		if(f(val1,val2))
		{
			sub_list_add(listMerge,val1);
			leftlist->head=leftlist->head->next;
		}
		else if(f(val2,val1))
		{
			sub_list_add(listMerge,val2);
			rightlist->head=rightlist->head->next;
		}
		else
		{
			sub_list_add(listMerge,val1); sub_list_add(listMerge,val2);
			leftlist->head=leftlist->head->next; rightlist->head=rightlist->head->next;
		}
	}

	while(leftlist->head!=NULL)
	{
		sub_list_add(listMerge,leftlist->head->value);
		leftlist->head=leftlist->head->next;
	}

	while(rightlist->head!=NULL)
	{
		sub_list_add(listMerge,rightlist->head->value);
		rightlist->head=rightlist->head->next;
	}

	return listMerge;
}

/*-----trie une liste chaînée selon l'ordre de f (cette fonction est récursive)------------------------------------------------------------*/
SubList list_mergesort(SubList l, OrderFunctor f)
{
	if(l==NULL || l->head==l->tail) //Si une liste pointe vers une mémoire null ou si la liste contient un seul élément->on retourne la liste
		return l;

	
	SubList listSub= list_split(l);
	SubList listleft= malloc(sizeof(struct _SubList)); 
	listleft->head=l->head; 
	listleft->tail=listSub->head;

	if (listleft->tail!=NULL) listleft->tail->next=NULL;

	SubList listright= malloc(sizeof(struct _SubList)); 
	listright->head=listSub->tail;
	listright->tail=l->tail; 

	if (listright->head!=NULL) listright->head->previous=NULL;

	SubList listmerge= list_merge(list_mergesort(listleft,f),list_mergesort(listright,f),f);

	//free(listSub); free(listleft); free(listright);
	return listmerge;

}

/*---transforme la liste l en une liste sans sentinelle et la trie; puis retourne le résultat sous forme d'une liste avec sentinelle--------------------------------------------------------------*/

List *list_sort(List *l, OrderFunctor f) {
	//transformation de l en une liste sans sentinelle
	SubList listSub= malloc(sizeof(struct _SubList));
	for(LinkedElement *e=l->sentinel->next; e!=l->sentinel; e=e->next) 
	{
		sub_list_add(listSub,e->value);
		free(e);
	}

	listSub=list_mergesort(listSub,f);
	//retransformation de la liste sans sentinelle en une liste avec sentinelle
	l->sentinel->next= listSub->head; l->sentinel->previous=listSub->tail;
	l->sentinel->next->previous=l->sentinel->previous->next=l->sentinel;
	free(listSub);
	return l;

}

