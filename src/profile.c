#include "profile.h"
#include <stdio.h>

VecProfile profile_createVector() {
    VecProfile vecProfile;
    vecProfile.profiles = NULL;
    vecProfile.qttProfiles = 0;
    return vecProfile;
}

void profile_pushVector(VecProfile* vecProfile, Profile prof) {
    vecProfile->qttProfiles++;
    vecProfile->profiles = realloc(vecProfile->profiles, vecProfile->qttProfiles * sizeof(Profile));
    vecProfile->profiles[vecProfile->qttProfiles - 1] = prof;
}

void profile_printProfile(Profile profile) {
	const char infoDescription[MAXINFO][MAXSTR] = {"City", "Movie", "Team", "Favorite Color", "Music Band", "Neymar Tweet"};
	printf("Profile - %s [%d]\n", profile.name, profile.id);
	printf("Age: %d\n", profile.age);
	for (int i = 0; i < MAXINFO; i++) {
		printf("%s: %s\n", infoDescription[i], profile.info[i]);
	}
}

void profile_printVector(VecProfile vecProfile) {
    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        profile_printProfile(vecProfile.profiles[i]);
    }
}