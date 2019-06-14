#ifndef PROFILE_H
#define PROFILE_H

#include "error.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 256
#define MAXINFO 6

//macros for colors
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"
#define COLOR_BRRED   "\x1b[91m"
#define COLOR_BRWHITE "\x1b[97m"
#define COLOR_RESET   "\x1b[0m"

#define BG_RED     "\x1b[41m"
#define BG_GREEN   "\x1b[42m"
#define BG_YELLOW  "\x1b[43m"
#define BG_BLUE    "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN    "\x1b[46m"
#define BG_WHITE   "\x1b[47m"

#define UNKNOWN 0
#define REQUESTED 1
#define REQUESTING 2
#define FRIEND 3

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

Profile profile_getRandom();

#endif