#include "LibrarySystem.h"

//  CONSTRUCTOR 
LibrarySystem::LibrarySystem() {
    memberCount = 0;
    loanCount = 0;
}

//   SIMILARITY 
int LibrarySystem::calculateStringSimilarity(const char* str1, const char* str2) {
    int matches = 0;
    int len1 = (int)strlen(str1);
    int len2 = (int)strlen(str2);

    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            if (tolower((unsigned char)str1[i]) == tolower((unsigned char)str2[j])) {
                matches++;
                break;
            }
        }
    }

    int maxLen = (len1 > len2) ? len1 : len2;
    if (maxLen == 0) return 0;

    return (matches * 100) / maxLen;
}

//  BOOK OPERATIONS 
void LibrarySystem::addBook() {
    Book newBook;
    cout << "\n=== Add New Book ===\n";
    cout << "Enter Book ID (0-" << (MAX_BOOK_ID - 1) << "): ";
    cin >> newBook.bookID;

    if (!cin || newBook.bookID < 0 || newBook.bookID >= MAX_BOOK_ID) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n✗ Invalid Book ID! Must be between 0 and " << (MAX_BOOK_ID - 1) << ".\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Title: ";
    cin.getline(newBook.title, 100);

    cout << "Enter Author: ";
    cin.getline(newBook.author, 50);

    cout << "Enter ISBN: ";
    cin.getline(newBook.ISBN, 20);

    cout << "Enter Genre: ";
    cin.getline(newBook.genre, 30);

    cout << "Enter Total Copies: ";
    cin >> newBook.totalCopies;

    if (!cin || newBook.totalCopies < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n✗ Invalid total copies.\n";
        return;
    }

    newBook.availableCopies = newBook.totalCopies;
    newBook.timesIssued = 0;

    bookTree.insert(newBook);
    cout << "\n✓ Book added successfully!\n";
}

void LibrarySystem::searchBook() {
    cout << "\n=== Search Book ===\n";
    cout << "Search by:\n";
    cout << "1. Book ID\n";
    cout << "2. Title\n";
    cout << "3. Author\n";
    cout << "4. ISBN\n";
    cout << "5. Genre\n";
    cout << "6. Show All Books\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 6) {
        displayAllBooks();
        return;
    }

    if (choice == 1) {
        int id;
        cout << "\nEnter Book ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Book* book = bookTree.search(id);
        if (book != NULL) {
            cout << "\n✓ Book Found:\n";
            displayBookTable(book, 1);
        } else {
            cout << "\n✗ Book not found!\n";
            cout << "\nShowing all available books:\n";
            displayAllBooks();
        }
        return;
    }

    char searchTerm[100];

    switch (choice) {
        case 2: cout << "\nEnter Title (or part of it): "; break;
        case 3: cout << "\nEnter Author (or part of it): "; break;
        case 4: cout << "\nEnter ISBN: "; break;
        case 5: cout << "\nEnter Genre: "; break;
        default:
            cout << "Invalid choice!\n";
            return;
    }

    cin.getline(searchTerm, 100);

    for (int i = 0; searchTerm[i]; i++) {
        searchTerm[i] = (char)tolower((unsigned char)searchTerm[i]);
    }

    searchByField(searchTerm, choice);
}

