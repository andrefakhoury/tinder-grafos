#include "profile.h"
#include <stdio.h>
#include <math.h>

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
	const char infoDescription[MAXINFO][MAXSTR] = {"Gender", "Attracted by",  "City", "Movie", "Team", "Favorite Color", "Music Band", "Neymar Tweet"};

    printf(COLOR_BLUE "+==============================================================+\n" COLOR_RESET);
    int len = (60 + strlen(profile.name))/2;
    
    printf(COLOR_BLUE "|" COLOR_RESET " %*s %-*s " COLOR_BLUE "|\n" COLOR_RESET, len, profile.name, 59 - len, "");
    printf(COLOR_BLUE "|" COLOR_RESET " %d " COLOR_CYAN "years old%-*s" COLOR_BLUE "|\n" COLOR_RESET, profile.age, (int)(51 - log10(profile.age%10 && profile.age != 1 ? profile.age : profile.age+1)), "");
    printf(COLOR_BLUE "+--------------------------------------------------------------+\n" COLOR_RESET);

	for (int i = 0; i < MAXINFO; i++) {
        printf(COLOR_BLUE "|" COLOR_CYAN " %s: " COLOR_RESET "%-*s " COLOR_BLUE "|\n" COLOR_RESET, infoDescription[i], (int)(58 - strlen(infoDescription[i])), profile.info[i]);
	}
    printf(COLOR_BLUE "+==============================================================+\n" COLOR_RESET);
}

void profile_printVector(VecProfile vecProfile) {
    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        profile_printProfile(vecProfile.profiles[i]);
    }
}