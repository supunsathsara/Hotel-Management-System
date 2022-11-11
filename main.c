#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

// region Global Variables
struct details
{
    // char nic[10];
    char nic[13];
    char name[50];
    char address[50];
    char phone[14];
    char email[40];
    char room[10];
} customer, fcustomer;
char *suites[3] = {"Precedential Suite", "Royal Suite", "Family Suite"};
float suitePrices[3] = {10999, 8499, 5999}; // Precedential Suite, Royal Suite, Family Suite

// dining vars
struct diningCount
{
    int breakfast;
    int lunch;
    int dinner;
    int drinks;
} diningCount;

char breakfastMenu[100][100];
float *breakfastPrice;
char lunchMenu[100][100];
float *lunchPrice;
char dinnerMenu[100][100];
float *dinnerPrice;
char drinksMenu[100][100];
float *drinksPrice;

// Endregion

#define BILL_CMD "cd /D \"C:\\Windows\\System32\" & rundll32.exe mshtml.dll,PrintHTML \"F:\\Dev\\C\\Hotel Managment System\\bill.txt\""

int roomBooking();

void initMenu()
{
    struct menu
    {
        char type[20];
        char name[20];
        float price;
    } menu;
    diningCount.breakfast = 0, diningCount.lunch = 0, diningCount.dinner = 0, diningCount.drinks = 0;
    FILE *fp;

    // init menu price array
    breakfastPrice = (float *)malloc(diningCount.breakfast * sizeof(float));
    lunchPrice = (float *)malloc(diningCount.lunch * sizeof(float));
    dinnerPrice = (float *)malloc(diningCount.dinner * sizeof(float));
    drinksPrice = (float *)malloc(diningCount.dinner * sizeof(float));

    // add init values
    strncpy(breakfastMenu[0], "breakfast", 10);
    breakfastPrice[0] = 0;
    strncpy(lunchMenu[0], "lunch", 10);
    lunchPrice[0] = 0;
    strncpy(dinnerMenu[0], "dinner", 10);
    dinnerPrice[0] = 0;
    strncpy(drinksMenu[0], "drinks", 10);
    drinksPrice[0] = 0;

    fp = fopen("menu.csv", "r");
    while (!feof(fp))
    {
        fscanf(fp, "%[^,],%[^,],%f\n)", menu.type, menu.name, &menu.price);
        if (strcmp(menu.type, "breakfast") == 0)
        {
            diningCount.breakfast++;
            breakfastPrice = (float *)realloc(breakfastPrice, (diningCount.breakfast + 1) * sizeof(float));
            strcpy(breakfastMenu[diningCount.breakfast], menu.name);
            breakfastPrice[diningCount.breakfast] = menu.price;
        }

        else if (strcmp(menu.type, "lunch") == 0)
        {
            diningCount.lunch++;
            lunchPrice = (float *)realloc(lunchPrice, (diningCount.lunch + 1) * sizeof(float));
            strcpy(lunchMenu[diningCount.lunch], menu.name);
            lunchPrice[diningCount.lunch] = menu.price;
        }
        else if (strcmp(menu.type, "dinner") == 0)
        {
            diningCount.dinner++;
            dinnerPrice = (float *)realloc(dinnerPrice, (diningCount.dinner + 1) * sizeof(float));
            strcpy(dinnerMenu[diningCount.dinner], menu.name);
            dinnerPrice[diningCount.dinner] = menu.price;
        }
        else if (strcmp(menu.type, "drinks") == 0)
        {
            diningCount.drinks++;
            drinksPrice = (float *)realloc(drinksPrice, (diningCount.drinks + 1) * sizeof(float));
            strcpy(drinksMenu[diningCount.drinks], menu.name);
            drinksPrice[diningCount.drinks] = menu.price;
        }
    }
    fclose(fp);
}

void header()
{
    printf("\n\t\t\t ABC HOTEL\n");
}

void welcomeMsg()
{
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
    system("pause");
    // system("cls");
}

void saveLog(float price, int type)
{
    FILE *log;
    char *transactionType;
    time_t t = time(NULL);
    char *time;
    log = fopen("log.csv", "a");
    if (log == NULL)
    {
        printf("Error in opening the log\n");
        printf("didn't Update the transaction...\n");
        system("pause");
        return;
    }
    time = strtok(ctime(&t), "\n");
    transactionType = (type == 1) ? "Booking" : "Dining";
    fprintf(log, "%s,%s,%.2f\n", time, transactionType, price);
    fclose(log);
}

int loginScrn()
{
    system("cls");
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

    if (login != 1 && login != 2)
    {
        printf("Invalid login type.\n");
        printf("Try Again...");
        Sleep(300);
        system("cls");
        loginScrn();
    }
    else
    {
        return login;
    }
}

