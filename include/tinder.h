#ifndef TINDER_H
#define TINDER_H

#include "graph.h"
#include "error.h"
#include "profile.h"
#include <stdbool.h>

typedef struct {
	Graph* graph;
	VecProfile vecProfiles;
} Tinder;

Tinder tinder_create(Error* error);

int tinder_getMatch(Profile p1, Profile p2);

void tinder_addProfile(Tinder* tinder, Profile profile, Error* error);

void tinder_printProfile(Profile profile);

VecProfile tinder_getProfiles(Tinder tinder);

VecProfile tinder_getConnected(Tinder tinder, int id, Status status);

// bool tinder_listProfiles(Tinder tinder);

// bool tinder_listConnected(Tinder tinder, int id, Status status);

Profile tinder_perfectMatch(Tinder tinder, int id);

void tinder_addFriend(Tinder tinder, int id, int toAdd, Error* error);

void tinder_destroy(Tinder tinder, Error* error);

void tinder_acceptRequest(Tinder tinder, int id, int requestingId, Error* error);

#endif