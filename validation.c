/*
 * validation.c - the code of the validation functions
 * (the list of functions is in validation.h)
 */

#include "validation.h"   // our own header file

// 100 series is for standard , 200 for deluxe and 300 for suite
// returns 1 if the room number is correct, 0 if not
int isValidRoom(int room)
{
    if((room >= 101 && room <= 110) ||
       (room >= 201 && room <= 210) ||
       (room >= 301 && room <= 310))
    {
        return 1;
    }

    return 0;
}

// Validating guest name: cannot be empty, only letters and spaces are allowed (no digits or symbols)
// returns 1 if the name is correct, 0 if not
int isValidName(char name[])
{
    int x;

    if(strlen(name) == 0) // length cannot be empty
    {
        printf("Name cannot be empty!\n");
        return 0;
    }

    for(x = 0; name[x] != '\0'; x++) // only letters and spaces are allowed in a name
    {
        if(!((name[x] >= 'A' && name[x] <= 'Z') ||
             (name[x] >= 'a' && name[x] <= 'z') ||
              name[x] == ' '))
        {
            printf("Invalid name! Use letters and spaces only.\n");
            return 0;
        }
    }

    return 1;
}

// Validating phone number: Must be exactly 11 digits and contain only numeric characters
// (same code as before, just moved into a function)
int isValidPhone(char phone[])
{
    int x, correct;

    correct = 1;
    if (strlen(phone) != 11) // phone number should be of 11 digits
    {
        correct = 0;
    }
    else
    {
        for (x = 0; phone[x] != '\0'; x++) // only numeric digits are allowed
        {
            if (phone[x] < '0' || phone[x] > '9')
            {
                correct = 0;
                break;
            }
        }
    }
    if (correct == 0)
    {
        printf("Invalid phone number! Enter 11 digits only.\n");
    }

    // validating that phone numbers cannot be the same

    int y;
    for(y = 0; y < count ; y++ )
    {
        if(reservations[y].status != 2 && strcmp(reservations[y].phone, phone) == 0) // checked out guests can book again their phone number can be used again
        {
            correct = 0;
            printf("\n phone number cannot be same :) ");
            printf("Enter again \n");
            break;
        }
    }

    return correct; // 1 = valid, 0 = not valid
}