void LibrarySystem::searchByField(char* searchTerm, int field) {
    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    Book exactMatches[MAX_BOOKS_ARR];
    int exactCount = 0;

    Book nearMatches[MAX_BOOKS_ARR];
    int nearCount = 0;

    for (int i = 0; i < count; i++) {
        char fieldValue[100] = {0};

        switch (field) {
            case 2: strcpy(fieldValue, bookArray[i].title);  break;
            case 3: strcpy(fieldValue, bookArray[i].author); break;
            case 4: strcpy(fieldValue, bookArray[i].ISBN);   break;
            case 5: strcpy(fieldValue, bookArray[i].genre);  break;
            default: return;
        }

        // lowercase copy
        char lower[100];
        strcpy(lower, fieldValue);
        for (int j = 0; lower[j]; j++)
            lower[j] = (char)tolower((unsigned char)lower[j]);

        // 1) Exact substring match in whole text
        if (strstr(lower, searchTerm)) {
            exactMatches[exactCount++] = bookArray[i];
            continue;
        }

        // 2) Near match: compare with EACH WORD 
        bool added = false;
        char temp[100];
        strcpy(temp, lower);

        const char* delim = " ,.-_:/()[]{}!?\"";
        char* token = strtok(temp, delim);

        while (token != NULL) {
            int len1 = (int)strlen(searchTerm);
            int len2 = (int)strlen(token);

            int diff = abs(len1 - len2);

            // if close in length, check similarity
            if (diff <= 2) {
                int sim = calculateStringSimilarity(searchTerm, token);

                
               if (sim >= 60 && token[0] == searchTerm[0]) {
    nearMatches[nearCount++] = bookArray[i];
    break;
}

            }

            token = strtok(NULL, delim);
        }

        (void)added;
    }

    if (exactCount > 0) {
        cout << "\n✓ Found " << exactCount << " exact match(es):\n";
        displayBookTable(exactMatches, exactCount);
    }

    if (nearCount > 0) {
        cout << "\n💡 Did you mean:\n";
        displayBookTable(nearMatches, nearCount);
    }

    if (exactCount == 0 && nearCount == 0) {
        cout << "\n✗ No books found matching \"" << searchTerm << "\"\n";
        cout << "\nShowing all books instead:\n";
        displayAllBooks();
    }
}
void LibrarySystem::displayBookTable(Book* books, int count) {
    cout << "\n";
    cout << "================================================================================================\n";
    cout << "| ID  | Title                           | Author              | Genre      | Avail | Status    |\n";
    cout << "================================================================================================\n";

    for (int i = 0; i < count; i++) {
        char status[15];
        if (books[i].availableCopies == 0) strcpy(status, "OUT OF STOCK");
        else if (books[i].availableCopies == books[i].totalCopies) strcpy(status, "AVAILABLE");
        else strcpy(status, "PARTIALLY");

        // Short author
        char author[21];
        if (strlen(books[i].author) > 20) {
            strncpy(author, books[i].author, 17);
            author[17] = '.'; author[18] = '.'; author[19] = '.';
            author[20] = '\0';
        } else strcpy(author, books[i].author);

        // Short genre
        char genre[11];
        if (strlen(books[i].genre) > 10) {
            strncpy(genre, books[i].genre, 7);
            genre[7] = '.'; genre[8] = '.'; genre[9] = '.';
            genre[10] = '\0';
        } else strcpy(genre, books[i].genre);

        int titleLen = (int)strlen(books[i].title);

        if (titleLen <= 31) {
            printf("| %-3d | %-31s | %-19s | %-10s | %d/%-3d | %-11s |\n",
                   books[i].bookID, books[i].title, author, genre,
                   books[i].availableCopies, books[i].totalCopies, status);
        } else {
            char line1[32], line2[32];
            int splitPos = 31;

            for (int j = 30; j > 20; j--) {
                if (books[i].title[j] == ' ') { splitPos = j; break; }
            }

            strncpy(line1, books[i].title, splitPos);
            line1[splitPos] = '\0';

            strcpy(line2, books[i].title + splitPos);
            if (line2[0] == ' ') strcpy(line2, line2 + 1);

            printf("| %-3d | %-31s |                     |            |       |           |\n",
                   books[i].bookID, line1);

            printf("|     | %-31s | %-19s | %-10s | %d/%-3d | %-11s |\n",
                   line2, author, genre,
                   books[i].availableCopies, books[i].totalCopies, status);
        }
    }

    cout << "================================================================================================\n";
}

void LibrarySystem::displayAllBooks() {
    cout << "\n=== Library Book Catalog ===\n";

    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    if (count == 0) {
        cout << "\n✗ No books in library!\n";
        return;
    }

    cout << "\nTotal Books in Library: " << count << "\n";
    displayBookTable(bookArray, count);

    int totalCopies = 0;
    int availableCopies = 0;

    for (int i = 0; i < count; i++) {
        totalCopies += bookArray[i].totalCopies;
        availableCopies += bookArray[i].availableCopies;
    }

    cout << "\n📊 Library Statistics:\n";
    cout << "   Total Book Titles: " << count << "\n";
    cout << "   Total Copies: " << totalCopies << "\n";
    cout << "   Available: " << availableCopies << "\n";
    cout << "   Issued: " << (totalCopies - availableCopies) << "\n";
}

