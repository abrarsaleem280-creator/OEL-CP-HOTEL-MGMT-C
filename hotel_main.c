//                                          HOTEL MANAGEMENT SYSTEM 
//                       ============================================================
/*
 * Project: Hotel Management System
 * In program: to manage hotel room reservations, guest check-ins/check-outs, and billing using file I/O.
 * Roll Numbers: [CS-043] [CS-036] [CS-044] [CS-046]
 * Section/Group: [SECTION A ( FE 2026 )]
 */

#include <stdio.h>

#include <string.h>

#include "validation.h"   // our own header file (MAX, struct, validation functions)

// ---------- ADMIN LOGIN SETTINGS ----------
#define ADMIN_USER "RECEPTIONIST"
#define ADMIN_PASS "Admin@123"     
#define MAX_LOGIN_ATTEMPTS 3

void displayMenu(); // each function for a different purpose 
void addReservation();
void displayReservations();
void searchReservation();
int updateRoomDetails(int i);
void updateReservation();
void deleteReservation();
int checkRoomAvailability(int roomNumber);
void checkInGuest();
void checkOutGuest();
void saveToFile();
void loadFromFile();
void generateReport();
void setRoomTypeAndPrice(int i); // helper functions used again and again
void printReservation(int i);
    // the real variables (validation.h only says "extern" about them)
    struct Reservation reservations[MAX];
    int count = 0;
    int nextID = 1;

// ---------- ADMIN LOGIN ----------

void removeNewline(char *s)
{
    s[strcspn(s, "\n")] = '\0';
}

int adminLogin(void)
{
    char username[50], password[50];
    int attempts = 0;

    while (attempts < MAX_LOGIN_ATTEMPTS)
    {
        printf("\n=========== ADMIN LOGIN ===========\n");

        printf("Username: ");
        if (fgets(username, sizeof(username), stdin) == NULL)
            return 0;
        removeNewline(username);

        printf("Password: ");
        if (fgets(password, sizeof(password), stdin) == NULL)
            return 0;
        removeNewline(password);

        if (strcmp(username, ADMIN_USER) == 0 &&
            strcmp(password, ADMIN_PASS) == 0)
        {
            printf("\nLogin successful! Welcome, Admin.\n");
            return 1;
        }

        attempts++;
        printf("Wrong credentials ! Try again: %d\n",
               MAX_LOGIN_ATTEMPTS - attempts);
    }

    printf("\n bye bye, you are not the admin \n");
    return 0;
}

// ---------- HELPER FUNCTIONS (room type and price, print reservation) ----------

// sets room type and price per night from the room number (used in add and update)
void setRoomTypeAndPrice(int i)
{
    int room = reservations[i].roomNumber;

    if(room >= 101 && room <= 110)
    {
        strcpy(reservations[i].roomType, "Standard"); // copying standard,deluxe,suite into the system
        reservations[i].pricePerNight = 4000;
    }
    else if(room >= 201 && room <= 210)
    {
        strcpy(reservations[i].roomType, "Deluxe");
        reservations[i].pricePerNight = 6000;
    }
    else
    {
        strcpy(reservations[i].roomType, "Suite");
        reservations[i].pricePerNight = 8000;
    }
}

// prints all the details of one reservation (i = position in the array)
void printReservation(int i)
{
    printf("Reservation ID: %d\n", reservations[i].reservationID);
    printf("Guest Name: %s\n", reservations[i].guestName);
    printf("Phone No: %s\n", reservations[i].phone);
    printf("Room Number: %d\n", reservations[i].roomNumber);
    printf("Room Type: %s\n", reservations[i].roomType);
    printf("Number of Days: %d\n", reservations[i].numberOfDays);
    printf("Price Per Night: Rs. %.2f\n", reservations[i].pricePerNight);
    printf("Total Bill: Rs. %.2f\n", reservations[i].totalBill);

    // status: 0 = RESERVED, 1 = CHECKED IN, 2 = CHECKED OUT
    if(reservations[i].status == 0)
    {
        printf("Status: Reserved\n");
    }
    else if(reservations[i].status == 1)
    {
        printf("Status: Checked In\n");
    }
    else
    {
        printf("Status: Checked Out\n");
    }
}

