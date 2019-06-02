#ifndef TINDER_H
#define TINDER_H

#include "graph.h"
#include "error.h"
#include <stdbool.h>

#define MAXSTR 256
#define MAXINFO 6

typedef enum {
	FRIEND, UNKNOWN, REQUESTING, REQUESTED
} Status;

typedef struct {
	int id;

	int age;
	char name[MAXSTR], info[MAXINFO][MAXSTR];
} Profile;

typedef struct {
	Graph* graph;
	Profile* profiles;
	size_t nProfiles;
} Tinder;

Tinder tinder_create(Error* error);

int tinder_getMatch(Profile p1, Profile p2);

void tinder_addProfile(Tinder* tinder, Profile profile, Error* error);

void tinder_printProfile(Profile profile);

bool tinder_listProfiles(Tinder tinder);

bool tinder_listConnected(Tinder tinder, int id, Status status);

Profile tinder_perfectMatch(Tinder tinder, int id);

void tinder_addFriend(Tinder tinder, int id, int toAdd, Error* error);

void tinder_destroy(Tinder tinder, Error* error);

void tinder_acceptRequest(Tinder tinder, int id, int requestingId, Error* error);

#endif