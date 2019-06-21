#include <stdio.h>
#include "tinder.h"
#include "error.h"

#define ascii1 " _____ _____ _____    _____ _____ _____ _____ \n"
#define ascii2 "|   __|  _  |   __|  |   __|  _  |  |  |   __|\n"
#define ascii3 "|   __|     |   __|  |   __|     |    -|   __|\n"
#define ascii4 "|_____|__|__|_____|  |__|  |__|__|__|__|_____|\n"
#define ascii ascii1 ascii2 ascii3 ascii4

#define UNKNOWN 0
#define REQUESTED 1
#define REQUESTING 2
#define FRIEND 3

int cmp(const void* a, const void* b) {
    return ((ProfileMatch*) b)->match - ((ProfileMatch*) a)->match;
}

void pressKey() {
    printf("Press any key to continue.");
    getchar();
    getchar();
}

int menu() {
    puts(COLOR_CYAN ascii COLOR_RESET);
    puts("1. Add user"); 
    puts("2. Login"); // show all profiles
    puts("3. Load from file"); // load from file
    puts("0. Exit\n");

    int op;
    scanf("%d", &op);

    if (op < 0 || op > 3) {
        printf("Invalid option.\n");
        return menu();
    }

    return op;
}

void addUser(Tinder* tinder) {
    const char infoDescription[MAXINFO][MAXSTR] = {"Gender", "Sexuality",  "City", "Movie", "Team", "Favorite Color", "Music Band", "Neymar Tweet"};
    Profile profile;

    printf("Name: ");
    scanf(" %[^\n\r]", profile.name);

    printf("Age: ");
    scanf("%d", &profile.age);

    for (int i = 0; i < MAXINFO; i++) {
        printf("%s: ", infoDescription[i]);
        scanf(" %[^\n\r]", profile.info[i]);
    }

    Error error;
    tinder_addProfile(tinder, profile, &error);

    printf("\nUser \"%s\" created sucessfully!\n", profile.name);
    pressKey();
}

int login(Tinder* tinder) {
    VecProfile vecProfile = tinder_getProfiles(*tinder);
    if (vecProfile.qttProfiles == 0) {
        printf("Please, add a user or load a file to use the application.\n");
        pressKey();
        return -1;
    }

    profile_printVector(vecProfile);

    printf("Select your profile: ");
    int op; scanf("%d", &op);

    if (op < 0 || op >= tinder->vecProfiles.qttProfiles) {
        printf("Invalid option.\n");
        pressKey();

        return login(tinder);
    }
    return op;
}

int menuProfile(Tinder* tinder, int id) {
    Profile profile = tinder->vecProfiles.profiles[id];
    printf("\n** %s's Profile **\n", profile.name);

    puts("1. Add a friend");
    puts("2. See friend requests");
    puts("3. See your friends");
    puts("4. Discover your perfect match");
    puts("5. Find new friends");
    puts("6. Falsiane Detector");
    puts("0. Logout");
    
    int op;
    scanf("%d", &op);

    if (op < 0 || op > 6) {
        puts("Invalid option");
        return menuProfile(tinder, id);
    }

    return op;
}

void manageRequests(Tinder* tinder, int id) {
    VecProfile vecProfile = tinder_getConnected(*tinder, id, REQUESTED);
    if (vecProfile.qttProfiles == 0) {
        printf("You have 0 requests remaining...\n");
        pressKey();
        return;
    }

    printf("\nHere are all your friend requests!\n");

    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        profile_printProfile(vecProfile.profiles[i]);
        int newId = vecProfile.profiles[i].id;
        
        int option;
        printf(COLOR_BLUE "|" COLOR_RESET " [1] " COLOR_GREEN "Accept        " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [2] " COLOR_BRRED "Refuse        " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [3] " COLOR_YELLOW "Ignore by now " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [4] Exit          " COLOR_BLUE "|\n");
        printf(COLOR_RESET "\nChoose: ");
        scanf("%d", &option);

        Error error;

        switch (option) {
            case 1:
                    tinder_acceptRequest(*tinder, id, newId, &error);
                    printf("Request sucessfully accepted.\n");
                    pressKey();
                    break;
            case 2:
                    tinder_refuseRequest(*tinder, id, newId, &error);
                    printf("Request sucessfully denied.\n");
                    pressKey();
                    break;
            case 3:
                    printf("Request sucessfully skipped.\n");
                    pressKey();
                    continue;
                    break;
            case 4:
                    i = vecProfile.qttProfiles;
                    printf("Exiting...\n");
                    pressKey();
        }
    }
}

void addFriend(Tinder* tinder, int id) {
    VecProfile vecProfile = tinder_getConnected(*tinder, id, UNKNOWN);
    if (vecProfile.qttProfiles == 0) {
        return;
    }
    
    profile_printVector(vecProfile);

    int newId;
    printf("Select a user to add: ");
    scanf("%d", &newId);

    if (newId == id) {
        printf("Invalid id.\n");
        pressKey();
        return;
    }

    Error error;
    tinder_addFriend(*tinder, id, newId, &error);

    if (error.occurred) {
        printf("Error... %s\n", error.msg);
    } else {
        printf("Request sent!\n");
    }

    pressKey();
}