int main() {
    loadFromFile();
    if (!adminLogin()) 
    return 0;
    int opt;
    
    do  {
        displayMenu();
    
    printf("Select an Option:"); // options for the receptionist to enter
    scanf("%d",&opt);
    switch (opt)
    {
    case 1:
        {
            addReservation();
        break;
        }
    case 2:
        {
            displayReservations();
        break;
        }
    case 3:
        {
            searchReservation();
        break;
        }
    case 4:
        {
            updateReservation();
        break;
        }
    case 5:
        {
            deleteReservation();
        break;
        }
    case 6:
        {
    int roomNumber;

    printf("Enter Room Number: ");
    scanf("%d", &roomNumber);

    // 100 series is for standard , 200 for deluxe and 300 for suite 

if(!isValidRoom(roomNumber))
{
    printf("Invalid Room Number!\n");
    break;
}
    if(checkRoomAvailability(roomNumber) == 1) // checking if room is available by calling checkroomAvailibility
    {
        printf("\nCongratulations! Room is Available.\n");
    }
    else
    {
        printf("\nSorry! Room is Not Available.\n");
    }

    break;
        }
    case 7:
        {
            checkInGuest();
        break;
        }
    case 8:
        {
            checkOutGuest();
        break;
        }
    case 9:
        {
            generateReport();
        break;
        }
    case 0:
        {
            printf("You exit from the management system");
        break;
        }
    default:
        printf("Invalid Option Selected");
        break;
    }
    } while (opt != 0);
    return 0;
}

// Displaying the menu 

void displayMenu(void) {
    printf("=================================\n");
    printf("     HOTEL MANAGEMENT SYSTEM\n");
    printf("=================================\n");
    printf("\n");
    printf("---------MAIN MENU----------\n");
    printf("1.Add New Reservation.\n");
    printf("2.Display All Reservation.\n");
    printf("3.Search Reservation.\n");
    printf("4.Update Reservation.\n");
    printf("5.Delete Reservations.\n");
    printf("6.Check Room Availbility.\n");
    printf("7.Guest Check-In\n");
    printf("8.Guest Check-Out.\n");
    printf("9.Generate Hotel Report.\n");
    printf("0.Exit\n");
}

// ADDING NEW RESERVATION 

void addReservation(void) {
    if(count >= MAX) {
        printf("Reservation Limit Reached!\n");
        return;
    }

    printf("==========ADD NEW RESERVATION==========\n");

    // Validating guest name: cannot be empty, only letters and spaces (checked inside isValidName)

do
{
    printf("Enter Guest Name:");
    scanf(" %49[^\n]",reservations[count].guestName);
} while (isValidName(reservations[count].guestName) == 0);

    // Validating phone number: Must be exactly 11 digits and contain only numeric characters
    // and cannot be same as another guest's (all checked inside isValidPhone)

do
{
    printf("Enter Phone Number: ");
    scanf("%19s", reservations[count].phone);
} while (isValidPhone(reservations[count].phone) == 0);
    
    printf("Enter Room Number 101 to 110 (standard) , 201 to 210 (deluxe) , 301 to 310 (suite)");
    scanf("%d",&reservations[count].roomNumber);

    if(!isValidRoom(reservations[count].roomNumber))
    {
    printf("Invalid Room Number!\n");
    return;
    }

    // checking if room is available by calling checkroomAvailibility

    if(checkRoomAvailability(reservations[count].roomNumber) == 0) 
    {
    printf("Sorry! Room is Not Available.\n");
    return;
    }
else
    {
    printf("Congratulations! Room is Available.\n");
    }
    setRoomTypeAndPrice(count); // sets room type and price from the room number
printf("Room Type: %s\n", reservations[count].roomType);
printf("Price Per Night: Rs. %.2f\n",
       reservations[count].pricePerNight);

printf("Enter No. of Days: ");
scanf("%d", &reservations[count].numberOfDays);

// validating the room number and number of days 

    if(reservations[count].roomNumber <= 0 || 
        reservations[count].numberOfDays <= 0 ) 
        {
        printf("Invalid Information.Try adding Information Again\n");
        return;
    } 

    // generating automatic next id by ++

    reservations[count].reservationID = nextID;
    nextID++;

    // calculating the total bill by multiplying the number of days by price per night

    reservations[count].totalBill = reservations[count].pricePerNight * reservations[count].numberOfDays ;
    reservations[count].status = 0; 

    // setting status to reserved 

    count++;
    printf("=================================\n");
    printf("Reservation Succesful\n");
    printf("=================================\n");

    printf("Reservation ID: %d\n",reservations[count - 1].reservationID);

    printf("Guest Name: %s\n",reservations[count - 1].guestName);
    
    printf("Room Number: %d\n",reservations[count - 1].roomNumber);
    
    printf("Total Bill: Rs%.2f\n",reservations[count - 1].totalBill);
    
    printf("=======================================================\n");
    saveToFile();
    
}
void displayReservations(void) 
{
    if(count==0) {
        printf("No Reservation Found\n");
        return;
    }
    printf("==============================\n");
    printf("======All Reservations========\n");
    printf("==============================\n");
    int i;

    // loop until the count of reservations 

    for(i = 0;i < count;i++) {
    printReservation(i);
        printf("--------------------------------------\n");
    }
}

