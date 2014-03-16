#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main (int argc, char *argv[]) {
	int i = 1;
	vertex_t **vhead, *vp;
	*vhead = NULL;
	adj_vertex_t *adj_v;
         //initially assigned to NULL
	while ((argv[i] != NULL) && (argv[i + 1] != NULL) && (argv[i + 2] != NULL)) {
	    int intWeight = atoi(argv[i + 2]) ;
	    add_edge(vhead, argv[i], argv[i + 1], intWeight);
	    i = i + 3;
	}

	//print out our adjacency list 
	printf("Adjacency list:\n");
	for (vp = *vhead; vp != NULL; vp = vp->next) {
		printf("  %s: ", vp->name);
		for (adj_v = vp->adj_list; adj_v != NULL; adj_v = adj_v->next) {
			printf("%s(%d) ", adj_v->vertex->name, adj_v->edge_weight);
		}
		printf("\n");
	}
	printf("Finding whether tour exists:\n");
	findTour(vhead);

        freeGraph(*vhead);

	return 0;
}
