#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

void add_edge (vertex_t **vtxhead, char *v1_name, char *v2_name, int weight) {

if (*vtxhead == NULL)  { //base case: only executes once
	vertex_t *v1;
	vertex_t *v2;
	adj_vertex_t *adjv;
	
	*vtxhead = v1 = (vertex_t *) malloc(sizeof(vertex_t));
	
	v1->name = v1_name;
	v2 = (vertex_t *)malloc(sizeof(vertex_t));
	v2->name = v2_name;

	v1->next = v2;
	v2->next = NULL;

	adjv = v1->adj_list =(adj_vertex_t *) (malloc(sizeof(adj_vertex_t)));
	adjv->edge_weight = weight;
	adjv->vertex = v2;
	adjv->next = NULL;
  	
	adjv = v2->adj_list = (adj_vertex_t *) malloc(sizeof(adj_vertex_t));
	adjv->edge_weight = weight;
	adjv->vertex = v1;
	adjv->next = NULL;	
}

else if (*vtxhead != NULL) {

findorMakeVertex(vtxhead, v1_name, v2_name, weight);

}
}

void findorMakeVertex(vertex_t **vtxhead, char *v1_name, char *v2_name, int weight) { 

	vertex_t *v;
	v = *vtxhead;
	
	int c1,c2;
	while (1) {
	if (v != NULL) {
	if (strcmp((v->name), v1_name) == 0) { //Will execute if the first vertex exists and the second one doesn't
		c1 = 1;
		break;
	}
	else v = v->next;
	}
	else if (v == NULL) {
		c1 = 0;
		break;
	}
	}
	
	v = *vtxhead;

	while (1) {
	if (v != NULL) {
	if (strcmp((v->name), v2_name) == 0) { //Will execute if the first vertex exists and the second one doesn't
		c2 = 1;
		break;
	}
	else v = v->next;
	}
	else if (v == NULL) {
		c2 = 0;
		break;
	}
	}

	v = *vtxhead;
	
	if ((c1 == 1) && (c2 == 0)) {
		while (1) {
		 if (v != NULL) {	
		  
		  if (strcmp((v->name), v1_name) == 0) { //Will execute if the first vertex exists and the second one doesn't

			vertex_t *v1;
			v1 = (vertex_t *) malloc(sizeof(vertex_t));
			v1->name = v2_name;
			v1->next = NULL;
			
			vertex_t *findLastV;
			findLastV = v;

			while (1) {
			if (findLastV->next != NULL) {
				findLastV = findLastV->next; 
			} //end if
			else break;
			} //end while

			findLastV->next = v1;
			
			addAdjVertex(v, v1, weight); //adds v1 to v's adj_list

			addAdjVertex(v1, v, weight); //adds v to v1's adj_list
			
			break;
		  } //end inner-if 
		  else v = v->next;
			}
		}
	} 
	
	else if ((c1 == 0) && (c2 == 1)) {
		while(1) {
			if (v != NULL) {
		vertex_t *v1;
	  	v1 = (vertex_t *) malloc(sizeof(vertex_t));
		v1->name = v1_name;
		v1->next = NULL;

		vertex_t *findLastV;
		findLastV = *vtxhead;

		while (1) { //finds last vertex
		if (findLastV->next != NULL) {
			findLastV = findLastV->next; 
		} //end if
		else break;
		} //end while

		findLastV->next = v1;
		
		vertex_t *v2;
		v2 = *vtxhead;

		while (1) { //finding where the existing vertex is
		if (v2 != NULL) {
		if (strcmp((v2->name), v2_name) == 0) {
			break;
		}//end inner-if
		else v2 = v2->next;
		}
		}//end while
		
		addAdjVertex(v2, v1, weight); //adds v1 to v2's adj list

		addAdjVertex(v1, v2, weight); //adds v2 to v1's adj list

		break;				
	    }
	} 
	}

	else if ((c1 == 1) && (c2 == 1)) {
		vertex_t *x, *y;
		x = *vtxhead;
		y = *vtxhead;

		while (1) {
			if (x != NULL) {
			if (strcmp((x->name), v1_name) == 0) {
				break;
			}
			else x = x->next;
			}
		}
		while (1) {
			if (y != NULL) {
			if (strcmp((y->name), v2_name) == 0) {
				break;
			}
			else y = y->next;
			}
		}

		addAdjVertex(x, y, weight); //adds v1 to v2's adj list

		addAdjVertex(y, x, weight); //adds v2 to v1's adj list

	}
}

