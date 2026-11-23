/*
 * validation.h - header file for the validation functions
 * Has: MAX, the Reservation struct, shared variables and the
 *      list (prototypes) of the validation functions.
 * The function code itself is written in validation.c
 * Each validation function returns 1 if the input is correct and 0 if not.
 */

#ifndef VALIDATION_H   // include guard: makes sure this file is read only once
#define VALIDATION_H

#include<stdio.h>
#include<string.h>

#define MAX 100 // Maximum capacity of reservations 
struct Reservation {
        int reservationID; // Unique identifier for the booking
        char guestName[50];
        char phone[20];
        int roomNumber;
        char roomType[20]; // Standard, Deluxe, or Suite
        int numberOfDays;
        float pricePerNight;
        float totalBill;
        int status; // 0 = RESERVED, 1 = CHECKED IN, 2 = CHECKED OUT
    };

// extern = these variables are created in hotel_main.c, validation.c can use them too
extern struct Reservation reservations[MAX];
extern int count;

int isValidRoom(int room);
int isValidName(char name[]);
int isValidPhone(char phone[]);

#endif
