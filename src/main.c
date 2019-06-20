#include <stdio.h>
#include "tinder.h"
#include "error.h"

/*
    [ TO-DO ]
    -> Retornar usuarios para um vector na main
    -> Mostrar matches ordenadamente para o usuario, como se fosse o TINDER.
    -> Repetir o mesmo processo para manageRequests
*/

#define ascii1 " _____ _       _            _____         ___         \n"
#define ascii2 "|_   _|_|___ _| |___ ___   |   __|___ ___|  _|___ ___ \n"
#define ascii3 "  | | | |   | . | -_|  _|  |  |  |  _| .'|  _| . |_ -|\n"
#define ascii4 "  |_| |_|_|_|___|___|_|    |_____|_| |__,|_| |___|___|\n"
#define ascii ascii1 ascii2 ascii3 ascii4

#define UNKNOWN 0
#define REQUESTED 1
#define REQUESTING 2
#define FRIEND 3

int cmp(const void* a, const void* b) {
    return ((ProfileMatch*) b)->match - ((ProfileMatch*) a)->match;
}

void pressKey() {
    printf("Press any key to continue.\n");
    getchar();
    getchar();
}

int menu() {
    puts(ascii);
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
    puts("0. Logout");
    
    int op;
    scanf("%d", &op);

    if (op < 0 || op > 5) {
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

    for (int i = 0; i < vecProfile.qttProfiles; i++) {
        profile_printProfile(vecProfile.profiles[i]);
        int newId = vecProfile.profiles[i].id;
        
        int option;
        printf(COLOR_BLUE "|" COLOR_RESET " [1] " COLOR_GREEN "Accept        " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [2] " COLOR_BRRED "Refuse        " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [3] " COLOR_YELLOW "Ignore by now " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [4] Exit          " COLOR_BLUE "|\n");
        printf(COLOR_RESET "Choose: \n");
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
        printf("Você e %s tem %.2lf de similaridade!\n", profileMatch[i].profile.name, 100* profileMatch[i].match/(double)11);
        int newId = profileMatch[i].profile.id;
        
        int option;
        printf(COLOR_BLUE "|" COLOR_RESET " [1] " COLOR_GREEN "Send request        " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [2] " COLOR_YELLOW "Ignore by now " COLOR_BLUE "|\n");
        printf("|" COLOR_RESET " [3] Exit          " COLOR_BLUE "|\n");
        printf(COLOR_RESET "Choose: \n");
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
        printf("You have 0 friends.\n");
        pressKey();
        return;
    }

    profile_printVector(vecProfile);
    pressKey();
}

void profileOptions(Tinder* tinder) {
    int id = login(tinder);
    if (id < 0) {
        return;
    }

    int op = 1;

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
                    printf("PERFECT MATCH!\n");
                    profile_printProfile(matched);
                    break;
            case 5:
                    manageMatches(tinder, id);
        }
    }    
}

void loadFromFile(Tinder* tinder) {
    printf("Digite o nome do arquivo: ");
    char fileName[MAXSTR];

    scanf(" %s", fileName);

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

int main() {
    Error error;
    Tinder tinder = tinder_create(&error);

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
                    loadFromFile(&tinder);
                    break;
        }
    }

    tinder_destroy(tinder, &error);
}