void manageMatches(Tinder* tinder, int id) {
    VecProfile vecProfile = tinder_getConnected(*tinder, id, UNKNOWN);

    Profile currentProfile = tinder->vecProfiles.profiles[id];
    ProfileMatch profileMatch[vecProfile.qttProfiles];

    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        profileMatch[i].profile = vecProfile.profiles[i];
        profileMatch[i].match = tinder_getMatch(currentProfile, vecProfile.profiles[i]);
    }

    qsort(profileMatch, vecProfile.qttProfiles, sizeof(ProfileMatch), cmp);

    if (vecProfile.qttProfiles == 0) {
        printf("You already know everyone...\n");
        pressKey();
        return;
    }

    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        profile_printProfile(profileMatch[i].profile);
        printf("Você e %s tem %.2lf de similaridade!\n", profileMatch[i].profile.name, 100* profileMatch[i].match/(double)8);
        int newId = profileMatch[i].profile.id;
        
        int option;
        printf(COLOR_BLUE "|" COLOR_RESET " [1] " COLOR_GREEN "Send request        " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [2] " COLOR_YELLOW "Ignore by now " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [3] Exit          " COLOR_BLUE "|\n");
        printf(COLOR_RESET "\nChoose: ");
        scanf("%d", &option);

        Error error;

        switch (option) {
            case 1:
                    tinder_addFriend(*tinder, id, newId, &error);

                    if (error.occurred) {
                        printf("Error... %s\n", error.msg);
                    } else {
                        printf("Request sent!\n");
                    }

                    pressKey();
                    break;
            case 2:
                    printf("User sucessfully skipped.\n");
                    pressKey();
                    continue;
                    break;
            case 3:
                    i = vecProfile.qttProfiles;
                    printf("Exiting...\n");
                    pressKey();
        }
    }
}

void seeFriends(Tinder* tinder, int id) {
    VecProfile vecProfile = tinder_getConnected(*tinder, id, FRIEND);

    if (vecProfile.qttProfiles == 0) {
        printf("You have no friends in life. =(\n");
        pressKey();
        return;
    }

    printf("\nHere are all your friends!\n");

    profile_printVector(vecProfile);
    pressKey();
}

void detectFakeFriends(Tinder* tinder, int id) {
    Profile currentProfile = tinder->vecProfiles.profiles[id];
    VecProfile vecProfile = tinder_getConnected(*tinder, id, FRIEND);
    
    if (vecProfile.qttProfiles == 0) {
            printf("You have no friends in life. =(\n");
            pressKey();
            return;
        }

    int falsianesCount = 0;

    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        if (tinder_getMatch(currentProfile, vecProfile.profiles[i]) <= 2) {
            falsianesCount++;
        }
    }

    if (falsianesCount == 0) {
        printf("Congratulations! All your friends \"love\" you!\n");
        pressKey();
        return;
    }

    printf("\nFalsianes detected! Watch out!\n");

    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        if (tinder_getMatch(currentProfile, vecProfile.profiles[i]) <= 2) {
            profile_printProfile(vecProfile.profiles[i]);
        }
    }
}

void profileOptions(Tinder* tinder) {
    int id = login(tinder);
    if (id < 0) {
        return;
    }

    int op = 1;

    Profile currentProfile = tinder->vecProfiles.profiles[id];
    Profile matched;

    while(op) {
        op = menuProfile(tinder, id);
        switch(op) {
            case 0:
                    break;
            case 1:
                    addFriend(tinder, id);
                    break;
            case 2:
                    manageRequests(tinder, id);
                    break;
            case 3:
                    seeFriends(tinder, id);
                    break;
            case 4:
                    matched = tinder_perfectMatch(*tinder, id);
                    printf("\nThis is the perfect match for you!\n");
                    int match = tinder_getMatch(currentProfile, matched);
                    profile_printProfile(matched);
                    printf("Você e %s tem %.2lf de similaridade!\n", matched.name, 100*match/(double)8);
                    pressKey();
                    break;
            case 5:
                    manageMatches(tinder, id);
            case 6:
                    detectFakeFriends(tinder, id);
        }
    }    
}

void loadFromFile(Tinder* tinder, char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Error...\n");
        return;
    }

    while(!feof(fp)) {
        Profile profile;

        int flag;

        flag = fscanf(fp, " %[^\n\r]", profile.name);

        if (flag == 0) {
            break;
        }

        fscanf(fp, "%d", &profile.age);
        for (int i = 0; i < MAXINFO; i++) {
            fscanf(fp, " %[^\n\r]", profile.info[i]);
        }
        
        Error error;
        tinder_addProfile(tinder, profile, &error);
    }

    fclose(fp);
}

void saveFile(Tinder tinder, char* filename) {
    printf("Do you want to save your users file?\n[1] Yes\n[2] No\n");
    
    int option;
    scanf("%d", &option);

    if (option == 2) return;

    FILE* fp = fopen(filename, "w");
    VecProfile profiles = tinder.vecProfiles;

    for (int i = 0; i < profiles.qttProfiles; i++) {
        Profile currentProfile = profiles.profiles[i];
        fprintf(fp, "%s\n", currentProfile.name);
        fprintf(fp, "%d\n", currentProfile.age);
        for (int j = 0; j < 8; j++) {
            fprintf(fp, "%s\n", currentProfile.info[j]);
        }

        fprintf(fp, "\n");
    }
}

int main() {
    Error error;
    Tinder tinder = tinder_create(&error);

    char filename[100];
    strcpy(filename, "friends.txt");

    int option = 1;
    while(option) {
        option = menu();

        switch(option) {
            case 0:
                    break;

            case 1:
                    addUser(&tinder);
                    break;

            case 2:
                    profileOptions(&tinder);
                    break;

            default:
                    printf("Digite o nome do arquivo: ");
                    scanf(" %s", filename);
                    loadFromFile(&tinder, filename);
                    break;
        }
    }

    saveFile(tinder, filename);
    tinder_destroy(tinder, &error);
}