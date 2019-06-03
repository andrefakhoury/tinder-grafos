#ifndef PROFILE_H
#define PROFILE_H

#include "error.h"
#include <stdbool.h>
#include <stdlib.h>

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
	Profile* profiles;
	size_t qttProfiles;
} VecProfile;

VecProfile profile_createVector();

void profile_pushVector(VecProfile* vecProfile, Profile prof);

void profile_printProfile(Profile profile);

void profile_printVector(VecProfile vecProfile);

#endif