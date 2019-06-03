#include "tinder.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	Status status;
	int match;
} Relation;

Tinder tinder_create(Error* error) {
	Tinder tinder;
	tinder.graph = graph_create(sizeof(Relation), error);
	tinder.profiles = NULL;
	tinder.nProfiles = 0;

	return tinder;
}

int tinder_getMatch(Profile p1, Profile p2) {
	int match = 0;

	for (int i = 0; i < MAXINFO; i++) {
		match += !strcmp(p1.info[i], p2.info[i]);
	}

	return match;
}

void tinder_addProfile(Tinder* tinder, Profile profile, Error* error) {

	tinder->nProfiles++;
	profile.id = graph_addVertex(tinder->graph, error);

	for (int i = 0; i < tinder->nProfiles; i++) {
		if (i == profile.id) {
			continue;
		}

		Relation relation;
		relation.match = tinder_getMatch(profile, tinder->profiles[i]);
		relation.status = UNKNOWN;

		graph_addEdge(tinder->graph, profile.id, i, &relation, error);
		graph_addEdge(tinder->graph, i, profile.id, &relation, error);
	}

	tinder->profiles = realloc(tinder->profiles, tinder->nProfiles * sizeof(Profile));
	tinder->profiles[tinder->nProfiles - 1] = profile;
}

void tinder_printProfile(Profile profile) {
	const char infoDescription[MAXINFO][MAXSTR] = {"City", "Movie", "Team", "Favorite Color", "Music Band", "Neymar Tweet"};
	printf("Profile - %s [%d]\n", profile.name, profile.id);
	printf("Age: %d\n", profile.age);
	for (int i = 0; i < MAXINFO; i++) {
		printf("%s: %s\n", infoDescription[i], profile.info[i]);
	}
}

bool tinder_listProfiles(Tinder tinder) {
	if (tinder.nProfiles == 0) {
		printf("No profiles...\n");
		return false;
	}

	printf("\n[Profile Selection]\nListing profiles:\n");
	for (int i = 0; i < tinder.nProfiles; i++) {
		printf("  [%d] %s\n", tinder.profiles[i].id, tinder.profiles[i].name);
	}

	printf("\n");
	return true;
}

bool tinder_listConnected(Tinder tinder, int id, Status status) {
	int qtt = 0;

	for (int i = 0; i < tinder.nProfiles; i++) {
		if (i == id) {
			continue;
		}

		Error error;
		Relation* relation = (Relation*)graph_edgeWeight(tinder.graph, id, i, &error);
		if (relation->status == status) {
			tinder_printProfile(tinder.profiles[i]);
			qtt++;
		}
	}

	return qtt;
}

Profile tinder_perfectMatch(Tinder tinder, int id) {
	Profile profile;
	int maxMatch = -1;

	Error error;
	for (int i = 0; i < tinder.nProfiles; i++) {
		if (i == id) {
			continue;
		}

		int curMatch = ((Relation*)(graph_edgeWeight(tinder.graph, id, i, &error)))->match;
		if (curMatch > maxMatch || (curMatch == maxMatch && rand()&1)) {
			maxMatch = curMatch;
			profile = tinder.profiles[i];
		}
	}

	return profile;
}

void tinder_addFriend(Tinder tinder, int id, int toAdd, Error* error) {
	error->occurred = false;

	if (id < 0 || toAdd < 0 || id >= tinder.nProfiles || toAdd >= tinder.nProfiles) {
		error->occurred = true;
		strcpy(error->msg, "Invalid id");
		return;
	}

	Relation* relation = (Relation*)graph_edgeWeight(tinder.graph, id, toAdd, error);
	if (error->occurred || relation->status == FRIEND || relation->status == REQUESTED) {
		error->occurred = true;
		sprintf(error->msg, "Invalid friend request.. %ld", tinder.nProfiles);
		if (relation->status == FRIEND) {
			sprintf(error->msg, "Friend...");
		} else if (relation->status == REQUESTED) {
			sprintf(error->msg, "Requested...");
		}

		return;
	}

	error->occurred = false;
	relation->status = REQUESTING;
	graph_addEdge(tinder.graph, id, toAdd, relation, error);
	relation->status = REQUESTED;
	graph_addEdge(tinder.graph, toAdd, id, relation, error);
}

void tinder_acceptRequest(Tinder tinder, int id, int requestingId, Error* error) {
	error->occurred = false;
	
	if (id < 0 || requestingId < 0 || id >= tinder.nProfiles || requestingId >= tinder.nProfiles) {
		error->occurred = true;
		strcpy(error->msg, "Invalid id");
		return;
	}

	Relation* relation = (Relation*)graph_edgeWeight(tinder.graph, id, requestingId, error);
	if (error->occurred || relation->status == FRIEND || relation->status != REQUESTED) {
		error->occurred = true;
		strcpy(error->msg, "Invalid status");
		return;
	}

	relation->status = FRIEND;
	graph_addEdge(tinder.graph, id, requestingId, relation, error);
	graph_addEdge(tinder.graph, requestingId, id, relation, error);
}

void tinder_destroy(Tinder tinder, Error* error) {
	graph_destroy(tinder.graph, error);
	free(tinder.profiles);
}