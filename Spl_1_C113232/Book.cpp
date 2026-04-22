#include "Book.h"


Book::Book() {
    bookID = 0;
    strcpy(title, "");
    strcpy(author, "");
    strcpy(ISBN, "");
    strcpy(genre, "");
    totalCopies = 0;
    availableCopies = 0;
    timesIssued = 0;
}


void Book::display() {
    cout << "\n--- Book Details ---\n";
    cout << "ID: " << bookID << endl;
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
    cout << "ISBN: " << ISBN << endl;
    cout << "Genre: " << genre << endl;
    cout << "Total Copies: " << totalCopies << endl;
    cout << "Available: " << availableCopies << endl;
    cout << "Times Issued: " << timesIssued << endl;
}


bool Book::issueBook() {
    if (availableCopies > 0) {
        availableCopies--;
        timesIssued++;
        return true;
    }
    return false;
}

// Return a book
void Book::returnBook() {
    if (availableCopies < totalCopies) {
        availableCopies++;
    }
}
