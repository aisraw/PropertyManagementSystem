#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define MAX_NAME_LENGTH 100
#define MAX_PROPERTIES 100
#define MAX_CUSTOMERS 100

// ----------------- Structures -----------------
typedef struct {
    int code;
    char name[MAX_NAME_LENGTH];
    char place[MAX_NAME_LENGTH];
    float price;
    char owner[MAX_NAME_LENGTH];
    char category[MAX_NAME_LENGTH];
    char subcategory[MAX_NAME_LENGTH];
    int isSold; // 0 = available, 1 = sold
} Property;

typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    char phone[20];
} Customer;

// ----------------- Globals -----------------
Property properties[MAX_PROPERTIES];
Customer customers[MAX_CUSTOMERS];
int propertyCount = 0;
int customerCount = 0;

// ----------------- Utility: Console Colors & Typewriter -----------------
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// fast subtle typewriter print (small delay)
void type_print(const char* s) {
    const DWORD DELAY_MS = 6; // subtle & fast
    for (size_t i = 0; i < strlen(s); ++i) {
        putchar(s[i]);
        fflush(stdout);
        Sleep(DELAY_MS);
    }
}

// print a line with typewriter and newline
void type_println(const char* s) {
    type_print(s);
    putchar('\n');
}

// clear screen wrapper
void clear_screen() {
    system("cls");
}

// ----------------- Prototypes -----------------
void loadProperties();
void saveProperties();
void loadCustomers();
void saveCustomers();

void showHeader();
void mainMenu();
void adminPanel();
void customerPanel();
void userService();

void addProperty();
void displayProperties();
void searchProperty();
void updateProperty();
void deleteProperty();
void viewPropertyStatus();

void customerSignUp();
void customerLogin();
void viewPropertiesCustomer();

// ----------------- File IO -----------------
void saveProperties() {
    FILE *f = fopen("properties.dat", "wb");
    if (!f) {
        setColor(12); // red
        printf("Error: cannot save properties file.\n");
        setColor(15);
        return;
    }
    fwrite(&propertyCount, sizeof(int), 1, f);
    fwrite(properties, sizeof(Property), propertyCount, f);
    fclose(f);
}

void loadProperties() {
    FILE *f = fopen("properties.dat", "rb");
    if (!f) {
        // no previous data is OK
        return;
    }
    fread(&propertyCount, sizeof(int), 1, f);
    if (propertyCount < 0 || propertyCount > MAX_PROPERTIES) propertyCount = 0;
    fread(properties, sizeof(Property), propertyCount, f);
    fclose(f);
}

void saveCustomers() {
    FILE *f = fopen("customers.dat", "wb");
    if (!f) {
        setColor(12);
        printf("Error: cannot save customers file.\n");
        setColor(15);
        return;
    }
    fwrite(&customerCount, sizeof(int), 1, f);
    fwrite(customers, sizeof(Customer), customerCount, f);
    fclose(f);
}

void loadCustomers() {
    FILE *f = fopen("customers.dat", "rb");
    if (!f) return;
    fread(&customerCount, sizeof(int), 1, f);
    if (customerCount < 0 || customerCount > MAX_CUSTOMERS) customerCount = 0;
    fread(customers, sizeof(Customer), customerCount, f);
    fclose(f);
}

// ----------------- Display Header -----------------
void showHeader() {
    clear_screen();
    setColor(11); // light cyan
    type_println("==============================================");
    type_println("  Welcome to Property Management System (PMS)  ");
    type_println("            by Warish Group (Professional)     ");
    type_println("==============================================");
    setColor(15); // reset
}