//  MEMBER OPERATIONS 
void LibrarySystem::addMember() {
    if (memberCount >= MAX_MEMBERS) {
        cout << "\n✗ Member limit reached!\n";
        return;
    }

    Member newMember;
    cout << "\n=== Register New Member ===\n";
    cout << "Enter Member ID: ";
    cin >> newMember.memberID;

    if (!cin) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n✗ Invalid Member ID!\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Name: ";
    cin.getline(newMember.name, 50);

    cout << "Enter Email: ";
    cin.getline(newMember.email, 50);

    cout << "Enter Phone: ";
    cin.getline(newMember.phone, 15);

    newMember.booksBorrowed = 0;
    newMember.fineAmount = 0.0;
    newMember.historyCount = 0;

    members[memberCount++] = newMember;

    cout << "\n✓ Member registered successfully!\n";
    saveMembers();
}

Member* LibrarySystem::searchMember(int memberID) {
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID == memberID) return &members[i];
    }
    return NULL;
}

void LibrarySystem::displayAllMembers() {
    cout << "\n=== Registered Members ===\n";

    if (memberCount == 0) {
        cout << "\n✗ No members registered!\n";
        return;
    }

    cout << "\nTotal Members: " << memberCount << "\n\n";
    cout << "============================================================================================\n";
    cout << "| ID   | Name                      | Email                     | Books | Fine (Rs.) | Status |\n";
    cout << "============================================================================================\n";

    for (int i = 0; i < memberCount; i++) {
        char name[26], email[26];
        if (strlen(members[i].name) > 25) {
            strncpy(name, members[i].name, 22);
            name[22] = '.'; name[23] = '.'; name[24] = '.';
            name[25] = '\0';
        } else strcpy(name, members[i].name);

        if (strlen(members[i].email) > 25) {
            strncpy(email, members[i].email, 22);
            email[22] = '.'; email[23] = '.'; email[24] = '.';
            email[25] = '\0';
        } else strcpy(email, members[i].email);

        char status[10];
        if (members[i].booksBorrowed >= 5) strcpy(status, "LIMIT");
        else if (members[i].fineAmount > 0) strcpy(status, "FINE DUE");
        else strcpy(status, "ACTIVE");

        printf("| %-4d | %-25s | %-25s | %d/5   | %-10.2f | %-6s |\n",
               members[i].memberID, name, email,
               members[i].booksBorrowed, members[i].fineAmount, status);
    }

    cout << "============================================================================================\n";
}

void LibrarySystem::loadMembers() {
    ifstream file("members.txt");
    if (!file) return;

    memberCount = 0;

    Member m;
    char line[400];

    while (file.getline(line, 400) && memberCount < MAX_MEMBERS) {
        int ok = sscanf(line, "%d|%49[^|]|%49[^|]|%14[^|]|%d|%lf",
                        &m.memberID, m.name, m.email, m.phone,
                        &m.booksBorrowed, &m.fineAmount);

        if (ok == 6) {
            m.historyCount = 0;
            members[memberCount++] = m;
        }
    }

    file.close();
}

