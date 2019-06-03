#include <stdio.h>
#include "tinder.h"
#include "error.h"

/*
    [ TO-DO ]
    -> Retornar usuarios para um vector na main
    -> Mostrar matches ordenadamente para o usuario, como se fosse o TINDER.
    -> Repetir o mesmo processo para manageRequests
    -> Vou dar um mijão
*/

#define ascii1 " _____ _       _            _____         ___         \n"
#define ascii2 "|_   _|_|___ _| |___ ___   |   __|___ ___|  _|___ ___ \n"
#define ascii3 "  | | | |   | . | -_|  _|  |  |  |  _| .'|  _| . |_ -|\n"
#define ascii4 "  |_| |_|_|_|___|___|_|    |_____|_| |__,|_| |___|___|\n"
#define ascii ascii1 ascii2 ascii3 ascii4

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
    const char infoDescription[MAXINFO][MAXSTR] = {"City", "Movie", "Team", "Favorite Color", "Music Band", "Neymar Tweet"};

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
    printf("Press any key to continue.\n");
    getchar();
    getchar();
}

int login(Tinder* tinder) {
    bool hasProfiles = tinder_listProfiles(*tinder);
    if (!hasProfiles)
        return -1;

    printf("Select your profile: ");
    int op; scanf("%d", &op);

    if (op < 0 || op >= tinder->nProfiles) {
        printf("Invalid option.\n");
        printf("Press any key to continue.\n");
        getchar();
        getchar();

        return login(tinder);
    }

    return op;
}

int menuProfile() {
    puts("** Profile **\n");

    puts("1. Add friend");
    puts("2. Friend requests");
    puts("3. See friends");
    puts("4. Perfect match");
    puts("0. Logout");
    
    int op;
    scanf("%d", &op);

    if (op < 0 || op > 4) {
        puts("Invalid option");
        return menuProfile();
    }

    return op;
}

void manageRequests(Tinder* tinder, int id) {
    bool any = tinder_listConnected(*tinder, id, REQUESTED);
    if (!any) {
        return;
    }

    int newId;
    scanf("%d", &newId);

    Error error;
    tinder_acceptRequest(*tinder, id, newId, &error);

    if (error.occurred) {
        printf("Error... %s\n", error.msg);
    } else {
        printf("Accepted!\n");
    }
}

void addFriend(Tinder* tinder, int id) {
    bool any = tinder_listConnected(*tinder, id, UNKNOWN);
    if (!any) {
        return;
    }

    int newId;
    scanf("%d", &newId);

    Error error;
    tinder_addFriend(*tinder, id, newId, &error);

    if (error.occurred) {
        printf("Error... %s\n", error.msg);
    } else {
        printf("Request sent!\n");
    }
}

void profileOptions(Tinder* tinder) {
    int id = login(tinder);
    if (id < 0) {
        return;
    }

    int op = 1;

    Profile matched;
    while(op) {
        op = menuProfile();
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
                    tinder_listConnected(*tinder, id, FRIEND);
                    break;
            case 4:
                    matched = tinder_perfectMatch(*tinder, id);
                    printf("PERFECT MATCH!\n");
                    tinder_printProfile(matched);
                    break;
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