int managerLogin()
{
    struct logins
    {
        char id[11];
        char pass[11];
    } manager, fmanager;
    int LoginSuccess = 0; // 0 = false, 1 = true
    FILE *db = fopen("logins.env", "r");
    header();
    printf("\n\t\tManager Login\n\n");
    strcpy(manager.id, "00000");
    printf("\tEnter 0 to go back\n\n");
    printf("\n\tEnter Password: ");

    scanf("%s", &manager.pass);
    if (strcmp(manager.pass, "0") == 0)
    {
        return 0;
    }
    while (!feof(db))
    {
        fscanf(db, "%10[^=]=%10[^\n]\n", &fmanager.id, &fmanager.pass);
        if (strcmp(manager.id, fmanager.id) == 0 && strcmp(manager.pass, fmanager.pass) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int managerScrn()
{
    system("cls");
    header();
    int choice;

    printf("\t\t[1]: View Income report\n");
    printf("\t\t[2]: Update Logins\n");
    printf("\t\t[3]: View Room Bookings\n");
    printf("\t\t[4]: Customer Update\n");
    printf("\t\t[5]: Menu Update\n");
    printf("\t\t[6]: Exit\n");
    printf("\t\tEnter your choice: ");
    scanf("%d", &choice);
    return choice;
}

float getIncome(int month, int year)
{
    system("cls");
    float income = 0;
    float amount;
    char transactionType[12];
    struct date
    {
        char day[4];
        char month[4];
        char time[9];
        int year;
        int date;
    } date;
    char *months[13] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    FILE *log;
    log = fopen("log.csv", "r");
    if (log == NULL)
    {
        printf("Error in opening the log\n");
        system("pause");
        return 0;
    }
    printf("\n\n\t Date\t\t Type\t\t Amount\n");
    printf("\t ----\t\t -----\t\t ------\n\n");
    if (month == 0)
    {
        while (!feof(log))
        {
            fscanf(log, "%3[^ ] %3[^ ] %d %8[^ ] %d,%10[^,],%f\n", &date.day, &date.month, &date.date, &date.time, &date.year, &transactionType, &amount);
            fflush(stdin);
            if (date.year == year)
            {
                printf("\t%d/%s/%d %s %.2f\n", date.date, date.month, date.year, transactionType, amount);
                income += amount;
            }
        }
    }
    else
    {
        while (!feof(log))
        {
            fscanf(log, "%3[^ ] %3[^ ] %d %8[^ ] %d,%10[^,],%f\n", &date.day, &date.month, &date.date, &date.time, &date.year, &transactionType, &amount);
            fflush(stdin);
            if (date.year == year && strcmp(date.month, months[month]) == 0)
            {
                printf("\t%d/%s/%d \t %s \t %.2f\n", date.date, date.month, date.year, transactionType, amount);
                income += amount;
            }
        }
    }
    fclose(log);
    return income;
}

void viewIncome()
{
    system("cls");
    float total;
    int choice;
    // date
    time_t s;
    struct tm *current_time;
    s = time(NULL);
    current_time = localtime(&s);
    int year = current_time->tm_year + 1900;
    int month = current_time->tm_mon + 1;

checkIncome:
    printf("\n\t\t\tIncome Report\n\n");
    printf("\t[1]: This month\n");
    printf("\t[2]: Last month\n");
    printf("\t[3]: This year\n");
    printf("\t[4]: custom month & year\n");

    printf("\n\tEnter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        total = getIncome(month, year);
        break;
    case 2:
        total = getIncome(month - 1, year);
        break;
    case 3:
        total = getIncome(0, year);
        break;
    case 4:
        printf("Enter year: ");
        scanf("%d", &year);
        printf("Enter month (0 to check all): ");
        scanf("%d", &month);
        if (month >= 0 && month <= 12)
        {
            total = getIncome(month, year);
        }
        else
        {
            printf("invalid month");
            system("pause");
            goto checkIncome;
        }
        break;
    default:
        printf("Invalid choice.\n");
        goto checkIncome;
        break;
    }
    if (total == 0.00)
    {
        printf("\n\t\t--- No data found. ---\n\n");
    }
    else
    {
        printf("\n\tTotal income: %.2f\n\n", total);
    }
    system("pause");
}

void updateLogins()
{
    int choice;
    FILE *logins;
    FILE *temp;
    struct logins
    {
        char id[11];
        char pass[11];
    } user, fuser;
    logins = fopen("logins.env", "a+");
updateLogins:
    system("cls");
    printf("\t\tUpdate Logins\n");
    printf("\t[1]: Add a receptionist\n");
    printf("\t[2]: Delete a receptionist\n");
    printf("\t[3]: Change a receptionist password\n");
    printf("\t[4]: Change Manager password\n");
    printf("\t[5]: go back\n");

    printf("\t => ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        printf("\n\tEnter new receptionist id: ");
        scanf("%s", &user.id);
        printf("\tEnter new receptionist password: ");
        scanf("%s", &user.pass);
        fprintf(logins, "%s=%s\n", user.id, user.pass);
        fclose(logins);
        break;
    case 2:
        temp = fopen("tempLogins.env", "a+");
        printf("\n\tEnter receptionist id: ");
        scanf("%s", &user.id);
        while (!feof(logins))
        {
            fscanf(logins, "%10[^=]=%10[^\n]\n", &fuser.id, &fuser.pass);
            if (strcmp(fuser.id, user.id) == 0)
            {
                continue;
            }
            else
            {
                fprintf(temp, "%s=%s\n", fuser.id, fuser.pass);
            }
        }
        fclose(logins);
        fclose(temp);
        int status = remove("logins.env");
        if (status != 0)
        {
            printf("\n\tcouldn't update the database...\n");
        }
        rename("tempLogins.env", "logins.env");
        printf("Success");
        system("cls");
        break;
    case 3:
        temp = fopen("tempLogins.env", "a+");
        printf("\n\tEnter receptionist id: ");
        scanf("%s", &user.id);
        printf("\tEnter new receptionist password: ");
        scanf("%s", &user.pass);
        while (!feof(logins))
        {
            fscanf(logins, "%10[^=]=%10[^\n]\n", &fuser.id, &fuser.pass);
            if (strcmp(fuser.id, user.id) == 0)
            {
                fprintf(temp, "%s=%s\n", user.id, user.pass);
            }
            else
            {
                fprintf(temp, "%s=%s\n", fuser.id, fuser.pass);
            }
        }
        fclose(logins);
        fclose(temp);
        Sleep(200);
        remove("logins.env");
        rename("tempLogins.env", "logins.env");
        printf("Success");
        system("pause");
        // system("cls");
        break;
    case 4:
        temp = fopen("tempLogins.env", "a+");
        printf("Enter new password: ");
        scanf("%s", user.pass);
        strcpy(user.id, "00000");
        while (!feof(logins))
        {
            fscanf(logins, "%10[^=]=%10[^\n]\n", &fuser.id, &fuser.pass);
            if (strcmp(fuser.id, user.id) == 0)
            {
                fprintf(temp, "%s=%s\n", user.id, user.pass);
            }
            else
            {
                fprintf(temp, "%s=%s\n", fuser.id, fuser.pass);
            }
        }
        fclose(temp);
        fclose(logins);
        Sleep(200);
        remove("logins.env");
        rename("tempLogins.env", "logins.env");
        printf("Success");
        system("cls");
        break;
    case 5:
        system("cls");
        return;
    default:
        printf("Invalid choice");
        system("cls");
        goto updateLogins;
        break;
    }
}

void viewRoomBookings()
{
    FILE *bookingDB = fopen("bookingDB.csv", "a+");
    int choice;
    struct bookingData
    {
        char nic[20];
        char suite[25];
        int days;
        float price;
        float total;
    } fbooking;
    while (feof(bookingDB) == 0)
    {
        fscanf(bookingDB, "%12[^,],%24[^,],%d,%f,%f\n", &fbooking.nic, &fbooking.suite, &fbooking.days, &fbooking.price, &fbooking.total);
        fflush(stdin);
        printf("\t%s\t%s\t%d\t%.2f\t%.2f\n", fbooking.nic, fbooking.suite, fbooking.days, fbooking.price, fbooking.total);
    }
    system("pause");
    fclose(bookingDB);
    system("cls");
}

void viewFullMenu()
{
    printf("\t\tBreakfast Menu\n");
    for (int i = 1; i <= diningCount.breakfast; i++)
    {
        printf("\t%d. %s \t\t %.2f\n", i, breakfastMenu[i], breakfastPrice[i]);
    }
    printf("\t\tLunch Menu\n");
    for (int i = 1; i <= diningCount.lunch; i++)
    {
        printf("\t%d. %s \t\t %.2f\n", i, lunchMenu[i], lunchPrice[i]);
    }
    printf("\t\tDinner Menu\n");
    for (int i = 1; i <= diningCount.dinner; i++)
    {
        printf("\t%d. %s \t\t %.2f\n", i, dinnerMenu[i], dinnerPrice[i]);
    }
    printf("\t\tDrinks Menu\n");
    for (int i = 1; i <= diningCount.drinks; i++)
    {
        printf("\t%d. %s \t\t %.2f\n", i, drinksMenu[i], drinksPrice[i]);
    }
}

void resetMenu()
{
    // reset menu counts
    diningCount.breakfast = 0;
    diningCount.lunch = 0;
    diningCount.dinner = 0;
    diningCount.drinks = 0;

    // initialize menu arrays
    initMenu();
}
void updateMenu()
{
    int choice;
    FILE *menu;
    FILE *temp;
    struct menu
    {
        char type[20];
        char item[25];
        float price;
    } fmenu, menuData;
updateMenu:
    system("cls");
    resetMenu();
    // viewFullMenu();
    system("pause");
    printf("\t\tUpdate Menu\n");
    printf("\t[1]: Add a new item\n");
    printf("\t[2]: Delete an item\n");
    printf("\t[3]: Change an item price\n");
    printf("\t[4]: go back\n");
    printf("\t\t =>");
    scanf("%d", &choice);
    system("cls");
    switch (choice)
    {
    case 1:
        // handle adding a new item
        menu = fopen("menu.csv", "a+");
        printf("\tSelect item type:");
        printf("\n\t[1]: Breakfast");
        printf("\n\t[2]: Lunch");
        printf("\n\t[3]: Dinner");
        printf("\n\t[4]: Drinks");
        printf("\n\t\t =>");
        scanf("%d", &choice);
        if (choice == 1)
        {
            strcpy(menuData.type, "breakfast");
        }
        else if (choice == 2)
        {
            strcpy(menuData.type, "lunch");
        }
        else if (choice == 3)
        {
            strcpy(menuData.type, "dinner");
        }
        else if (choice == 4)
        {
            strcpy(menuData.type, "drinks");
        }
        else
        {
            printf("Invalid choice");
            system("cls");
            goto updateMenu;
        }
        printf("\tEnter item name: ");
        scanf("%s", &menuData.item);

        // check if item already exists
        while (feof(menu) == 0)
        {
            fscanf(menu, "%19[^,],%24[^,],%f\n", &fmenu.type, &fmenu.item, &fmenu.price);
            if (strcmp(menuData.item, fmenu.item) == 0 && strcmp(menuData.type, fmenu.type) == 0)
            {
                printf("Item already exists\n");
                printf("\t%s\t%s\t%.2f\n", fmenu.type, fmenu.item, fmenu.price);
                system("pause");
                fclose(menu);
                system("cls");
                goto updateMenu;
            }
        }

        printf("\tEnter item price: ");
        scanf("%f", &menuData.price);

        fprintf(menu, "%s,%s,%.2f\n", menuData.type, menuData.item, menuData.price);
        fclose(menu);
        printf("\nSuccessfully Added the item\n");
        system("pause");
        break;
    case 2:
        // handle delete
        viewFullMenu();
        menu = fopen("menu.csv", "a+");
        temp = fopen("tempMenu.csv", "a+");
        printf("\tEnter item name: ");
        scanf("%s", &menuData.item);
        while (!feof(menu))
        {
            fscanf(menu, "%20[^,],%20[^,],%f\n", &fmenu.type, &fmenu.item, &fmenu.price);
            if (strcmp(fmenu.item, menuData.item) == 0)
            {
                printf("\n%s => %s - %.2f\n", fmenu.type, fmenu.item, fmenu.price);
                printf("Do you want to delete this item? [y/n] ");
                char choice;
                scanf("%s", &choice);
                if (choice == 'y')
                {
                    continue;
                }
                else
                {
                    fprintf(temp, "%s,%s,%.2f\n", fmenu.type, fmenu.item, fmenu.price);
                }
            }
            else
            {
                fprintf(temp, "%s,%s,%.2f\n", fmenu.type, fmenu.item, fmenu.price);
            }
        }
        fclose(menu);
        fclose(temp);
        Sleep(500);
        remove("menu.csv");
        rename("tempMenu.csv", "menu.csv");
        printf("\n\nSuccessfully deleted the item\n\n");
        system("pause");
        system("cls");
        break;
    case 3:
        // handle menu update
        menu = fopen("menu.csv", "a+");
        temp = fopen("tempMenu.csv", "a+");
        // set seek to start of file
        fseek(menu, 0, SEEK_SET);
        printf("\tEnter item name: ");
        scanf("%s", &menuData.item);
        printf("\tEnter new price: ");
        scanf("%f", &menuData.price);
        while (!feof(menu))
        {
            fscanf(menu, "%20[^,],%20[^,],%f\n", &fmenu.type, &fmenu.item, &fmenu.price);
            if (strcmp(fmenu.item, menuData.item) == 0)
            {
                printf("\n%s => %s - %.2f\n", fmenu.type, fmenu.item, fmenu.price);
                printf("Do you want to update this item? [y/n] ");
                char choice;
                scanf(" %c", &choice);
                if (choice == 'y')
                {
                    fprintf(temp, "%s,%s,%.2f\n", fmenu.type, fmenu.item, menuData.price);
                }
                else
                {
                    fprintf(temp, "%s,%s,%.2f\n", fmenu.type, fmenu.item, fmenu.price);
                }
            }
            else
            {
                fprintf(temp, "%s,%s,%.2f\n", fmenu.type, fmenu.item, fmenu.price);
            }
        }
        fclose(menu);
        fclose(temp);
        Sleep(200);
        remove("menu.csv");
        rename("tempMenu.csv", "menu.csv");
        printf("\n\nSuccessfully updated the item\n\n");
        system("pause");
        system("cls");
        break;
    case 4:
        // handle go back
        system("cls");
        return;
        break;
    default:
        printf("Invalid choice");
        system("cls");
        goto updateMenu;
        break;
    }
}

int receptionistLogin()
{
    struct logins
    {
        char id[11];
        char pass[11];
    } receptionist, freceptionist;
    int LoginSuccess = 0; // 0 = false, 1 = true
    FILE *db = fopen("logins.env", "r");
    header();
    printf("\n\t\tReceptionist Login\n\n");
    printf("\tEnter 0 to go back\n\n");
    printf("\tEnter Login ID: ");
    scanf("%s", &receptionist.id);
    if (strcmp(receptionist.id, "0") == 0)
    {
        return 0;
    }
    printf("\n\tEnter Password: ");
    scanf("%s", &receptionist.pass);
    while (!feof(db))
    {
        fscanf(db, "%10[^=]=%10[^\n]\n", &freceptionist.id, &freceptionist.pass);
        if (strcmp(receptionist.id, freceptionist.id) == 0 && strcmp(receptionist.pass, freceptionist.pass) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int receptionistScrn()
{
    system("cls");
    header();
    int choice;

    printf("\t\t[1]: Check In\n");
    printf("\t\t[2]: Check Out\n");
    printf("\t\t[3]: Dining\n");
    printf("\t\t[4]: Update\n");
    printf("\t\t[5]: Exit\n");
    printf("\t\tEnter your choice: ");
    scanf("%d", &choice);
    return choice;
}

void customerUpdate()
{
    printf("\n\tEnter new details:\n");
    printf("\tName: ");
    scanf("%[^\n]%*c", &customer.name);
    printf("\tAddress: ");
    scanf("%[^\n]%*c", &customer.address);
    printf("\tPhone: ");
    scanf("%s", &customer.phone);
    printf("\tEmail: ");
    scanf("%s", &customer.email);
    FILE *customerDB = fopen("customerDB.csv", "r");
    FILE *temp;
    int line = 0;
    temp = fopen("temp.csv", "a");
    while (feof(customerDB) == 0)
    {
        fscanf(customerDB, "%12[^,],%49[^,],%49[^,],%13[^,],%39[^\n]\n", &fcustomer.nic, &fcustomer.name, &fcustomer.address, &fcustomer.phone, &fcustomer.email);
        fflush(stdin);
        if (strcmp(customer.nic, fcustomer.nic) == 0)
        {
            fprintf(temp, "%s,%s,%s,%s,%s\n", customer.nic, customer.name, customer.address, customer.phone, customer.email);
        }
        else
        {
            fprintf(temp, "%s,%s,%s,%s,%s\n", fcustomer.nic, fcustomer.name, fcustomer.address, fcustomer.phone, fcustomer.email);
        }
    }
    fclose(customerDB);
    fclose(temp);
    Sleep(200);
    remove("customerDB.csv");
    rename("temp.csv", "customerDB.csv");
}

void checkIn()
{

    FILE *customerDB = fopen("customerDB.csv", "a+");

    printf("\t\tCheck In\n\n");
    printf("\tEnter customer NIC or passport: ");
    scanf("%s", &customer.nic);
    // check customer is already booked
    FILE *bookingDB = fopen("bookingDB.csv", "r");
    struct bookingData
    {
        char nic[20];
        char suite[25];
        int days;
        float price;
        float total;
    } booking, fbooking;
    while (feof(bookingDB) == 0)
    {
        fscanf(bookingDB, "%12[^,],%24[^,],%d,%f,%f\n", &fbooking.nic, &fbooking.suite, &fbooking.days, &fbooking.price, &fbooking.total);
        fflush(stdin);
        if (strcmp(customer.nic, fbooking.nic) == 0)
        {
            printf("\n\tCustomer already booked\n");
            system("pause");
            fclose(bookingDB);
            return;
        }
    }
    // read to check if customer is already in the database
    while (feof(customerDB) == 0)
    {
        fscanf(customerDB, "%12[^,],%49[^,],%49[^,],%13[^,],%39[^\n]\n", &fcustomer.nic, &fcustomer.name, &fcustomer.address, &fcustomer.phone, &fcustomer.email);
        fflush(stdin);
        if (strcmp(customer.nic, fcustomer.nic) == 0)
        {
            printf("\n\tCustomer is already in the database.\n");
            printf("\tName: %s\n", fcustomer.name);
            printf("\tAddress: %s\n", fcustomer.address);
            printf("\tPhone: %s\n", fcustomer.phone);
            printf("\tEmail: %s\n", fcustomer.email);
            printf("\n\tDo you want to update the details? [Y/N]: ");
            char choice;
            scanf("%c", &choice);
            if (choice == 'Y' || choice == 'y')
            {
                fclose(customerDB);
                customerUpdate();
                goto Addpkg;
            }
            else
            {
                printf("\n\tCustomer details not updated.\n");
                fclose(customerDB);
            }
            goto Addpkg;
            break;
        }
    }
    printf("\n\tCustomer is not in the database.\n");
    printf("\tEnter customer details:\n");
    printf("\tName: ");
    scanf("%[^\n]%*c", &customer.name);
    printf("\tAddress: ");
    scanf("%[^\n]%*c", &customer.address);
    printf("\tPhone: ");
    scanf("%s", &customer.phone);
    printf("\tEmail: ");
    scanf("%s", &customer.email);
    fprintf(customerDB, "%s,%s,%s,%s,%s\n", customer.nic, customer.name, customer.address, customer.phone, customer.email);
    fclose(customerDB);

Addpkg:
    system("cls");
    roomBooking();
}

int roomBooking()
{
    char choice;
    int suiteChoice;
roomBooking:
    system("cls");
    header();
    printf("\t\tRoom Booking\n\n");
    printf("\t     Suite Type \t\t Price \t\t\t Capacity\n");
    printf("\t[1]: Presidential suite \t %.2f /night \t 02\n", suitePrices[0]);
    printf("\t[2]: Royal suite \t\t %.2f /night \t 03\n", suitePrices[1]);
    printf("\t[3]: Family suite \t\t %.2f /night \t 05\n", suitePrices[2]);
    fflush(stdin);
    printf("\n\tchoice: ");
    scanf("%d", &suiteChoice);
    switch (suiteChoice)
    {
    case 1:
        printf("\n\t\tPresidential suite.\n");
        printf("\tPrice: %.2f\n", suitePrices[0]);
        printf("\tCapacity: 02\n\n");

        printf("\tDo you want to book this suite? [Y/N]: ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("\n\t%s Choosen\n", suites[suiteChoice - 1]);
        }
        else
        {
            goto roomBooking;
        }
        break;

    case 2:
        printf("\n\t\tRoyal suite.\n");
        printf("\tPrice: %.2f\n", suitePrices[1]);
        printf("\tCapacity: 03\n\n");

        printf("\tDo you want to book this suite? [Y/N]: ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("\n\t%s Choosen\n", suites[suiteChoice - 1]);
        }
        else
        {
            goto roomBooking;
        }
        break;
    case 3:
        printf("\n\t\tFamily suite.\n");
        printf("\tPrice: %.2f\n", suitePrices[2]);
        printf("\tCapacity: 05\n\n");

        printf("\tDo you want to book this suite? [Y/N]: ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("\n\t%s Choosen\n", suites[suiteChoice - 1]);
        }
        else
        {
            goto roomBooking;
        }
        break;
    default:
        printf("\n\tInvalid choice.\n");
        goto roomBooking;
    }

    int days;
    FILE *bookingDB = fopen("bookingDB.csv", "a");
    printf("How long do you wish to stay: ");
    scanf("%d", &days);
    fprintf(bookingDB, "%s,%s,%d,%.2f,%.2f\n", customer.nic, suites[suiteChoice - 1], days, suitePrices[suiteChoice - 1], suitePrices[suiteChoice - 1] * days);
    fclose(bookingDB);
    printf("\n\tBooking successful.\n");
}

void bookingUpdate()
{
    FILE *bookingDB = fopen("bookingDB.csv", "a+");
    FILE *tempDB = fopen("tempDB.csv", "w");
    int choice;
    struct bookingData
    {
        char nic[20];
        char suite[25];
        int days;
        float price;
        float total;
    } booking, fbooking;
    printf("\n\t\tUpdate booking details\n");
    printf("\tEnter NIC or Passport Number: ");
    scanf("%s", &booking.nic);
    while (feof(bookingDB) == 0)
    {
        fscanf(bookingDB, "%12[^,],%24[^,],%d,%f,%f\n", &fbooking.nic, &fbooking.suite, &fbooking.days, &fbooking.price, &fbooking.total);
        fflush(stdin);
        if (strcmp(booking.nic, fbooking.nic) == 0)
        {
            printf("\n\t\tBooking found.\n");
            printf("\tNIC: %s\n", fbooking.nic);
            printf("\tSuite Type: %s\n", fbooking.suite);
            printf("\tBooked Days: %d\n", fbooking.days);
            printf("\tPrice: %.2f\n", fbooking.price);
            printf("\tTotal: %.2f\n", fbooking.total);
            printf("\n\tDo you want to update the details? [Y/N]: ");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y')
            {
                printf("\n\tEnter new details:\n");
                printf("\tNIC: ");
                scanf("%s", &booking.nic);
                printf("\n\t[1]: Presidential suite \t %.2f /night \t 02\n", suitePrices[0]);
                printf("\t[2]: Royal suite \t\t %.2f /night \t 03\n", suitePrices[1]);
                printf("\t[3]: Family suite \t\t %.2f /night \t 05\n", suitePrices[2]);
                printf("\tSuite: ");
                scanf("%d", &choice);
                printf("\tBooked Days: ");
                scanf("%d", &booking.days);
                booking.price = suitePrices[choice - 1];
                strcpy(booking.suite, suites[choice - 1]);
                booking.total = booking.price * booking.days;
                printf("\tSuite Price: %.2f\n", booking.price);
                printf("\ttotal: %.2f", booking.total);
                fprintf(tempDB, "%s,%s,%d,%.2f,%.2f\n", booking.nic, booking.suite, booking.days, booking.price, booking.total);
                printf("\n\n\tBooking updated.\n");
            }
            else
            {
                fprintf(tempDB, "%s,%s,%d,%.2f,%.2f\n", fbooking.nic, fbooking.suite, fbooking.days, fbooking.price, fbooking.total);
                printf("\n\tBooking details not updated.\n");
            }
            // goto Addpkg;
        }
        else
        {
            fprintf(tempDB, "%s,%s,%d,%.2f,%.2f\n", fbooking.nic, fbooking.suite, fbooking.days, fbooking.price, fbooking.total);
        }
    }
    fclose(bookingDB);
    fclose(tempDB);
    Sleep(200);
    remove("bookingDB.csv");
    rename("tempDB.csv", "bookingDB.csv");
}

void checkOut()
{
    FILE *bookingDB = fopen("bookingDB.csv", "a+");
    FILE *tempDB = fopen("tempDB.csv", "w");
    int choice;
    int found = 0;
    struct bookingData
    {
        char nic[20];
        char suite[25];
        int days;
        float price;
        float total;
    } booking, fbooking;
checkOut:
    printf("\n\t\tCheckout\n");
    printf("\tEnter NIC or Passport Number: ");
    scanf("%s", &booking.nic);
    while (feof(bookingDB) == 0)
    {
        fscanf(bookingDB, "%12[^,],%24[^,],%d,%f,%f\n", &fbooking.nic, &fbooking.suite, &fbooking.days, &fbooking.price, &fbooking.total);
        fflush(stdin);
        if (strcmp(booking.nic, fbooking.nic) == 0)
        {
            found = 1;
            printf("\n\t\tBooking found.\n");
            printf("\tNIC: %s\n", fbooking.nic);
            printf("\tSuite Type: %s\n", fbooking.suite);
            printf("\tBooked Days: %d\n", fbooking.days);
            printf("\tPrice: %.2f\n", fbooking.price);
            printf("\n\tYour Total is: %.2f\n", fbooking.total);
            printf("\n\tDo you want to checkout? [Y/N]: ");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y')
            {
                printf("\n\tCheckout successful.\n");
                FILE *bill = fopen("bill.txt", "w");
                time_t t = time(NULL);
                fprintf(bill, "\t\tHOTEL ABC\n");
                fprintf(bill, "\tCheckout Date & time: %s\n\n", ctime(&t));
                fprintf(bill, "NIC: %s\n", fbooking.nic);
                fprintf(bill, "Suite Type: %s\n", fbooking.suite);
                fprintf(bill, "Booked Days: %d\n", fbooking.days);
                fprintf(bill, "Price: %.2f\n", fbooking.price);
                fprintf(bill, "----------------\n");
                fprintf(bill, "Total: %.2f\n", fbooking.total);
                fprintf(bill, "----------------\n\n");
                fprintf(bill, "Thank you for choosing us.\nHave a nice day.\n");
                fclose(bill);
                printf("\n\tPrinting the bill....\n");

                system(BILL_CMD);
                saveLog(fbooking.total, 1);
                system("pause");
                remove("bill.txt");
                system("cls");
            }
            else
            {
                fprintf(tempDB, "%s,%s,%d,%.2f,%.2f\n", fbooking.nic, fbooking.suite, fbooking.days, fbooking.price, fbooking.total);
                printf("\n\tBooking details not updated.\n");
            }

            // goto Addpkg;
        }
        else
        {
            fprintf(tempDB, "%s,%s,%d,%.2f,%.2f\n", fbooking.nic, fbooking.suite, fbooking.days, fbooking.price, fbooking.total);
        }
    }
    if (found == 0)
    {
        printf("\n\t\tBooking not found.\n");
        printf("\tDo you want to search again? [Y/N]: ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            system("cls");
            goto checkOut;
        }
    }
    fclose(bookingDB);
    fclose(tempDB);
    Sleep(200);
    remove("bookingDB.csv");
    remove("bill.txt");
    rename("tempDB.csv", "bookingDB.csv");
}

void dining()
{
    system("cls");
    header();
    int menuChoice, QTY, orderCount = 0, drinksOrderCount = 0;
    float totalPrice = 0;
    int choosenFromBreakfast = 0, choosenFromLunch = 0, choosenFromDinner = 0, choosenFromDrinks = 0;
    int *order, *orderQTY, *drinksOrder, *drinksQTY;
    order = (int *)calloc(1, sizeof(int));
    orderQTY = (int *)calloc(1, sizeof(int));
    drinksOrder = (int *)calloc(1, sizeof(int));
    drinksQTY = (int *)calloc(1, sizeof(int));

diningMenu:
    printf("\n\t\tDining\n");
    printf("\t1. Breakfast\n");
    printf("\t2. Lunch\n");
    printf("\t3. Dinner\n");
    printf("\t4. Drinks\n");
    printf("\t5. Back\n");
    printf("\t6. Confirm\n");
    printf("\tyour choice: ");
    scanf("%d", &menuChoice);
    system("cls");
    switch (menuChoice)
    {
    case 1:
    // breakfast();
    breakfastMenu:
        printf("\n\t\tBreakfast\n");
        for (int i = 1; i <= diningCount.breakfast; i++)
        {
            printf("\t%d. %s \t\t %.2f\n", i, breakfastMenu[i], breakfastPrice[i]);
        }
        printf("\t%d. Back\n", diningCount.breakfast + 1);
        printf("\t0. Confirm\n");
        printf("\tEnter your order one by one: \n");
        for (int i = orderCount; i >= 0; i++)
        {
        breakfastOrdering:
            printf("\t\tOrder=> ");
            scanf("%d", &menuChoice);
            if (menuChoice == (diningCount.breakfast + 1))
            {
                system("cls");
                orderCount = 0;
                free(order);
                int *order;
                order = (int *)calloc(1, sizeof(int));
                goto diningMenu;
            }
            else if (menuChoice == 0)
            {
                choosenFromBreakfast = 1;
                system("cls");
                // break;
                goto drinksMenu;
            }
            else if (menuChoice > 0 && menuChoice < (diningCount.breakfast + 1))
            {
                if (order[i] == menuChoice)
                {
                    printf("\t\t QTY you want to add more=> ");
                    scanf("%d", &QTY);
                    printf("\n");
                    orderQTY[i] += QTY;
                    goto breakfastOrdering;
                }
                orderQTY = (int *)realloc(orderQTY, (i + 1) * sizeof(int));
                order = realloc(order, (i + 1) * sizeof(int));
                printf("\t\tQTY=> ");
                scanf("%d", &orderQTY[i]);
                order[i] = menuChoice;
                printf("\n");
                orderCount++;
            }
            else
            {
                printf("\tInvalid choice.\n");
                i--;
                goto breakfastMenu;
            }
        }
        break;
    case 2:
        // lunch();
    lunchMenu:
        printf("\n\t\tLunch\n");
        for (int i = 1; i <= diningCount.lunch; i++)
        {
            printf("\t%d. %s \t\t %.2f\n", i, lunchMenu[i], lunchPrice[i]);
        }
        printf("\t%d. Back\n", diningCount.lunch + 1);
        printf("\t0. Confirm\n");
        printf("\tEnter your order one by one: \n");
        for (int i = orderCount; i >= 0; i++)
        {
        lunchOrdering:
            printf("\t\tOrder=> ");
            scanf("%d", &menuChoice);
            if (menuChoice == (diningCount.lunch + 1))
            {
                system("cls");
                orderCount = 0;
                free(order);
                int *order;
                order = (int *)calloc(1, sizeof(int));
                goto diningMenu;
            }
            else if (menuChoice == 0)
            {
                choosenFromLunch = 1;
                system("cls");
                // break;
                goto drinksMenu;
            }
            else if (menuChoice > 0 && menuChoice < (diningCount.lunch + 1))
            {
                if (order[i] == menuChoice)
                {
                    printf("\t\t QTY you want to add more=> ");
                    scanf("%d", &QTY);
                    printf("\n");
                    orderQTY[i] += QTY;
                    goto lunchOrdering;
                }

                orderQTY = (int *)realloc(orderQTY, (i + 1) * sizeof(int));
                order = realloc(order, (i + 1) * sizeof(int));
                printf("\t\tQTY=> ");
                scanf("%d", &orderQTY[i]);
                order[i] = menuChoice;
                printf("\n");
                orderCount++;
            }
            else
            {
                printf("\tInvalid choice.\n");
                i--;
                goto lunchMenu;
            }
        }
        break;
    case 3:
        // dinner();
    dinnerMenu:
        printf("\n\t\tDinner\n");
        for (int i = 1; i <= diningCount.dinner; i++)
        {
            printf("\t%d. %s \t\t %.2f\n", i, dinnerMenu[i], dinnerPrice[i]);
        }
        printf("\t%d. Back\n", diningCount.dinner + 1);
        printf("\t0. Confirm\n");
        printf("\tEnter your order one by one: \n");
        for (int i = orderCount; i >= 0; i++)
        {
        dinnerOrdering:
            printf("\t\tOrder=> ");
            scanf("%d", &menuChoice);
            if (menuChoice == (diningCount.dinner + 1))
            {
                system("cls");
                orderCount = 0;
                free(order);
                int *order;
                order = (int *)calloc(1, sizeof(int));
                goto diningMenu;
            }
            else if (menuChoice == 0)
            {
                choosenFromDinner = 1;
                system("cls");
                // break;
                goto drinksMenu;
            }
            else if (menuChoice > 0 && menuChoice < (diningCount.dinner + 1))
            {
                if (order[i] == menuChoice)
                {
                    printf("\t\t QTY you want to add more=> ");
                    scanf("%d", &QTY);
                    printf("\n");
                    orderQTY[i] += QTY;
                    goto dinnerOrdering;
                }

                orderQTY = realloc(orderQTY, (i + 1) * sizeof(int));
                order = realloc(order, (i + 1) * sizeof(int));
                printf("\t\tQTY=> ");
                scanf("%d", &orderQTY[i]);
                order[i] = menuChoice;
                printf("\n");
                orderCount++;
            }
            else
            {
                printf("\tInvalid choice.\n");
                i--;
                goto dinnerMenu;
            }
        }
        break;
    case 4:
        goto chooseDrinks;
    // drinks();
    drinksMenu:
        if (drinksOrderCount != 0)
        {
            goto billing;
        }
        printf("Do you want to order drinks? [Y/N]: ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
        chooseDrinks:
            system("cls");
            printf("\n\t\tDrinks\n");
            for (int i = 1; i <= diningCount.drinks; i++)
            {
                printf("\t%d. %s \t\t %.2f\n", i, drinksMenu[i], drinksPrice[i]);
            }
            printf("\t%d. Reset\n", diningCount.drinks + 1);
            printf("\t0. Confirm\n");
            printf("\tEnter your order one by one: \n");
            for (int i = drinksOrderCount; i >= 0; i++)
            {
            drinksOrdering:
                printf("\t\tOrder=> ");
                scanf("%d", &menuChoice);
                if (menuChoice == (diningCount.drinks + 1))
                {
                    system("cls");
                    drinksOrderCount = 0;
                    free(drinksOrder);
                    int *drinksOrder;
                    drinksOrder = (int *)calloc(1, sizeof(int));
                    goto diningMenu;
                }
                else if (menuChoice == 0)
                {
                    choosenFromDrinks = 1;
                    system("cls");
                    // break;
                    if (orderCount == 0)
                    {
                        printf("Do you want to order anything else? [Y/N]: ");
                        char choice;
                        scanf(" %c", &choice);
                        if (choice == 'Y' || choice == 'y')
                        {
                            goto diningMenu;
                        }
                        else
                        {
                            goto billing;
                        }
                    }
                    else
                    {
                        goto billing;
                    }
                }
                else if (menuChoice > 0 && menuChoice < (diningCount.drinks + 1))
                {
                    // check to see already added to array
                    for (int i = 0; i < drinksOrderCount; i++)
                    {
                        if (drinksOrder[i] == menuChoice)
                        {
                            printf("\n\t\t QTY you want to add more=> ");
                            scanf("%d", &QTY);
                            drinksQTY[i] += QTY;
                            goto drinksOrdering;
                        }
                    }
                    drinksOrder = realloc(drinksOrder, (i + 1) * sizeof(int));
                    drinksOrder[i] = menuChoice;
                    drinksQTY = realloc(drinksQTY, (i + 1) * sizeof(int));
                    printf("\t\tQTY=> ");
                    scanf("%d", &drinksQTY[i]);
                    printf("\n");
                    drinksOrderCount++;
                }
                else
                {
                    printf("\tInvalid choice.\n");
                    i--;
                    goto chooseDrinks;
                }
            }
        }
        else
        {
            system("cls");
            goto billing;
        }
        break;
    case 5:
        system("cls");
        return;
    case 6:
        system("cls");
        break;
    }
billing:

    printf("\n\t\tYour order:\n\n");
    if (!choosenFromBreakfast && !choosenFromLunch && !choosenFromDinner && !choosenFromDrinks)
    {
        printf("\tNo order.\n");
        system("pause");
        goto diningMenu;
    }
    else
    {
        totalPrice = 0;
        printf("\t\tOrder \t QTY \t Price \t total\n");
        if (choosenFromBreakfast)
        {
            for (int i = 0; i < orderCount; i++)
            {
                printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", i + 1, breakfastMenu[order[i]], orderQTY[i], breakfastPrice[order[i]], breakfastPrice[order[i]] * orderQTY[i]);
                totalPrice += breakfastPrice[order[i]] * orderQTY[i];
            }
        }
        else if (choosenFromLunch)
        {
            for (int i = 0; i < orderCount; i++)
            {
                printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", i + 1, lunchMenu[order[i]], orderQTY[i], lunchPrice[order[i]], lunchPrice[order[i]] * orderQTY[i]);
                totalPrice += lunchPrice[order[i]] * orderQTY[i];
            }
        }
        else if (choosenFromDinner)
        {
            for (int i = 0; i < orderCount; i++)
            {
                printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", i + 1, dinnerMenu[order[i]], orderQTY[i], dinnerPrice[order[i]], dinnerPrice[order[i]] * orderQTY[i]);
                totalPrice += dinnerPrice[order[i]] * orderQTY[i];
            }
        }
        if (choosenFromDrinks)
        {
            for (int i = 0; i < drinksOrderCount; i++)
            {
                printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", orderCount + i + 1, drinksMenu[drinksOrder[i]], drinksQTY[i], drinksPrice[drinksOrder[i]], drinksPrice[drinksOrder[i]] * drinksQTY[i]);
                totalPrice += drinksPrice[drinksOrder[i]] * drinksQTY[i];
            }
        }
        printf("\t Total price:\t %.2f$\n", totalPrice);
        printf("\n\n\t\t[1]: Confirm\n\t\t[2]: Edit\n");
        printf("\t\tChoice: ");
        scanf("%d", &menuChoice);
        switch (menuChoice)
        {
        case 1:
            // handle confirmation
            printf("\n\tPrinting receipt...\n");
            FILE *bill = fopen("bill.txt", "a");
            time_t t = time(NULL);
            fprintf(bill, "\t\tHOTEL ABC\n");
            fprintf(bill, "\t\t %s\n\n", ctime(&t));
            fprintf(bill, "\tOrder \t\t QTY \t Price \t total\n");
            if (choosenFromBreakfast)
            {
                for (int i = 0; i < orderCount; i++)
                {
                    fprintf(bill, "%d. %s \t%d \t\t%.2f \t %.2f$\n", i + 1, breakfastMenu[order[i]], orderQTY[i], breakfastPrice[order[i]], breakfastPrice[order[i]] * orderQTY[i]);
                }
            }
            else if (choosenFromLunch)
            {
                for (int i = 0; i < orderCount; i++)
                {
                    fprintf(bill, "%d. %s \t%d \t\t%.2f \t %.2f$\n", i + 1, lunchMenu[order[i]], orderQTY[i], lunchPrice[order[i]], lunchPrice[order[i]] * orderQTY[i]);
                }
            }
            else if (choosenFromDinner)
            {
                for (int i = 0; i < orderCount; i++)
                {
                    fprintf(bill, "%d. %s \t%d \t\t%.2f \t %.2f$\n", i + 1, dinnerMenu[order[i]], orderQTY[i], dinnerPrice[order[i]], dinnerPrice[order[i]] * orderQTY[i]);
                }
            }
            if (choosenFromDrinks)
            {
                for (int i = 0; i < drinksOrderCount; i++)
                {
                    fprintf(bill, "%d. %s \t\t%d \t\t%.2f \t %.2f$\n", orderCount + i + 1, drinksMenu[drinksOrder[i]], drinksQTY[i], drinksPrice[drinksOrder[i]], drinksPrice[drinksOrder[i]] * drinksQTY[i]);
                }
            }
            fprintf(bill, "----------------\n");
            fprintf(bill, " Total price:\t %.2f$\n", totalPrice);
            fprintf(bill, "----------------\n\n");
            fprintf(bill, "Thank you for choosing us.\nHave a nice day.\n");
            fclose(bill);
            system(BILL_CMD);
            saveLog(totalPrice, 0);
            system("pause");
            remove("bill.txt");
            system("cls");
            break;
        case 2:
            // handle edit
            printf("\n\t\t[1]:Add More\n");
            printf("\t\t[2]:Remove Items\n");
            printf("\t\t[3]:Reset Order\n");
            printf("\t\tChoice: ");
            scanf("%d", &menuChoice);
            switch (menuChoice)
            {
            case 1:
                system("cls");
                goto diningMenu;
                break;
            case 2:
                system("cls");
                // handle remove
                // printing order
                printf("\t\tOrder \t QTY \t Price \t total\n");
                if (choosenFromBreakfast)
                {
                    for (int i = 0; i < orderCount; i++)
                    {
                        printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", i + 1, breakfastMenu[order[i]], orderQTY[i], breakfastPrice[order[i]], breakfastPrice[order[i]] * orderQTY[i]);
                        totalPrice += breakfastPrice[order[i]] * orderQTY[i];
                    }
                }
                else if (choosenFromLunch)
                {
                    for (int i = 0; i < orderCount; i++)
                    {
                        printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", i + 1, lunchMenu[order[i]], orderQTY[i], lunchPrice[order[i]], lunchPrice[order[i]] * orderQTY[i]);
                        totalPrice += lunchPrice[order[i]] * orderQTY[i];
                    }
                }
                else if (choosenFromDinner)
                {
                    for (int i = 0; i < orderCount; i++)
                    {
                        printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", i + 1, dinnerMenu[order[i]], orderQTY[i], dinnerPrice[order[i]], dinnerPrice[order[i]] * orderQTY[i]);
                        totalPrice += dinnerPrice[order[i]] * orderQTY[i];
                    }
                }
                if (choosenFromDrinks)
                {
                    for (int i = 0; i < drinksOrderCount; i++)
                    {
                        printf("\t%d. %s \t%d \t%.2f \t %.2f$\n", orderCount + i + 1, drinksMenu[drinksOrder[i]], drinksQTY[i], drinksPrice[drinksOrder[i]], drinksPrice[drinksOrder[i]] * drinksQTY[i]);
                        totalPrice += drinksPrice[drinksOrder[i]] * drinksQTY[i];
                    }
                }
                printf("\tEnter the order number You want to remove: ");
                scanf("%d", &menuChoice);
                if (menuChoice > orderCount + drinksOrderCount)
                {
                    printf("\tInvalid order number.\n");
                    system("pause");
                    system("cls");
                    goto diningMenu;
                }
                else if (menuChoice > orderCount)
                {
                    // remove drinks
                    printf("new %s QTY (enter 0 to remove all): ", drinksMenu[drinksOrder[menuChoice - orderCount - 1]]);
                    scanf("%d", &QTY);
                    if (QTY == 0)
                    {
                        for (int i = menuChoice - orderCount - 1; i < drinksOrderCount - 1; i++)
                        {
                            drinksOrder[i] = drinksOrder[i + 1];
                            drinksQTY[i] = drinksQTY[i + 1];
                        }
                        drinksOrderCount--;
                    }
                    else
                    {
                        drinksQTY[menuChoice - 1 - orderCount] = QTY;
                    }
                }
                else
                {
                    // remove food
                    printf("new QTY (enter 0 to remove all):: ");
                    scanf("%d", &QTY);
                    if (QTY == 0)
                    {
                        for (int i = menuChoice - 1; i < orderCount - 1; i++)
                        {
                            order[i] = order[i + 1];
                            orderQTY[i] = orderQTY[i + 1];
                        }
                        orderCount--;
                    }
                    else
                    {
                        orderQTY[menuChoice - 1] = QTY;
                    }
                }
                goto billing;
                break;
            case 3:
                system("cls");
                // handle reset
                free(order);
                int *order;
                order = (int *)calloc(1, sizeof(int));
                free(drinksOrder);
                int *drinksOrder;
                drinksOrder = (int *)calloc(1, sizeof(int));
                orderQTY = 0;
                drinksQTY = 0;
                drinksOrderCount = 0;
                orderCount = 0;
                goto diningMenu;
                break;
            default:
                system("cls");
                goto billing;
                break;
            }
            break;
        default:
            printf("\tInvalid choice.\n");
            goto billing;
        }

        // system("pause");
    }
}

int main()
{
    initMenu();
    welcomeMsg();
    int loginChoice;
login:
    loginChoice = loginScrn();

    // Receptionist Login
    if (loginChoice == 1)
    {
    receptionistLogin:
        if (receptionistLogin())
        {
            printf("\n\t\tLogin Successful\n");
            Sleep(500);
            int choice;
        receptionistScreen:
            choice = receptionistScrn();
            switch (choice)
            {
            case 1:
                // Check In
                checkIn();
                goto receptionistScreen;
                break;
            case 2:
                // Check Out
                checkOut();
                goto receptionistScreen;
                break;
            case 3:
                // Dining
                dining();
                goto receptionistScreen;
                break;
            case 4:
                // Update
                bookingUpdate();
                goto receptionistScreen;
                break;
            case 5:
                // Exit
                goto login;
                break;
            default:
                printf("Invalid choice.\n");
                Sleep(300);
                system("cls");
                goto receptionistScreen;
            }
        }
        else
        {

            system("color 04");
            printf("\n\t\tLogin Failed\n");
            printf("Press 0 to go back\n\n");
            scanf("%d", &loginChoice);
            if (loginChoice == 0)
            {
                system("color 0f");
                system("cls");
                goto login;
            }
            else
            {
                printf("\n\t\tPlease try again\n");
                system("pause");
                system("color 0f");
                system("cls");
                goto receptionistLogin;
            }
        }
    }
    // Manager Login
    else if (loginChoice == 2)
    {
    managerLogin:
        if (managerLogin())
        {
            printf("\n\t\tLogin Successful\n");
            Sleep(500);
            int choice;
        managerScreen:
            choice = managerScrn();
            switch (choice)
            {
            case 1:
                viewIncome();
                goto managerScreen;
                break;
            case 2:
                updateLogins();
                goto managerScreen;
                break;
            case 3:
                viewRoomBookings();
                goto managerScreen;
                break;
            case 4:
                customerUpdate();
                goto managerScreen;
                break;
            case 5:
                updateMenu();
                goto managerScreen;
                break;
            case 6:
                goto login;
                break;
            default:
                printf("Invalid choice.\n");
                Sleep(300);
                system("cls");
                goto managerScreen;
            }
        }
        else
        {
            system("color 04");
            printf("\n\t\tLogin Failed\n");
            printf("Press 0 to go back\n\n");
            scanf("%d", &loginChoice);
            if (loginChoice == 0)
            {
                system("color 0f");
                system("cls");
                goto login;
            }
            else
            {

                printf("\n\t\tPlease try again\n");
                system("pause");
                system("color 0f");
                system("cls");
                goto managerLogin;
            }
        }
    }
    // Exit
    else
    {
        printf("\n\t\tWrong input\n");
        system("cls");
        goto login;
        return 0;
    }
}