void searchReservation(void) {
    int id,i;
    int found = 0;
    printf("\nEnter Reservation ID:\n");
    scanf("%d", &id);

// loop until the count of reservations

    for(i=0;i<count;i++) 

    // if found then update the flag 

    {
        if(reservations[i].reservationID == id) {
            found = 1;
            printf("\n===============Reservation Found===============\n");
            printReservation(i);
            printf("\n=============================================\n");
                break;
        }
    }
    if(found == 0)
    {
        printf("\n Reservation Not Found!\n");
    }
}
int updateRoomDetails(int i)
{
    int room;

    printf("\nEnter New Room Number: ");
    scanf("%d", &room);

    if(!isValidRoom(room))
    {
        printf("\nInvalid Room Number!");
        return 0;
    }

    if(checkRoomAvailability(room) == 0 && 
    
    // checking if room is available or not or already occupied by someone else

       reservations[i].roomNumber != room)
    {
        printf("\nSorry! Room is Already Occupied.");
        return 0;
    }

    
    reservations[i].roomNumber = room;
    setRoomTypeAndPrice(i); // sets room type and price from the new room number

    reservations[i].totalBill =
        reservations[i].numberOfDays *
        reservations[i].pricePerNight;

    printf("\nRoom Details Updated Successfully!");

    return 1;
    // (saving is done in updateReservation after the update)
}
void updateReservation(void)
{
    int id, i, choice;
    int found = 0;
    struct Reservation oldReservation;

    printf("\n========== UPDATE RESERVATION ==========\n");


    printf("Enter Reservation ID: ");
    scanf("%d", &id);
    for(i = 0; i < count; i++) 

    // loop until the count of reservations

    {
        if(reservations[i].reservationID == id)
        {
            found = 1;
            break;
        }
    }

    if(found == 0)
    {
        printf("\nReservation Not Found!\n");
        return;
    }
    // showing the current details 
    printf("\n========== CURRENT DETAILS ==========\n");

    printReservation(i);



    printf("\n========== UPDATE MENU ==========\n");

    printf("1. Update Guest Name\n");
    printf("2. Update Phone Number\n");
    printf("3. Update Room Number\n");
    printf("4. Update Number of Days\n");
    printf("5. Update Multiple Details\n");
    printf("0. Cancel Update\n");

    printf("\nEnter Your Choice: "); // getting choice from user 
    scanf("%d", &choice);
    switch(choice)
    {
      
       case 1:

            do
            {
                printf("Enter New Guest Name: ");
                scanf(" %49[^\n]", reservations[i].guestName);
            } while (isValidName(reservations[i].guestName) == 0);
            break;

    case 2:
{
    char newPhone[20];

do
{
    printf("Enter Phone Number: ");
    scanf("%19s", newPhone);
} while (strcmp(newPhone, reservations[i].phone) != 0 && isValidPhone(newPhone) == 0); // same as his own old number is ok, otherwise 11 digits, only numbers, not same as someone else

strcpy(reservations[i].phone, newPhone);
break;
}

        case 3:

            if(updateRoomDetails(i) == 0)
            {
                return;
            }

            break;

        case 4:

            printf("Enter New Number of Days: ");

            scanf("%d",
                  &reservations[i].numberOfDays);

            if(reservations[i].numberOfDays <= 0)
            {
                printf("\nInvalid Number of Days!\n");
                return;
            }

            reservations[i].totalBill = reservations[i].pricePerNight * reservations[i].numberOfDays ;

            break;

        case 5: // for multiple data to be updated 
        {
            int roomChoice;
            char newPhone[20];
    oldReservation = reservations[i];

    do
    {
        printf("\nEnter New Guest Name: ");
        scanf(" %49[^\n]", reservations[i].guestName);
    } while (isValidName(reservations[i].guestName) == 0);
    
do
{
    printf("Enter Phone Number: ");
    scanf("%19s", newPhone);
} while (strcmp(newPhone, reservations[i].phone) != 0 && isValidPhone(newPhone) == 0); // own old number is ok

strcpy(reservations[i].phone, newPhone);

    // Validate Number of Days
    printf("\nEnter New Number of Days: ");

    scanf("%d", &reservations[i].numberOfDays);

    if(reservations[i].numberOfDays <= 0)
    {
        printf("\nInvalid Number of Days!\n");
        printf("Update Cancelled! Original Details Restored.\n");

        reservations[i] = oldReservation;
        return;
    }

    // Ask for Room Change
    printf("\nDo You Want to Change Room? (Yes=1, No=0): ");

    scanf("%d", &roomChoice);

    if(roomChoice != 0 && roomChoice != 1)
    {
        printf("\nInvalid Choice!\n");
        printf("Original Details Restored.\n");

        reservations[i] = oldReservation;
        return;
    }

    if(roomChoice == 1)
    {
        if(updateRoomDetails(i) == 0)
        {
            printf("\nUpdate Failed! Original Details Restored.\n");

            reservations[i] = oldReservation;
            return;
        }
    }

    break;
        }
        case 0:

            printf("\nUpdate Cancelled!\n");
            return;


        default:

            printf("\nInvalid Choice!\n");
            return;
    }
    reservations[i].totalBill =
        reservations[i].numberOfDays *
        reservations[i].pricePerNight;

    printf("\n========== UPDATED RESERVATION ==========\n");

    printReservation(i);

    printf("\nReservation Updated Successfully!\n");
        saveToFile();
}

