#ifndef LOAN_H
#define LOAN_H

#include <iostream>
#include <cstring>

class Loan {
public:
    int loanID;
    int bookID;
    int memberID;
    char issueDate[11];
    char dueDate[11];
    char returnDate[11];
    bool isReturned;
    double fine;

    
    Loan();

    // Member functions
    void display();
    void calculateFine(int daysLate);
};

#endif
