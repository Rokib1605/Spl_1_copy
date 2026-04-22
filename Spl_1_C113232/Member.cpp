#include "Member.h"
using namespace std;


Member::Member() {
    memberID = 0;
    strcpy(name, "");
    strcpy(email, "");
    strcpy(phone, "");
    booksBorrowed = 0;
    historyCount = 0;
    fineAmount = 0.0;

    for (int i = 0; i < 20; i++) {
        borrowHistory[i] = 0;
    }
}


void Member::display() {
    cout << "\n--- Member Details ---\n";
    cout << "ID: " << memberID << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phone << endl;
    cout << "Books Borrowed: " << booksBorrowed << endl;
    cout << "Fine Amount: Rs." << fineAmount << endl;
}


bool Member::canBorrow() {
    return booksBorrowed < 5;
}


void Member::addToHistory(int bookID) {
    if (historyCount < 20) {
        borrowHistory[historyCount] = bookID;
        historyCount++;
    }
}