void deleteReservation() {
    int id;
    printf("Enter Reservation id to delete:");
    scanf("%d",&id);

    int i,j,found=0;
    for(i = 0; i < count; i++) {
        if(reservations[i].reservationID == id) {
            found = 1;
            break;
        }
    }
    
    if(found == 1) {
        for(j = i; j < count - 1; j++) 

        // Shift all subsequent records one index to the left to overwrite the deleted record

        {
            reservations[j]=reservations[j+1];
        }
        count-- ;

        printf("Reservation Deleted Succesfully!\n");
    }
    else {
           printf("Reservation Not Found!\n");
        }
        saveToFile();
}

int checkRoomAvailability(int roomNumber)
{
    int i;

    for(i = 0; i < count; i++) // loop till the reservations 
    {
        if(reservations[i].roomNumber == roomNumber)
        {
            if(reservations[i].status == 0 ||
               reservations[i].status == 1)     // checking if the room is already occupied 
            {
                return 0;
            }
        }
    }

    return 1;
}


void checkInGuest()
{
    int id;
    int i;
    int found = 0;

    printf("\nEnter Reservation ID: ");
    scanf("%d", &id);

    for(i = 0; i < count; i++)
    {
        if(reservations[i].reservationID == id)
        {
            found = 1;
        // Update room state from RESERVED (0) to CHECKED IN (1)
            if(reservations[i].status == 0)
            {
                reservations[i].status = 1;

                printf("\nCheck-In Successful!\n");

                printf("Guest Name: %s\n",
                       reservations[i].guestName);

                printf("Room Number: %d\n",
                       reservations[i].roomNumber);
                saveToFile(); // save so the check-in is not lost
            }
            else if(reservations[i].status == 1)
            {
                printf("\nGuest is already Checked In!\n");
            }
            else
            {
                printf("\nGuest has already Checked Out!\n");
            }

            break;
        }
    }

    if(found == 0)
    {
        printf("\nReservation Not Found!\n");
    }
}