// ----------------- Main Menu -----------------
void mainMenu() {
    int choice;
    time_t now;
    time(&now);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%c", localtime(&now));
    printf("\n");
    setColor(10); // green
    printf("Current time: %s\n", timebuf);
    setColor(15);

    while (1) {
        showHeader();
        setColor(13); // magenta
        type_println("\nMain Menu:");
        setColor(14); // yellow
        printf("1. Manager (Admin) Access\n");
        printf("2. Customer Sign In\n");
        printf("3. User Service / Contact\n");
        printf("4. Exit\n");
        setColor(15);
        printf("\nEnter Your Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            setColor(12);
            type_println("Invalid input. Try again.");
            setColor(15);
            Sleep(700);
            continue;
        }
        getchar(); // consume newline

        switch (choice) {
            case 1: adminPanel(); break;
            case 2: customerPanel(); break;
            case 3: userService(); break;
            case 4:
                setColor(10);
                type_println("\nThank you for using Warish Property Management System :)");
                setColor(15);
                saveProperties();
                saveCustomers();
                exit(0);
            default:
                setColor(12);
                type_println("Invalid Choice! Try Again.");
                setColor(15);
                Sleep(700);
        }
    }
}

// ----------------- Admin Panel -----------------
void adminPanel() {
    int pin;
    showHeader();
    setColor(11);
    type_print("Enter Admin Pin: ");
    setColor(15);
    if (scanf("%d", &pin) != 1) {
        while (getchar() != '\n');
        setColor(12);
        type_println("Invalid input for PIN.");
        setColor(15);
        Sleep(700);
        return;
    }
    getchar();
    if (pin != 4049) {
        setColor(12);
        type_println("Invalid PIN! Access Denied.");
        setColor(15);
        Sleep(800);
        return;
    }

    int option;
    while (1) {
        showHeader();
        setColor(11);
        type_println("\n-- ADMIN PANEL --");
        setColor(14);
        printf("1. Add Property\n");
        printf("2. Display All Properties\n");
        printf("3. Search Property\n");
        printf("4. Update Property\n");
        printf("5. Delete Property\n");
        printf("6. View Property Status\n");
        printf("7. Save Properties Now\n");
        printf("8. Back to Main Menu\n");
        setColor(15);
        printf("\nEnter your choice: ");
        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            setColor(12);
            type_println("Invalid input. Try again.");
            setColor(15);
            Sleep(700);
            continue;
        }
        getchar();

        switch (option) {
            case 1: addProperty(); break;
            case 2: displayProperties(); break;
            case 3: searchProperty(); break;
            case 4: updateProperty(); break;
            case 5: deleteProperty(); break;
            case 6: viewPropertyStatus(); break;
            case 7:
                saveProperties();
                setColor(10);
                type_println("Properties saved successfully!");
                setColor(15);
                break;
            case 8: return;
            default:
                setColor(12);
                type_println("Invalid choice! Try again.");
                setColor(15);
        }
        setColor(15);
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// ----------------- Customer Panel -----------------
void customerPanel() {
    int choice;
    while (1) {
        showHeader();
        setColor(11);
        type_println("\n-- CUSTOMER PANEL --");
        setColor(14);
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Back to Main Menu\n");
        setColor(15);
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            setColor(12);
            type_println("Invalid input. Try again.");
            setColor(15);
            Sleep(700);
            continue;
        }
        getchar();

        switch (choice) {
            case 1: customerSignUp(); break;
            case 2: customerLogin(); break;
            case 3: return;
            default:
                setColor(12);
                type_println("Invalid choice! Try again.");
                setColor(15);
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// ----------------- User Service -----------------
void userService() {
    showHeader();
    setColor(11);
    type_println("\n=== USER SERVICE ===");
    setColor(15);
    printf("For any queries or support, please contact:\n");
    printf("Email: support@warishgroup.com\n");
    printf("Phone: +1-234-567-8900\n");
    printf("Office Hours: 9:00 AM - 6:00 PM\n");
    setColor(15);
    printf("\nPress Enter to go back...");
    getchar();
}

// ----------------- Property Functions -----------------
void addProperty() {
    if (propertyCount >= MAX_PROPERTIES) {
        setColor(12);
        type_println("Property database full!");
        setColor(15);
        return;
    }

    Property p;
    p.code = propertyCount + 1;
    showHeader();
    setColor(11);
    type_println("\n=== ADD NEW PROPERTY ===");
    setColor(15);

    printf("Enter Property Name: ");
    fgets(p.name, MAX_NAME_LENGTH, stdin);
    p.name[strcspn(p.name, "\n")] = 0;

    printf("Enter Place: ");
    fgets(p.place, MAX_NAME_LENGTH, stdin);
    p.place[strcspn(p.place, "\n")] = 0;

    printf("Enter Price: ");
    if (scanf("%f", &p.price) != 1) {
        while (getchar() != '\n');
        setColor(12);
        type_println("Invalid price. Aborting add.");
        setColor(15);
        return;
    }
    getchar();

    printf("Enter Owner Name: ");
    fgets(p.owner, MAX_NAME_LENGTH, stdin);
    p.owner[strcspn(p.owner, "\n")] = 0;

    printf("Enter Category (Residential/Commercial): ");
    fgets(p.category, MAX_NAME_LENGTH, stdin);
    p.category[strcspn(p.category, "\n")] = 0;

    printf("Enter Subcategory: ");
    fgets(p.subcategory, MAX_NAME_LENGTH, stdin);
    p.subcategory[strcspn(p.subcategory, "\n")] = 0;

    p.isSold = 0;
    properties[propertyCount++] = p;

    saveProperties();
    setColor(10);
    type_println("\nProperty added successfully!");
    setColor(15);
}

void displayProperties() {
    showHeader();
    setColor(11);
    type_println("\n=== ALL PROPERTIES ===");
    setColor(15);

    if (propertyCount == 0) {
        setColor(12);
        type_println("No properties available!");
        setColor(15);
        return;
    }

    printf("Code\tName\t\tPlace\t\tPrice\t\tOwner\t\tCategory\tStatus\n");
    printf("--------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < propertyCount; ++i) {
        printf("%d\t%s\t\t%s\t\t%.2f\t\t%s\t\t%s\t%s\n",
               properties[i].code,
               properties[i].name,
               properties[i].place,
               properties[i].price,
               properties[i].owner,
               properties[i].category,
               properties[i].isSold ? "Sold" : "Available");
    }
}

void searchProperty() {
    showHeader();
    if (propertyCount == 0) {
        setColor(12);
        type_println("No properties available!");
        setColor(15);
        return;
    }

    int choice;
    printf("\n=== SEARCH PROPERTY ===\n");
    printf("1. Search by Code\n");
    printf("2. Search by Place\n");
    printf("3. Search by Price Range\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input."); setColor(15); return; }
    getchar();

    int found = 0;
    if (choice == 1) {
        int code;
        printf("Enter Property Code: ");
        if (scanf("%d", &code) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input."); setColor(15); return; }
        getchar();
        for (int i = 0; i < propertyCount; ++i) {
            if (properties[i].code == code) {
                found = 1;
                setColor(10);
                printf("\nProperty Found:\nCode: %d\nName: %s\nPlace: %s\nPrice: %.2f\nOwner: %s\nCategory: %s\nSubcategory: %s\nStatus: %s\n",
                       properties[i].code, properties[i].name, properties[i].place, properties[i].price,
                       properties[i].owner, properties[i].category, properties[i].subcategory, properties[i].isSold ? "Sold" : "Available");
                setColor(15);
                break;
            }
        }
        if (!found) { setColor(12); type_println("Property not found!"); setColor(15); }
    } else if (choice == 2) {
        char place[MAX_NAME_LENGTH];
        printf("Enter Place: ");
        fgets(place, MAX_NAME_LENGTH, stdin);
        place[strcspn(place, "\n")] = 0;
        printf("\nProperties in %s:\n", place);
        for (int i = 0; i < propertyCount; ++i) {
            if (strcmp(properties[i].place, place) == 0) {
                found = 1;
                printf("Code: %d, Name: %s, Price: %.2f, Status: %s\n",
                       properties[i].code, properties[i].name, properties[i].price, properties[i].isSold ? "Sold" : "Available");
            }
        }
        if (!found) { setColor(12); type_println("No properties found in that place."); setColor(15); }
    } else if (choice == 3) {
        float minPrice, maxPrice;
        printf("Enter Minimum Price: ");
        if (scanf("%f", &minPrice) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input."); setColor(15); return; }
        printf("Enter Maximum Price: ");
        if (scanf("%f", &maxPrice) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input."); setColor(15); return; }
        getchar();
        printf("\nProperties in price range %.2f - %.2f:\n", minPrice, maxPrice);
        for (int i = 0; i < propertyCount; ++i) {
            if (properties[i].price >= minPrice && properties[i].price <= maxPrice) {
                found = 1;
                printf("Code: %d, Name: %s, Place: %s, Price: %.2f, Status: %s\n",
                       properties[i].code, properties[i].name, properties[i].place,
                       properties[i].price, properties[i].isSold ? "Sold" : "Available");
            }
        }
        if (!found) { setColor(12); type_println("No properties found in this price range."); setColor(15); }
    } else {
        setColor(12); type_println("Invalid choice!"); setColor(15);
    }
}

void updateProperty() {
    if (propertyCount == 0) {
        setColor(12); type_println("No properties available!"); setColor(15); return;
    }
    int code;
    printf("Enter Property Code to update: ");
    if (scanf("%d", &code) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input."); setColor(15); return; }
    getchar();

    for (int i = 0; i < propertyCount; ++i) {
        if (properties[i].code == code) {
            printf("\nUpdating Property Code %d:\n", code);

            printf("Enter New Property Name (current: %s): ", properties[i].name);
            fgets(properties[i].name, MAX_NAME_LENGTH, stdin);
            properties[i].name[strcspn(properties[i].name, "\n")] = 0;

            printf("Enter New Place (current: %s): ", properties[i].place);
            fgets(properties[i].place, MAX_NAME_LENGTH, stdin);
            properties[i].place[strcspn(properties[i].place, "\n")] = 0;

            printf("Enter New Price (current: %.2f): ", properties[i].price);
            if (scanf("%f", &properties[i].price) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input. Price not updated."); setColor(15); } 
            getchar();

            printf("Enter New Owner (current: %s): ", properties[i].owner);
            fgets(properties[i].owner, MAX_NAME_LENGTH, stdin);
            properties[i].owner[strcspn(properties[i].owner, "\n")] = 0;

            setColor(10);
            type_println("Property updated successfully!");
            setColor(15);
            saveProperties();
            return;
        }
    }
    setColor(12); type_println("Property not found!"); setColor(15);
}

void deleteProperty() {
    if (propertyCount == 0) {
        setColor(12); type_println("No properties available!"); setColor(15); return;
    }
    int code;
    printf("Enter Property Code to delete: ");
    if (scanf("%d", &code) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input."); setColor(15); return; }
    getchar();

    for (int i = 0; i < propertyCount; ++i) {
        if (properties[i].code == code) {
            for (int j = i; j < propertyCount - 1; ++j) {
                properties[j] = properties[j + 1];
                properties[j].code = j + 1; // reassign codes sequentially
            }
            propertyCount--;
            setColor(10); type_println("Property deleted successfully!"); setColor(15);
            saveProperties();
            return;
        }
    }
    setColor(12); type_println("Property not found!"); setColor(15);
}

void viewPropertyStatus() {
    if (propertyCount == 0) {
        setColor(12); type_println("No properties available!"); setColor(15); return;
    }
    int available = 0, sold = 0;
    float totalValue = 0.0f, soldValue = 0.0f;
    for (int i = 0; i < propertyCount; ++i) {
        totalValue += properties[i].price;
        if (properties[i].isSold) {
            sold++;
            soldValue += properties[i].price;
        } else available++;
    }
    showHeader();
    setColor(11); type_println("\n=== PROPERTY STATUS REPORT ==="); setColor(15);
    printf("Total Properties: %d\n", propertyCount);
    printf("Available Properties: %d\n", available);
    printf("Sold Properties: %d\n", sold);
    printf("Total Property Value: %.2f\n", totalValue);
    printf("Sold Property Value: %.2f\n", soldValue);
    printf("Available Property Value: %.2f\n", totalValue - soldValue);
}

// ----------------- Customer Functions -----------------
void customerSignUp() {
    if (customerCount >= MAX_CUSTOMERS) {
        setColor(12); type_println("Customer database full!"); setColor(15); return;
    }
    Customer c;
    showHeader();
    setColor(11); type_println("\n=== CUSTOMER SIGN UP ==="); setColor(15);

    printf("Enter Name: ");
    fgets(c.name, MAX_NAME_LENGTH, stdin);
    c.name[strcspn(c.name, "\n")] = 0;

    printf("Enter Phone: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    c.phone[strcspn(c.phone, "\n")] = 0;

    printf("Enter Username: ");
    fgets(c.username, MAX_NAME_LENGTH, stdin);
    c.username[strcspn(c.username, "\n")] = 0;

    // duplicate username check
    for (int i = 0; i < customerCount; ++i) {
        if (strcmp(customers[i].username, c.username) == 0) {
            setColor(12); type_println("Username already exists! Try a different one."); setColor(15);
            return;
        }
    }

    printf("Enter Password: ");
    fgets(c.password, MAX_NAME_LENGTH, stdin);
    c.password[strcspn(c.password, "\n")] = 0;

    customers[customerCount++] = c;
    saveCustomers();
    setColor(10); type_println("Registration successful!"); setColor(15);
}

void customerLogin() {
    char username[MAX_NAME_LENGTH], password[MAX_NAME_LENGTH];
    showHeader();
    setColor(11); type_println("\n=== CUSTOMER LOGIN ==="); setColor(15);

    printf("Enter Username: ");
    fgets(username, MAX_NAME_LENGTH, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter Password: ");
    fgets(password, MAX_NAME_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0;

    for (int i = 0; i < customerCount; ++i) {
        if (strcmp(customers[i].username, username) == 0 &&
            strcmp(customers[i].password, password) == 0) {
            setColor(10);
            printf("Login successful! Welcome %s\n", customers[i].name);
            setColor(15);
            // goto customer property view
            viewPropertiesCustomer();
            return;
        }
    }
    setColor(12); type_println("Invalid username or password!"); setColor(15);
}

void viewPropertiesCustomer() {
    if (propertyCount == 0) {
        setColor(12); type_println("No properties available!"); setColor(15); return;
    }

    int choice;
    while (1) {
        showHeader();
        setColor(11); type_println("\n=== AVAILABLE PROPERTIES ==="); setColor(15);
        printf("Code\tName\t\tPlace\t\tPrice\t\tCategory\n");
        printf("------------------------------------------------------------------------\n");
        for (int i = 0; i < propertyCount; ++i) {
            if (!properties[i].isSold) {
                printf("%d\t%s\t\t%s\t\t%.2f\t\t%s\n",
                       properties[i].code, properties[i].name, properties[i].place, properties[i].price, properties[i].category);
            }
        }
        setColor(14);
        printf("\n1. Buy Property\n2. Search Property\n3. Back to Customer Menu\n");
        setColor(15);
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            setColor(12); type_println("Invalid input. Try again."); setColor(15);
            Sleep(700);
            continue;
        }
        getchar();

        if (choice == 1) {
            int code;
            printf("Enter Property Code to buy: ");
            if (scanf("%d", &code) != 1) { while (getchar() != '\n'); setColor(12); type_println("Invalid input."); setColor(15); continue; }
            getchar();
            int bought = 0;
            for (int i = 0; i < propertyCount; ++i) {
                if (properties[i].code == code && !properties[i].isSold) {
                    properties[i].isSold = 1;
                    setColor(10); printf("Congratulations! You have purchased: %s\n", properties[i].name); setColor(15);
                    printf("Please contact the owner: %s\n", properties[i].owner);
                    saveProperties();
                    bought = 1;
                    break;
                }
            }
            if (!bought) { setColor(12); type_println("Property not available or not found!"); setColor(15); }
            printf("\nPress Enter to continue...");
            getchar();
        } else if (choice == 2) {
            searchProperty();
            printf("\nPress Enter to continue...");
            getchar();
        } else if (choice == 3) {
            return;
        } else {
            setColor(12); type_println("Invalid choice!"); setColor(15);
        }
    }
}

// ----------------- Main -----------------
int main() {
    // load saved data (if any)
    loadProperties();
    loadCustomers();

    // subtle welcome
    setColor(11);
    type_println("\nWelcome! Loading Property Management System...");
    setColor(15);
    Sleep(300);

    mainMenu(); // run main menu loop

    // Program end (should not reach due to exit in menus)
    saveProperties();
    saveCustomers();
    return 0;
}
