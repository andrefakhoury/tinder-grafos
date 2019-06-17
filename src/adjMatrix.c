#include "graph.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/** Macro for an empty element of the matrix */
#define EMPTY NULL

/** First alloc size */
#define QTTALLOC 256

/** Struct of a graph with adjacency Matrix */
struct Graph {
	void*** mat;
	size_t nVertex, elemSize, allocated;
};

/** Create and return a graph with nVertex vertex. */
Graph* graph_create(size_t elemSize, Error* error) {
	Graph* g = malloc(sizeof(Graph));
	if (g == NULL) {
		error->occurred = true;
		strcpy(error->msg, "No space for allocation.");
		return NULL;
	}

	g->nVertex = 0;
	g->elemSize = elemSize;
	g->allocated = QTTALLOC;

	g->mat = malloc(QTTALLOC * sizeof(void**));
	if (g->mat == NULL) {
		error->occurred = true;
		strcpy(error->msg, "No space for allocation.");
		graph_destroy(g, error);
		return NULL;
	}

	for (int i = 0; i < QTTALLOC; i++) {
		g->mat[i] = malloc(QTTALLOC * sizeof(void*));
		if (g->mat[i] == NULL) {
			error->occurred = true;
			strcpy(error->msg, "No space for allocation.");
			graph_destroy(g, error);
			return NULL;
		}

		for (int j = 0; j < QTTALLOC; j++) {
			g->mat[i][j] = EMPTY;
		}
	}

	error->occurred = false;
	return g;
}

/** Free the memory usage of the graph */
void graph_destroy(Graph* g, Error* error) {
	if (g != NULL) {
		if (g->mat != NULL) {
			for (int i = 0; i < g->allocated; i++) {
				if (g->mat[i] != NULL) {
					for (int j = 0; j < g->allocated; j++) {
						if (g->mat[i][j] != EMPTY)
							free(g->mat[i][j]);
					}
				}
				free(g->mat[i]);
			}
			free(g->mat);
		}
		free(g);
	}
}

/** Returns a copy of the graph g */
Graph* graph_copy(Graph* g, Error* error) {
	if (g == NULL) {
		error->occurred = true;
		strcpy(error->msg, "The original graph is null.");
		return NULL;
	}

	Graph* r = graph_create(g->elemSize, error);
	if (error->occurred) {
		return NULL;
	}

	r->elemSize = g->elemSize;
	r->nVertex = g->nVertex;
	r->allocated = g->allocated;

	for (int u = 0; u < g->allocated; u++) {
		for (int v = 0; v < g->allocated; v++) {
			if (g->mat[u][v] != NULL) {
				r->mat[u][v] = malloc(r->elemSize);
				memcpy(r->mat[u][v], g->mat[u][v], r->elemSize);
			}
		}
	}

	return r;
}

/** Adds a new vertex to the graph, reallocating space if necessary */
int graph_addVertex(Graph* g, Error* error) {
	// TODO erro

	if (g == NULL) {
		error->occurred = true;
		strcpy(error->msg, "Invalid graph given");
		return -1;
	}

	int u = g->nVertex;

	if (u >= g->allocated) { // need to realloc
		g->allocated += QTTALLOC;

		g->mat = realloc(g->mat, g->allocated * sizeof(void**));
		if (g->mat == NULL) {
			error->occurred = true;
			strcpy(error->msg, "No space for allocation.");
			graph_destroy(g, error);
			return -1;
		}

		for (int i = g->nVertex; i < g->allocated; i++) {
			g->mat[i] = malloc(g->allocated * sizeof(void*));
			
			for (int j = 0; j < g->allocated; j++) {
				g->mat[i][j] = EMPTY;
			}
		}

		for (int i = 0; i < g->nVertex; i++) {
			g->mat[i] = realloc(g->mat[i], g->allocated * sizeof(void*));

			for (int j = g->nVertex; j < g->allocated; j++) {
				g->mat[i][j] = EMPTY;
			}
		}
	}

	g->nVertex++;

	return u;
}

/** Adds the edge u->v on the graph. */
void graph_addEdge(Graph* g, int u, int v, void* w, Error* error) {
	if (u >= g->nVertex || v >= g->nVertex) {
		error->occurred = true;
		strcpy(error->msg, "Invalid vertex number.");
		return;
	}

	error->occurred = false;

	// if (g->mat[u][v] != EMPTY) {
	// 	graph_removeEdge(g, u, v, error);
	// }

	g->mat[u][v] = malloc(g->elemSize);
	memcpy(g->mat[u][v], w, g->elemSize);
}

/** Removes the edge u->v from a graph. */
void graph_removeEdge(Graph* g, int u, int v, Error* error) {
	if (u >= g->nVertex || v >= g->nVertex) {
		error->occurred = true;
		strcpy(error->msg, "Invalid vertex number.");
		return;
	}

	error->occurred = false;

	if (g->mat[u][v] != EMPTY) {
		free(g->mat[u][v]);
		g->mat[u][v] = EMPTY;
	}
}

/** Returns the degree of a vertex */
int graph_degreeOfVertex(Graph* g, int u, Error* error) {
	if (u >= g->nVertex) {
		error->occurred = true;
		strcpy(error->msg, "Invalid vertex number.");
		return 0;
	}

	error->occurred = false;

	int degree = 0;
	for (int v = 0; v < g->nVertex; v++) {
		if (g->mat[u][v] != EMPTY)
			degree++;
	}

	return degree;
}

/** Returns the next vertex adjacent to u, in range [first, nVertex) */
int graph_nextAdj(Graph* g, int u, int first, Error* error) {
	if (u >= g->nVertex || first >= g->nVertex) {
		error->occurred = true;
		strcpy(error->msg, "Invalid vertex number.");
		return -1;
	}

	error->occurred = false;
	for (int i = first; i < g->nVertex; i++) {
		if (g->mat[u][i] != EMPTY) {
			return i;
		}
	}

	return 0;
}

/** Returns a pointer to the weight of the edge u->v. */
void* graph_edgeWeight(Graph* g, int u, int v, Error* error) {
	if (g == NULL) {
		error->occurred = true;
		strcpy(error->msg, "Given graph is null.");
		return NULL;
	}

	if (u >= g->nVertex || v >= g->nVertex) {
		error->occurred = true;
		strcpy(error->msg, "Invalid vertex number.");
		return NULL;
	}

	return g->mat[u][v];
}

bool graph_edgeIsSet(Graph* g, int u, int v, Error* error) {
	//TODO: erro

	bool ret = g->mat[u][v] != EMPTY;

	return ret;
}