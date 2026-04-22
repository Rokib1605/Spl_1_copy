#ifndef MEMBER_H
#define MEMBER_H

#include <iostream>
#include <cstring>

class Member {
public:
    int memberID;
    char name[50];
    char email[50];
    char phone[15];
    int booksBorrowed;

    int borrowHistory[20];
    int historyCount;
    double fineAmount;

    
    Member();

    // Member functions
    void display();
    bool canBorrow();
    void addToHistory(int bookID);
};

#endif
