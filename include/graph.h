#ifndef GRAPH_H
#define GRAPH_H

#include "error.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct Graph Graph;

/** Create and return a graph with nVertex vertex. */
Graph* graph_create(size_t elemSize, Error* error);

/** Free the memory usage of the graph */
void graph_destroy(Graph* g, Error* error);

/** Returns a copy of the graph g */
Graph* graph_copy(Graph* g, Error* error);

/** Adds a new vertex to the graph, reallocating space if necessary */
int graph_addVertex(Graph* g, Error* error);

/** Adds the edge u->v on the graph. */
void graph_addEdge(Graph* g, int u, int v, void* w, Error* error);

/** Removes the edge u->v from a graph. */
void graph_removeEdge(Graph* g, int u, int v, Error* error);

/** Returns the degree of a vertex */
int graph_degreeOfVertex(Graph* g, int u, Error* error);

/** Returns the next vertex adjacent to u, in range [first, nVertex) */
int graph_nextAdj(Graph* g, int u, int first, Error* error);

/** Returns a pointer to the weight of the edge u->v. */
void* graph_edgeWeight(Graph* g, int u, int v, Error* error);

#endif