//  ISSUE/RETURN OPERATIONS 
void LibrarySystem::issueBook() {
    int bookID, memberID;
    cout << "\n=== Issue Book ===\n";
    cout << "Enter Book ID: ";
    cin >> bookID;
    cout << "Enter Member ID: ";
    cin >> memberID;

    if (!cin) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n✗ Invalid input!\n";
        return;
    }

    if (bookID < 0 || bookID >= MAX_BOOK_ID) {
        cout << "\n✗ Invalid Book ID (0-" << (MAX_BOOK_ID - 1) << ")!\n";
        return;
    }

    Book* book = bookTree.search(bookID);
    Member* member = searchMember(memberID);

    if (book == NULL) {
        cout << "\n✗ Book not found!\n";
        return;
    }

    if (member == NULL) {
        cout << "\n✗ Member not found!\n";
        return;
    }

    if (member->fineAmount > 0) {
        cout << "\n⚠ Warning: Member has outstanding fine of Rs." << member->fineAmount << endl;
        cout << "Clear fine before issuing? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            member->fineAmount = 0;
            cout << "✓ Fine cleared.\n";
        } else {
            cout << "✗ Cannot issue book with outstanding fine.\n";
            return;
        }
    }

    if (!member->canBorrow()) {
        cout << "\n✗ Member has reached borrow limit (5 books)!\n";
        return;
    }

    if (book->availableCopies <= 0) {
        cout << "\n✗ Book not available!\n";
        cout << "   Title: " << book->title << endl;
        cout << "   All " << book->totalCopies << " copies are currently issued.\n";
        cout << "\nWould you like to reserve this book? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            reservations[bookID].enqueue(memberID);
            cout << "✓ Book reserved! You'll be notified when available.\n";
        }
        return;
    }

    if (loanCount >= MAX_LOANS) {
        cout << "\n✗ Loan limit reached!\n";
        return;
    }

    book->issueBook();
    member->booksBorrowed++;
    member->addToHistory(bookID);

    Loan newLoan;
    newLoan.loanID = loanCount + 1;
    newLoan.bookID = bookID;
    newLoan.memberID = memberID;
    strcpy(newLoan.issueDate, getCurrentDate());
    strcpy(newLoan.dueDate, getDueDate());
    newLoan.isReturned = false;
    newLoan.fine = 0.0;

    loans[loanCount++] = newLoan;

    cout << "\n✓ Book issued successfully!\n";
    cout << "═══════════════════════════════════════\n";
    cout << "   Loan ID: " << newLoan.loanID << endl;
    cout << "   Book: " << book->title << endl;
    cout << "   Member: " << member->name << endl;
    cout << "   Issue Date: " << newLoan.issueDate << endl;
    cout << "   Due Date: " << newLoan.dueDate << endl;
    cout << "═══════════════════════════════════════\n";
    cout << "⚠ Please return by due date to avoid fine (Rs.5/day)\n";
}

char* LibrarySystem::getCurrentDate() {
    static char date[11];
    strcpy(date, "13-01-2026"); 
    return date;
}

char* LibrarySystem::getDueDate() {
    static char date[11];
    strcpy(date, "27-01-2026"); 
    return date;
}

void LibrarySystem::returnBook() {
    int loanID;
    cout << "\n=== Return Book ===\n";
    cout << "Enter Loan ID: ";
    cin >> loanID;

    if (!cin) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n✗ Invalid loan ID!\n";
        return;
    }

    Loan* loan = NULL;
    for (int i = 0; i < loanCount; i++) {
        if (loans[i].loanID == loanID && !loans[i].isReturned) {
            loan = &loans[i];
            break;
        }
    }

    if (loan == NULL) {
        cout << "\n✗ Loan not found or already returned!\n";
        return;
    }

    Book* book = bookTree.search(loan->bookID);
    Member* member = searchMember(loan->memberID);

    if (book == NULL || member == NULL) {
        cout << "\n✗ Error: Book or Member not found!\n";
        return;
    }

    book->returnBook();
    member->booksBorrowed--;
    loan->isReturned = true;
    strcpy(loan->returnDate, getCurrentDate());

    cout << "Enter days late (0 if on time): ";
    int daysLate;
    cin >> daysLate;

    if (daysLate > 0) {
        loan->calculateFine(daysLate);
        member->fineAmount += loan->fine;
        cout << "\nFine imposed: Rs." << loan->fine << endl;
    }

    cout << "\n✓ Book returned successfully!\n";

    if (loan->bookID >= 0 && loan->bookID < MAX_BOOK_ID && !reservations[loan->bookID].isEmpty()) {
        int nextMember = reservations[loan->bookID].peek();
        cout << "\nNotify Member " << nextMember << " - Book is now available!\n";
    }
}

