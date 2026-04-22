#include "Loan.h"
using namespace std;


Loan::Loan() {
    loanID = 0;
    bookID = 0;
    memberID = 0;
    strcpy(issueDate, "");
    strcpy(dueDate, "");
    strcpy(returnDate, "");
    isReturned = false;
    fine = 0.0;
}

// Display loan details
void Loan::display() {
    cout << "\n--- Loan Details ---\n";
    cout << "Loan ID: " << loanID << endl;
    cout << "Book ID: " << bookID << endl;
    cout << "Member ID: " << memberID << endl;
    cout << "Issue Date: " << issueDate << endl;
    cout << "Due Date: " << dueDate << endl;

    if (isReturned) {
        cout << "Return Date: " << returnDate << endl;
        cout << "Status: Returned\n";
    } else {
        cout << "Status: Active\n";
    }

    if (fine > 0) {
        cout << "Fine: Rs." << fine << endl;
    }
}

// Calculate fine
void Loan::calculateFine(int daysLate) {
    if (daysLate > 0) {
        fine = daysLate * 5.0;
    }
}