void checkOutGuest()
{
    int id;
    int i;
    int found = 0;

    printf("\nEnter Reservation ID: ");
    scanf("%d", &id);

    for(i = 0; i < count; i++) // loop till the reservations 
    {
        if(reservations[i].reservationID == id) // if the id is found 
        {
            found = 1;

            if(reservations[i].status == 1)
            {
                reservations[i].status = 2; // changing the status to checkedout 

                printf("\nCheck-Out Successful!\n");

                printf("Guest Name: %s\n",
                       reservations[i].guestName);

                printf("Room Number: %d\n",
                       reservations[i].roomNumber);

                printf("Room is now Available!\n");
                saveToFile(); // save so the check-out is not lost
            }
            else if(reservations[i].status == 0)
            {
                printf("\nGuest has not Checked In yet!\n");
            }
            else
            {
                printf("\nGuest has already Checked Out!\n");
            }

            break;
        }
    }

    if(found == 0)
    {
        printf("\nReservation Not Found!\n");
    }
}

// saving to the file so we dont lose the previous record 

void saveToFile()
{
    FILE *fp;
    int i;

    fp = fopen("reservations.txt", "w");

    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        fprintf(fp, "%d|%s|%s|%d|%s|%d|%.2f|%.2f|%d\n",
            reservations[i].reservationID,
            reservations[i].guestName,
            reservations[i].phone,
            reservations[i].roomNumber,
            reservations[i].roomType,
            reservations[i].numberOfDays,
            reservations[i].pricePerNight,
            reservations[i].totalBill,
            reservations[i].status);
    }

    fclose(fp);

    printf("Reservations saved successfully!\n");
}

void loadFromFile()
{
    FILE *fp;
    int i;

    fp = fopen("reservations.txt", "r");

    if (fp == NULL)
    {
        count = 0;
        printf("No previous records found.\n");
        return;
    }

    count = 0;

    while (count < MAX &&
        fscanf(fp,
        " %d|%49[^|]|%19[^|]|%d|%19[^|]|%d|%f|%f|%d",
        &reservations[count].reservationID,
        reservations[count].guestName,
        reservations[count].phone,
        &reservations[count].roomNumber,
        reservations[count].roomType,
        &reservations[count].numberOfDays,
        &reservations[count].pricePerNight,
        &reservations[count].totalBill,
        &reservations[count].status) == 9)
    {
        count++;
    }

    fclose(fp);

    // set nextID to highest ID + 1 so new guests never get an old guest's ID
    nextID = 1;
    for(i = 0; i < count; i++)
    {
        if(reservations[i].reservationID >= nextID)
        {
            nextID = reservations[i].reservationID + 1;
        }
    }

    printf("Reservations loaded successfully!\n");
    printf("Total records: %d\n", count);
}

void generateReport()
{
    int i;

    int reserved = 0;
    int checkedIn = 0;
    int checkedOut = 0;

    float totalRevenue = 0;

    for (i = 0; i < count; i++)
    {
        if (reservations[i].status == 0)
        {
            reserved++;
        }
        else if (reservations[i].status == 1)
        {
            checkedIn++;
        }
        else if (reservations[i].status == 2)
        {
            checkedOut++;
        }
        // calculating the total revenue generated by the hotel 
        totalRevenue += reservations[i].totalBill;
    }

    printf("\n====================================\n");
    printf("          HOTEL REPORT\n");
    printf("====================================\n");

    printf("Total Reservations: %d\n", count);

    printf("Reserved Guests: %d\n", reserved);

    printf("Checked-in Guests: %d\n", checkedIn);

    printf("Checked-out Guests: %d\n", checkedOut);

    printf("------------------------------------\n");

    printf("Total Booking Value: Rs. %.2f\n",
           totalRevenue);

    printf("====================================\n");
    }
