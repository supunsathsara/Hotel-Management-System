#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void header()
{
    printf("\t\t\t ABC HOTEL\n");
}

void welcomeMsg()
{
    char any;
    header();
    system("color 03");
    printf("\t\t\t");
    char msg[11] = "WELCOME....";
    for (int i = 0; i < 11; i++)
    {
        printf("%c", msg[i]);
        Sleep(100);
    }
    printf("\n");
    system("color 0f");
    time_t t = time(NULL);
    printf("\n%s", ctime(&t));
    printf("\nEnter any key to continue.....");
    scanf("%c", &any);
    // exit(1);
    system("cls");
}

int loginScrn()
{
    int login = 0;
    header();
    printf("\t\t");
    printf("[1]: Receptionist\n");
    printf("\t\t");
    printf("[2]: Manager\n");
    printf("\t\t");
    printf("[3]: Exit\n");
    printf("Enter your login type: ");
    scanf("%d", &login);

    // condition ? true : false;
    login == 3 ? exit(1) : system("cls");
    login != 1 && login != 2 ? loginScrn() : system("cls");
    return login;
}

void receptionistLogin()
{
    int Rid, fRid;
    char pass[10], fpass[10];
    FILE *db = fopen("db.txt", "r");
    header();
    printf("\n\t\tReceptionist Login\n\n");
    printf("\tEnter Login ID: ");
    scanf("%d", &Rid);
    printf("\n\tEnter Password: ");
    scanf("%s", pass);
    while (!feof(db))
    {
        fscanf(db, "%d:%s", &fRid, &fpass);
        if (Rid == fRid && strcmp(pass, fpass) == 0)
        {
            printf("\n\tLogin Successful\n");
            system("pause");
            system("cls");

            break;
        }
        // else
        // {
        //     printf("\n\tLogin Failed\n");
        //     printf(fpass, fRid);
        //     system("pause");
        //     system("cls");
        //     receptionistLogin();
        // }
    }
    printf("\n\tLogin Failed\n");
    printf(fpass, fRid);
    system("pause");
    system("cls");
    receptionistLogin();
}

int main()
{
    welcomeMsg();
    int loginChoice = loginScrn();
    if (loginChoice == 1)
    {
        receptionistLogin();
    }
    else if (loginChoice == 2)
    {
        printf("\n\t\tManager Login\n\n");
    }
}
