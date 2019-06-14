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

Profile profile_getRandom() {
    Profile profile;
    strcpy(profile.name, "AAAAAAAAAAAAAAQAaaaaaaaaaaaaaaaaaaAdemastor AAAAAAAAAAAAAAA");
    profile.age = rand()%210;
    profile.id = 2;
    for (int i = 0; i < MAXINFO; i++) {
        strcpy(profile.info[i], "");
        sprintf(profile.info[i], "AAAAA%d", rand()%100);
    }
    return profile;
}

void profile_printProfile(Profile profile) {
	const char infoDescription[MAXINFO][MAXSTR] = {"City", "Movie", "Team", "Favorite Color", "Music Band", "Neymar Tweet"};

    printf(COLOR_BLUE "+==============================================================+\n" COLOR_RESET);
    int len = (60 + strlen(profile.name))/2;
    printf(COLOR_BLUE "|" COLOR_RESET " %*s %-*s " COLOR_BLUE "|\n" COLOR_RESET, len, profile.name, 59 - len, "");
    printf(COLOR_BLUE "|" COLOR_RESET " %d " COLOR_CYAN "years old%-*s" COLOR_BLUE "|\n" COLOR_RESET, profile.age, (int)(51 - log10(profile.age%10 && profile.age != 1 ? profile.age : profile.age+1)), "");
    printf(COLOR_BLUE "+--------------------------------------------------------------+\n" COLOR_RESET);

	for (int i = 0; i < MAXINFO; i++) {
        printf(COLOR_BLUE "|" COLOR_CYAN " %s: " COLOR_RESET "%-*s " COLOR_BLUE "|\n" COLOR_RESET, infoDescription[i], (int)(58 - strlen(infoDescription[i])), profile.info[i]);
	}
    printf(COLOR_BLUE "+==============================================================+\n" COLOR_RESET);
    // printf(COLOR_BLUE "|" COLOR_RESET " [1] " COLOR_GREEN "Accept        " COLOR_BLUE "|\n");
    // printf("|" COLOR_RESET " [2] " COLOR_BRRED "Refuse        " COLOR_BLUE "|\n");
    // printf("|" COLOR_RESET " [3] " COLOR_YELLOW "Ignore by now " COLOR_BLUE "|\n");
    // printf("|" COLOR_RESET " [4] Exit          " COLOR_BLUE "|\n");
    // printf(COLOR_RESET "Choose: \n");
}

void profile_printVector(VecProfile vecProfile) {
    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        profile_printProfile(vecProfile.profiles[i]);
    }
}