#include "tinder.h"
#include "profile.h"

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
	tinder.vecProfiles = profile_createVector();

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
	profile.id = graph_addVertex(tinder->graph, error);

	for (int i = 0; i < tinder->vecProfiles.qttProfiles; i++) {
		if (i == profile.id) {
			continue;
		}

		Relation relation;
		relation.match = tinder_getMatch(profile, tinder->vecProfiles.profiles[i]);
		relation.status = UNKNOWN;

		graph_addEdge(tinder->graph, profile.id, i, &relation, error);
		graph_addEdge(tinder->graph, i, profile.id, &relation, error);
	}

	profile_pushVector(&tinder->vecProfiles, profile);
}

VecProfile tinder_getProfiles(Tinder tinder) {
	VecProfile vecProfile;
	vecProfile.qttProfiles = 0;
	vecProfile.profiles = NULL;

	for (int i = 0; i < tinder.vecProfiles.qttProfiles; i++) {
		profile_pushVector(&vecProfile, tinder.vecProfiles.profiles[i]);
	}

	return vecProfile;
}

VecProfile tinder_getConnected(Tinder tinder, int id, Status status) {
	VecProfile vecProfile;
	vecProfile.qttProfiles = 0;
	vecProfile.profiles = NULL;

	for (int i = 0; i < tinder.vecProfiles.qttProfiles; i++) {
		if (i == id) {
			continue;
		}

		Error error;
		Relation* relation = (Relation*)graph_edgeWeight(tinder.graph, id, i, &error);

		if (relation->status == status) {
			profile_pushVector(&vecProfile, tinder.vecProfiles.profiles[i]);
		}
	}

	return vecProfile;
}

Profile tinder_perfectMatch(Tinder tinder, int id) {
	Profile profile;
	int maxMatch = -1;

	Error error;
	for (int i = 0; i < tinder.vecProfiles.qttProfiles; i++) {
		if (i == id) {
			continue;
		}

		int curMatch = ((Relation*)(graph_edgeWeight(tinder.graph, id, i, &error)))->match;
		if (curMatch > maxMatch || (curMatch == maxMatch && rand()&1)) {
			maxMatch = curMatch;
			profile = tinder.vecProfiles.profiles[i];
		}
	}

	return profile;
}

void tinder_addFriend(Tinder tinder, int id, int toAdd, Error* error) {
	error->occurred = false;

	if (id < 0 || toAdd < 0 || id >= tinder.vecProfiles.qttProfiles || toAdd >= tinder.vecProfiles.qttProfiles) {
		error->occurred = true;
		strcpy(error->msg, "Invalid id");
		return;
	}

	Relation* relation = (Relation*)graph_edgeWeight(tinder.graph, id, toAdd, error);
	if (error->occurred || relation->status == FRIEND || relation->status == REQUESTED) {
		error->occurred = true;
		sprintf(error->msg, "Invalid friend request.. %ld", tinder.vecProfiles.qttProfiles);
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
	
	if (id < 0 || requestingId < 0 || id >= tinder.vecProfiles.qttProfiles || requestingId >= tinder.vecProfiles.qttProfiles) {
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
	free(tinder.vecProfiles.profiles);
}