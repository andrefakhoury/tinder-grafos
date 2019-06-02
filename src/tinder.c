#include "tinder.h"

typedef struct {
	enum {
		FRIEND, UNKNOWN, REQUESTING, REQUESTED
	} Status;

	int match;
} Relation;

int create () {
    Error error;
	Graph* g = graph_create(sizeof(Relation), &error);

	graph_destroy(g, &error);
}