void addAdjVertex(vertex_t *v, vertex_t *y, int weight) { //v has the adj list we're adding to, v1 is the vertex the adj list
	adj_vertex_t *adj, *adj1;						  //points to.

	adj = v->adj_list;
	
	while (1) {
		if (adj == 0) {
		 adj1 = (adj_vertex_t *) malloc(sizeof(adj_vertex_t));
		 adj1->edge_weight = weight;
		 adj1->next = NULL;
		 adj1->vertex = y;
		 v->adj_list = adj1;
		 break;
		}
		else if (adj->next == 0) {
		 adj1 = (adj_vertex_t *) malloc(sizeof(adj_vertex_t));
		 adj1->edge_weight = weight;
		 adj1->next = NULL;
		 adj1->vertex = y;
		 adj->next = adj1;
		 break;
		}
		else adj = adj->next;
	}
}

void findTour(vertex_t **vtxhead) {	
	vertex_t *vp, *len;
	vp = *vtxhead;
	adj_vertex_t *adj;
	int i, j;
	int checkTour = 1; //1 if tour exists, 0 if tour doesn't exist
	for (len = *vtxhead; len != NULL; len = len->next) {
		i++;
	}
	char visited[i];
	
	for (vp = *vtxhead; vp != NULL; vp = vp->next) { //try to find a path for each vertex
		visited[0] = *(vp->name);
		for (j = 1; j < i; j++) {
			visited[j] = 0;
		}

		char *visitedPointer, *appendedPointer;
		visitedPointer = visited;
		appendedPointer = visited;
		int tourWeight = 0;
		for (adj = vp->adj_list; adj != NULL; adj = adj->next) {
			findTourHelper(adj, visitedPointer, appendedPointer, &tourWeight);
		}

		for (j = 0; j < i; j++) {
			if (visited[j] == 0) {
				checkTour = 0;
				break; 
			}
		}
		if (checkTour == 0) {
			printf("There's no tour starting at vertex %c", visited[0]);
			printf("\n");
		}
		else if (checkTour == 1) {
			printf("There's a tour at %c.\n", visited[0]);
			printf("TOUR: ");
			for (j = 1; j < i; j++) {
				printf("%c ", visited[j]);
			}
				printf("\n");
				printf("WEIGHT: %d", tourWeight);
				printf("\n");


		}
	} 
}
void findTourHelper(adj_vertex_t *adj, char *visitedPointer, char *appendedPointer, int*tourWeight) {
	vertex_t *v;
	v = adj->vertex;
		
	while (1){
	if(*(visitedPointer) != 0) { //was null
		if (*(v->name) != *visitedPointer) {
		visitedPointer = visitedPointer + 1; 
		}
		else if (*(v->name) == *visitedPointer) { //same name, so break
			break;
		}
	}
	else {
		*visitedPointer = *(v->name); //adds to array // this line of code
		vertex_t *y;
		adj = v->adj_list;           // is incredibly annoying
		y = adj->vertex;
		*(visitedPointer - 1) = *(y->name);
		if (*(y->name) == *appendedPointer) {
			*tourWeight+= adj->edge_weight;
			adj = adj->next;
			
		}
		if (v->next != NULL) { 
			if (adj == NULL) {
			*visitedPointer = 0; 
			visitedPointer = visitedPointer - 1;
			//*tourWeight-= adj->edge_weight;
			break;
			}
		}
		else if (v->next == NULL) {
			if (adj == NULL) {
				return;
			} //new conditional
			else if (adj->next == NULL) {
				*tourWeight = (*tourWeight) + (adj->edge_weight);
				return; //this seems like it'll work
			}
		}

		appendedPointer = appendedPointer + 1;
		findTourHelper(adj, visitedPointer, appendedPointer, tourWeight);
		return;
	}
    }
}

void freeGraph(vertex_t *vtx) {
	
	while (1) {
	if (vtx->next != NULL) {

		freeGraph(vtx->next);
		adj_vertex_t *temp, *adj;
		adj = vtx->adj_list;
	
		while (adj != NULL) {
		temp = adj->next;
		free(adj);
		adj = temp;
		}
		free(vtx);
		break;
	}
	else {
		adj_vertex_t *temp, *adj;
		adj = vtx->adj_list;
	
		while (adj != NULL) {
		temp = adj->next;
		free(adj);
		adj = temp;
		}
		free(vtx);
		break;
	}
	}
}
	