//  REPORTS 
void LibrarySystem::displayActiveLoans() {
    cout << "\n=== Active Loans ===\n";

    bool found = false;
    int activeCount = 0;

    cout << "================================================================================================================\n";
    cout << "| Loan ID | Book ID | Book Title                      | Member Name              | Issue Date | Due Date   |\n";
    cout << "================================================================================================================\n";

    for (int i = 0; i < loanCount; i++) {
        if (!loans[i].isReturned) {
            Book* book = bookTree.search(loans[i].bookID);
            Member* member = searchMember(loans[i].memberID);

            if (book && member) {
                char name[26];
                if (strlen(member->name) > 25) {
                    strncpy(name, member->name, 22);
                    name[22] = '.'; name[23] = '.'; name[24] = '.';
                    name[25] = '\0';
                } else strcpy(name, member->name);

                int titleLen = (int)strlen(book->title);

                if (titleLen <= 31) {
                    printf("| %-7d | %-7d | %-31s | %-24s | %-10s | %-10s |\n",
                           loans[i].loanID, loans[i].bookID, book->title, name,
                           loans[i].issueDate, loans[i].dueDate);
                } else {
                    char line1[32], line2[32];
                    int splitPos = 31;

                    for (int j = 30; j > 20; j--) {
                        if (book->title[j] == ' ') { splitPos = j; break; }
                    }

                    strncpy(line1, book->title, splitPos);
                    line1[splitPos] = '\0';

                    strcpy(line2, book->title + splitPos);
                    if (line2[0] == ' ') strcpy(line2, line2 + 1);

                    printf("| %-7d | %-7d | %-31s |                          |            |            |\n",
                           loans[i].loanID, loans[i].bookID, line1);

                    printf("|         |         | %-31s | %-24s | %-10s | %-10s |\n",
                           line2, name, loans[i].issueDate, loans[i].dueDate);
                }

                found = true;
                activeCount++;
            }
        }
    }

    cout << "================================================================================================================\n";

    if (!found) cout << "\n✓ No active loans! All books returned.\n";
    else cout << "\nTotal Active Loans: " << activeCount << "\n";
}

void LibrarySystem::displayOverdueBooks() {
    cout << "\n=== Overdue Books ===\n";
    cout << "(Simplified: showing all active loans)\n";

    bool found = false;
    for (int i = 0; i < loanCount; i++) {
        if (!loans[i].isReturned) {
            loans[i].display();
            found = true;
        }
    }
    if (!found) cout << "No overdue books!\n";
}

void LibrarySystem::displayTopIssuedBooks() {
    cout << "\n=== Top 5 Most Issued Books ===\n";

    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (bookArray[j].timesIssued < bookArray[j + 1].timesIssued) {
                Book temp = bookArray[j];
                bookArray[j] = bookArray[j + 1];
                bookArray[j + 1] = temp;
            }
        }
    }

    int limit = (count < 5) ? count : 5;
    for (int i = 0; i < limit; i++) bookArray[i].display();
}

// FILE OPERATIONS 
void LibrarySystem::saveBooks() {
    ofstream file("books.txt");
    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    for (int i = 0; i < count; i++) {
        file << bookArray[i].bookID << "|"
             << bookArray[i].title << "|"
             << bookArray[i].author << "|"
             << bookArray[i].ISBN << "|"
             << bookArray[i].genre << "|"
             << bookArray[i].totalCopies << "|"
             << bookArray[i].availableCopies << "|"
             << bookArray[i].timesIssued << "\n";
    }
    file.close();
    cout << "\nBooks saved to file!\n";
}

void LibrarySystem::loadBooks() {
    ifstream file("books.txt");
    if (!file) return;

    Book book;
    char line[500];

    while (file.getline(line, 500)) {
        int ok = sscanf(line, "%d|%99[^|]|%49[^|]|%19[^|]|%29[^|]|%d|%d|%d",
                        &book.bookID, book.title, book.author, book.ISBN,
                        book.genre, &book.totalCopies, &book.availableCopies,
                        &book.timesIssued);

        if (ok == 8) bookTree.insert(book);
    }

    file.close();
}

void LibrarySystem::saveMembers() {
    ofstream file("members.txt");
    for (int i = 0; i < memberCount; i++) {
        file << members[i].memberID << "|"
             << members[i].name << "|"
             << members[i].email << "|"
             << members[i].phone << "|"
             << members[i].booksBorrowed << "|"
             << members[i].fineAmount << "\n";
    }
    file.close();
    cout << "\nMembers saved to file!\n";
}

