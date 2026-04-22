#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <cstring>
using namespace std;

class Book {
public:
    int bookID;
    char title[100];
    char author[50];
    char ISBN[20];
    char genre[30];
    int totalCopies;
    int availableCopies;
    int timesIssued;

   
    Book();

    // Member functions
    void display();
    bool issueBook();
    void returnBook();
};

#endif
