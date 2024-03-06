/*-------------------------------------------------------------------
		Licence Informatique - Structures de données
		Mathias Paulin (Mathias.Paulin@irit.fr)
-------------------------------------------------------------------*/
/**
\mainpage
 \section TP3 Implantation et extension d'un TAD
 \subsection Objectifs Objectifs du TP :
    - Implantation du TAD List étudié en cours.
	- Représentation par liste doublement chainée avec sentinelle.
	- Ajout d'un opérateur de tri (algorithme de tri fusion)
 \subsection Resultats Resultats attendus
 \subsubsection Exercice1 Exercice 1
 \code{.unparsed}
 $ ./list_test 1
 -------- TEST PUSH_BACK --------
 List (10) : 0 1 2 3 4 5 6 7 8 9
 $
 \endcode
 
 \subsubsection Exercice2 Exercice 2
 \code{.unparsed}
 $ ./list_test 2
 -------- TEST PUSH_BACK --------
 List (10) : 0 1 2 3 4 5 6 7 8 9
 -------- TEST PUSH_FRONT --------
 List (10) : 9 8 7 6 5 4 3 2 1 0
 Sum is 45
 $
 \endcode
 
 \subsubsection Exercice3 Exercice 3
 \code{.unparsed}
 $ ./list_test 3
 -------- TEST PUSH_BACK --------
 List (10) : 0 1 2 3 4 5 6 7 8 9
 -------- TEST PUSH_FRONT --------
 List (10) : 9 8 7 6 5 4 3 2 1 0
 Sum is 45
 -------- TEST POP_FRONT --------
 Pop front : 9
 List (9) : 8 7 6 5 4 3 2 1 0
 -------- TEST POP_BACK --------
 Pop back : 0
 List (8) : 8 7 6 5 4 3 2 1
 $
 \endcode
 
 \subsubsection Exercice4 Exercice 4
 \code{.unparsed}
 $./list_test 4
 -------- TEST PUSH_BACK --------
 List (10) : 0 1 2 3 4 5 6 7 8 9
 -------- TEST PUSH_FRONT --------
 List (10) : 9 8 7 6 5 4 3 2 1 0
 Sum is 45
 -------- TEST POP_FRONT --------
 Pop front : 9
 List (9) : 8 7 6 5 4 3 2 1 0
 -------- TEST POP_BACK --------
 Pop back : 0
 List (8) : 8 7 6 5 4 3 2 1
 -------- TEST INSERT_AT	--------
 List (10) : 0 2 4 6 8 9 7 5 3 1
 -------- TEST REMOVE_AT	--------
 List (4) : 2 6 9 5
 List cleared (0)
 -------- TEST AT	--------
 List (10) : 0 1 2 3 4 5 6 7 8 9
 $
 \endcode
 
 \subsubsection Exercice5 Exercice 5
 \code{.unparsed}
 $./list_test 5
 -------- TEST SORT	--------
 Unsorted list    : List (8) : 5 3 4 1 6 2 3 7
 Decreasing order : List (8) : 7 6 5 4 3 3 2 1
 Increasing order : List (8) : 1 2 3 3 4 5 6 7
 $
 \endcode

*/
/*-----------------------------------------------------------------*/
/** \defgroup Main main
 Test of the implementation of the abstract data type List.
 @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/** \defgroup ListOperators Operators applied to the list.
@{
*/
/** Operator to be used with list_map to print the content of the list.
 @see SimpleFunctor
 */
int printList(int i){
	printf("%d ", i);
	return i;
}

/** Operator to be used with list_reduce to compute the sum of the element of the list.
 @see ReduceFunctor
 */
int accumulate(int v, void *acc) {
	*((int*)acc)+=v;
	return v;
}

/** Operator to sort the list in increasing order with list_sort
 @see OrderFunctor
 */
bool lt(int i, int j) {
	return i<j;
}

/** Operator to sort the list in decreasing order with list_sort
 @see OrderFunctor
 */
bool gt(int i, int j) {
	return i>j;
}

/** @} */

/** \defgroup MainFunction Main function for testing.
  @{
*/
/** Main function of the test program.
 See \ref Resultats for the expected output of this function.
 */
int main(int argc, char **argv){
	
	if (argc<2) {
		fprintf(stderr,"usage : %s num_exercice\n", argv[0]);
		return 1;
	}

	int num_exercice = atoi(argv[1]);
	if (num_exercice < 5) {
		List *l;
		if (num_exercice >= 1) {
			printf("-------- TEST PUSH_BACK --------\n");
			l = list_create();
			for (int i=0; i<10 ;i++)
				list_push_back(l, i);
			printf("List (%d) : ", list_size(l));
			list_map(l, printList);
			printf("\n");
			
			list_delete(&l);
		}
		
		if (num_exercice >= 2) {
			printf("-------- TEST PUSH_FRONT --------\n");
			l = list_create();
			for (int i=0; i<10 ;i++)
				list_push_front(l, i);
			printf("List (%d) : ", list_size(l));
			list_map(l, printList);
			printf("\n");
			int sum = 0;
			list_reduce(l, accumulate, &sum);
			printf("Sum is %d\n", sum);
		}
		
		if (num_exercice >= 3) {
			printf("-------- TEST POP_FRONT --------\n");
			printf("Pop front : %d\n", list_front(l));
			l = list_pop_front(l);
			printf("List (%d) : ", list_size(l));
			list_map(l, printList);
			printf("\n");
		
			printf("-------- TEST POP_BACK --------\n");
			printf("Pop back : %d\n", list_back(l));
			l = list_pop_back(l);
			printf("List (%d) : ", list_size(l));
			list_map(l, printList);
			printf("\n");
		}
		
		if (num_exercice >= 2) {
			list_delete(&l);
		}
		
		if (num_exercice >= 4) {
			printf("-------- TEST INSERT_AT	--------\n");
			l = list_create();
			for (int i=0; i<10 ;i++)
				list_insert_at(l, i/2 + i%2 , i);
			printf("List (%d) : ", list_size(l));
			list_map(l, printList);
			printf("\n");

			printf("-------- TEST REMOVE_AT	--------\n");
			for (int i=0; i<5 ;i++)
				list_remove_at(l, i);
			list_remove_at(l, list_size(l)-1);
			printf("List (%d) : ", list_size(l));
			list_map(l, printList);
			printf("\n");

			while(list_size(l))
				list_remove_at(l, list_size(l)/2);
			printf("List cleared (%d) \n", list_size(l));
			
			printf("-------- TEST AT	--------\n");
			for (int i=0; i<10 ;i++)
				list_push_back(l, i);

			printf("List (%d) : ", list_size(l));
			for (int i=0; i<list_size(l); ++i)
				printf("%d ", list_at(l, i));
			printf("\n");

			list_delete(&l);
		}
	} else {
		printf("-------- TEST SORT	--------\n");
		List * l = list_create();
		list_push_back(l, 5);
		list_push_back(l, 3);
		list_push_back(l, 4);
		list_push_back(l, 1);
		list_push_back(l, 6);
		list_push_back(l, 2);
		list_push_back(l, 3);
		list_push_back(l, 7);
		printf("Unsorted list    : List (%d) : ", list_size(l));
		list_map(l, printList);
		printf("\n");

		l = list_sort(l, gt);
		printf("Decreasing order : List (%d) : ", list_size(l));
		list_map(l, printList);
		printf("\n");

		l = list_sort(l, lt);
		printf("Increasing order : List (%d) : ", list_size(l));
		list_map(l, printList);
		printf("\n");

		list_delete(&l);
	}
	return 1;
}
/** @} */

/** @} */