//  RECOMMENDATION SYSTEM 
void LibrarySystem::getRecommendations() {
    int memberID;
    cout << "\n=== Get Book Recommendations ===\n";
    cout << "Enter Member ID: ";
    cin >> memberID;

    Member* member = searchMember(memberID);
    if (member == NULL) {
        cout << "\nMember not found!\n";
        return;
    }

    if (member->historyCount == 0) {
        cout << "\nMember has no borrowing history!\n";
        cout << "Try borrowing some books first.\n";
        return;
    }

    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    int lastBookID = member->borrowHistory[member->historyCount - 1];

    BookRecommendation recommender;
    recommender.recommend(bookArray, count, lastBookID);
}

void LibrarySystem::clusterAllBooks() {
    cout << "\n=== Book Clustering ===\n";

    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    if (count == 0) {
        cout << "\nNo books in library!\n";
        return;
    }

    BookRecommendation recommender;
    recommender.clusterByGenre(bookArray, count);
}

void LibrarySystem::findSimilarBooks() {
    int bookID;
    cout << "\nEnter Book ID to find similar books: ";
    cin >> bookID;

    Book* targetBook = bookTree.search(bookID);
    if (targetBook == NULL) {
        cout << "\n✗ Book not found!\n";
        return;
    }

    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    cout << "\n=== Books Similar to: " << targetBook->title << " ===\n";
    Book similarBooks[MAX_BOOKS_ARR];
    int similarCount = 0;

    for (int i = 0; i < count; i++) {
        if (bookArray[i].bookID != bookID &&
            strcmp(bookArray[i].genre, targetBook->genre) == 0) {
            similarBooks[similarCount++] = bookArray[i];
        }
    }

    if (similarCount == 0) {
        cout << "\n✗ No similar books found in the same genre.\n";
    } else {
        cout << "\nFound " << similarCount << " book(s) in the same genre:\n";
        displayBookTable(similarBooks, similarCount);
    }
}

void LibrarySystem::recommendByGenre() {
    char genre[30];
    cout << "\nEnter preferred genre: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(genre, 30);

    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    cout << "\n=== Books in Genre: " << genre << " ===\n";

    Book results[MAX_BOOKS_ARR];
    int rCount = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(bookArray[i].genre, genre) == 0) {
            results[rCount++] = bookArray[i];
        }
    }

    if (rCount == 0) cout << "\n✗ No books found in this genre.\n";
    else displayBookTable(results, rCount);
}

//  EXTRA FEATURES 
void LibrarySystem::browseByCategory() {
    cout << "\n=== Browse by Category ===\n";

    Book bookArray[MAX_BOOKS_ARR];
    int count = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    if (count == 0) {
        cout << "\n✗ No books in library!\n";
        return;
    }

    char genres[20][30];
    int genreCount = 0;

    for (int i = 0; i < count; i++) {
        bool found = false;
        for (int j = 0; j < genreCount; j++) {
            if (strcmp(bookArray[i].genre, genres[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found && genreCount < 20) {
            strcpy(genres[genreCount++], bookArray[i].genre);
        }
    }

    cout << "\nAvailable Categories:\n";
    for (int i = 0; i < genreCount; i++) {
        int bookCount = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(bookArray[j].genre, genres[i]) == 0) bookCount++;
        }
        cout << (i + 1) << ". " << genres[i] << " (" << bookCount << " books)\n";
    }

    cout << "\nSelect category (1-" << genreCount << "): ";
    int choice;
    cin >> choice;

    if (choice < 1 || choice > genreCount) {
        cout << "Invalid choice!\n";
        return;
    }

    cout << "\n=== Books in " << genres[choice - 1] << " ===\n";

    Book categoryBooks[MAX_BOOKS_ARR];
    int catCount = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(bookArray[i].genre, genres[choice - 1]) == 0) {
            categoryBooks[catCount++] = bookArray[i];
        }
    }

    displayBookTable(categoryBooks, catCount);
}

