#ifndef TINDER_H
#define TINDER_H

#include "graph.h"
#include "error.h"
#define MAXSTR 256
#define MAXINFO 6

typedef struct {
	int id;

	int age;
	char name[MAXSTR], info[MAXINFO][MAXSTR];
} Profile;

typedef struct {
	Graph* graph;
	Profile* profiles;
} Tinder;

#endif