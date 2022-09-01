#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

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
int roomBooking();

#define BILL_CMD "cd /D \"C:\\Windows\\System32\" & rundll32.exe mshtml.dll,PrintHTML \"F:\\Dev\\C\\Hotel Managment System\\bill.txt\""

void header()
{
    printf("\t\t\t ABC HOTEL\n");
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
        Sleep(300);
        system("cls");
        loginScrn();
    }
    else
    {
        return login;
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
    printf("\tEnter Login ID: ");
    scanf("%s", &receptionist.id);
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
        // fprintf(temp, "%s,%s,%s,%s,%s\n", fcustomer.nic, fcustomer.name, fcustomer.address, fcustomer.phone, fcustomer.email);
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
    remove("customerDB.csv");
    rename("temp.csv", "customerDB.csv");
}

void checkIn()
{

    FILE *customerDB = fopen("customerDB.csv", "a+");

    printf("\t\tCheck In\n\n");
    printf("\tEnter customer NIC or passport: ");
    scanf("%s", &customer.nic);
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

                // system("cd /D \"C:\\Windows\\System32\" & rundll32.exe mshtml.dll,PrintHTML \"%s\\bill.txt\"", PATH);
                system(BILL_CMD);
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
    char *breakfastMenu[] = {"Breakfast",
                             "Pancakes",
                             "Waffles",
                             "Omelette",
                             "noodles",
                             "eggs"};
    float breakfastPrice[] = {0, 65.0, 50, 44.99, 59.99, 69.99};

    char *lunchMenu[] = {"Lunch",
                         "Rice",
                         "Noodles",
                         "Pizza",
                         "Burger",
                         "Sandwich"};
    float lunchPrice[] = {0, 165.0, 150, 244.99, 109.99, 69.99};

    char *dinnerMenu[] = {"Dinner",
                          "Rice",
                          "Noodles",
                          "Pizza",
                          "Pasta",
                          "Rotti"};
    float dinnerPrice[] = {0, 165.0, 150, 244.99, 129.00, 5.59};

    char *drinksMenu[] = {"Drinks",
                          "Coke",
                          "Fanta",
                          "Sprite",
                          "Water",
                          "Coffee"};
    float drinksPrice[] = {0, 55.0, 52.0, 50.0, 15.0, 20.0};
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
        for (int i = 1; i < 6; i++)
        {
            printf("\t%d. %s \t\t %.2f\n", i, breakfastMenu[i], breakfastPrice[i]);
        }
        printf("\t6. Back\n");
        printf("\t0. Confirm\n");
        printf("\tEnter your order one by one: \n");
        for (int i = 0; i >= 0; i++)
        {
            printf("\t\tOrder=> ");
            scanf("%d", &menuChoice);
            if (menuChoice == 6)
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
            else if (menuChoice > 0 && menuChoice < 6)
            {
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
        for (int i = 1; i < 6; i++)
        {
            printf("\t%d. %s \t\t %.2f\n", i, lunchMenu[i], lunchPrice[i]);
        }
        printf("\t6. Back\n");
        printf("\t0. Confirm\n");
        printf("\tEnter your order one by one: \n");
        for (int i = 0; i >= 0; i++)
        {
            printf("\t\tOrder=> ");
            scanf("%d", &menuChoice);
            if (menuChoice == 6)
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
            else if (menuChoice > 0 && menuChoice < 6)
            {

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
        for (int i = 1; i < 6; i++)
        {
            printf("\t%d. %s \t\t %.2f\n", i, dinnerMenu[i], dinnerPrice[i]);
        }
        printf("\t6. Back\n");
        printf("\t0. Confirm\n");
        printf("\tEnter your order one by one: \n");
        for (int i = 0; i >= 0; i++)
        {
            printf("\t\tOrder=> ");
            scanf("%d", &menuChoice);
            if (menuChoice == 6)
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
            else if (menuChoice > 0 && menuChoice < 6)
            {

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
        printf("Do you want to order drinks? [Y/N]: ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
        chooseDrinks:
            system("cls");
            printf("\n\t\tDrinks\n");
            for (int i = 1; i < 6; i++)
            {
                printf("\t%d. %s \t\t %.2f\n", i, drinksMenu[i], drinksPrice[i]);
            }
            printf("\t6. Reset\n");
            printf("\t0. Confirm\n");
            printf("\tEnter your order one by one: \n");
            for (int i = 0; i >= 0; i++)
            {
                printf("\t\tOrder=> ");
                scanf("%d", &menuChoice);
                if (menuChoice == 6)
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
                else if (menuChoice > 0 && menuChoice < 6)
                {
                    drinksOrder = realloc(drinksOrder, (i + 1) * sizeof(int));
                    drinksOrder[i] = menuChoice;
                    drinksQTY = realloc(orderQTY, (i + 1) * sizeof(int));
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
            system("pause");
            remove("bill.txt");
            system("cls");
            break;
        case 2:
            // handle edit
            printf("\n\t\t[1]:Add More\n");
            printf("\t\t[2]:Remove\n");
            printf("\t\t[3]:Reset\n");
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
                break;
            case 3:
                system("cls");
                // handle reset
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
    // checkOut();
    welcomeMsg();
    int loginChoice = loginScrn();

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
                loginScrn();
                break;
            default:
                printf("Invalid choice.\n");
                Sleep(300);
                system("cls");
                receptionistScrn();
            }
        }
        else
        {

            system("color 04");
            printf("\n\t\tLogin Failed\n");
            printf("\n\t\tPlease try again\n");
            system("pause");
            system("color 0f");
            system("cls");
            goto receptionistLogin;
        }
    }
    // Manager Login
    else if (loginChoice == 2)
    {
        printf("\n\t\tManager Login\n\n");
        /*
        TODO: Manager Login
        */
    }
}