void LibrarySystem::viewMemberHistory() {
    int id;
    cout << "\n=== Member Borrowing History ===\nEnter Member ID: " << flush;
    if (!(cin >> id)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }

    Member* m = searchMember(id);
    if (m == NULL) {
        cout << "\n✗ Member not found!\n" << flush;
        return;
    }

    cout << "\n📚 Borrowing History for: " << m->name << "\n";
    cout << "══════════════════════════════════════════════\n";

    bool found = false;
    
    // IMPORTANT: We scan the global 'loanCount', NOT the member's internal history!
    for (int i = 0; i < loanCount; i++) {
        if (loans[i].memberID == id) {
            loans[i].display(); // Print the matching loan details
            found = true;
        }
    }

    if (!found) {
        cout << "✗ No borrowing history.\n" << flush;
    }
}

void LibrarySystem::showDashboard() {
    cout << "\n╔══════════════════════════════════════════════╗\n";
    cout << "║         LIBRARY DASHBOARD                    ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";

    Book bookArray[MAX_BOOKS_ARR];
    int bookCount = bookTree.toArray(bookArray, MAX_BOOKS_ARR);

    int totalCopies = 0;
    int availableCopies = 0;
    int totalIssued = 0;

    for (int i = 0; i < bookCount; i++) {
        totalCopies += bookArray[i].totalCopies;
        availableCopies += bookArray[i].availableCopies;
        totalIssued += bookArray[i].timesIssued;
    }

    int activeLoans = 0;
    for (int i = 0; i < loanCount; i++) {
        if (!loans[i].isReturned) activeLoans++;
    }

    double totalFines = 0;
    for (int i = 0; i < memberCount; i++) totalFines += members[i].fineAmount;

    cout << "\n📊 LIBRARY STATISTICS\n";
    cout << "════════════════════════════════════════════\n";
    cout << "Books:\n";
    cout << "  • Total Titles: " << bookCount << "\n";
    cout << "  • Total Copies: " << totalCopies << "\n";
    cout << "  • Available Now: " << availableCopies << "\n";
    cout << "  • Currently Issued: " << (totalCopies - availableCopies) << "\n";
    cout << "  • Total Issues (All Time): " << totalIssued << "\n";

    cout << "\n👥 Members:\n";
    cout << "  • Total Registered: " << memberCount << "\n";
    cout << "  • Active Borrowers: " << activeLoans << "\n";

    cout << "\n💰 Financials:\n";
    cout << "  • Outstanding Fines: Rs." << totalFines << "\n";

    cout << "\n📋 Loans:\n";
    cout << "  • Active Loans: " << activeLoans << "\n";
    cout << "  • Total Transactions: " << loanCount << "\n";

    cout << "════════════════════════════════════════════\n";
}
void LibrarySystem::deleteMember() {
    int id;
    cout << "\n=== Delete Member ===\nEnter Member ID to delete: " << flush;
    if (!(cin >> id)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }

    int index = -1;
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID == id) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        if (members[index].booksBorrowed > 0 || members[index].fineAmount > 0) {
            cout << "[-] Cannot delete! Member currently has active loans or unpaid fines.\n" << flush;
            return;
        }
        for (int i = index; i < memberCount - 1; i++) {
            members[i] = members[i+1];
        }
        memberCount--;
        cout << "[+] Member deleted successfully!\n" << flush;
    } else {
        cout << "[-] Member not found!\n" << flush;
    }
}

void LibrarySystem::deleteBook() {
    int id;
    cout << "\n=== Delete Book ===\nEnter Book ID to delete: " << flush;
    if (!(cin >> id)) { 
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        return; 
    }

    if (bookTree.remove(id)) {
        cout << "[+] Book ID " << id << " deleted successfully from the catalog!\n" << flush;
    } else {
        cout << "[-] Book not found!\n" << flush;
    }
}
void LibrarySystem::saveLoans() {
    ofstream outFile("loans.dat", ios::binary);
    if (outFile.is_open()) {
        outFile.write((char*)&loanCount, sizeof(int));
        outFile.write((char*)loans, sizeof(Loan) * loanCount);
        outFile.close();
        cout << "Loans saved to file!\n" << flush;
    }
}

void LibrarySystem::loadLoans() {
    ifstream inFile("loans.dat", ios::binary);
    if (inFile.is_open()) {
        inFile.read((char*)&loanCount, sizeof(int));
        inFile.read((char*)loans, sizeof(Loan) * loanCount);
        inFile.close();
    } else {
        loanCount = 0; 
    }
}