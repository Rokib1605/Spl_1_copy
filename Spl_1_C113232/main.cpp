#include <iostream>
#include <limits>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "LibrarySystem.h"

using namespace std;

#define PORT 8080
#define ADMIN_PASSWORD "admin123"

static void displayMemberMenu() {
    cout << "\n╔══════════════════════════════════════════════╗\n";
    cout << "║          LIBRARY SYSTEM (MEMBER)             ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << "\n📚 BOOKS & TRANSACTIONS\n";
    cout << "  1. Search Book\n";
    cout << "  2. Display All Books\n";
    cout << "  3. View My Borrowing History\n";
    cout << "  4. Issue a Book\n";
    cout << "  5. Return a Book\n";
    
    cout << "\n🤖 DISCOVER\n";
    cout << "  6. Get Recommendations\n";
    cout << "  7. Find Similar Books\n";
    cout << "  8. Recommend By Genre\n";
    cout << "  9. Browse By Category\n";
    
    cout << "\n  0. Disconnect\n";
    cout << "\nEnter choice: " << flush; 
}

static void displayAdminMenu() {
    cout << "\n╔══════════════════════════════════════════════╗\n";
    cout << "║        LIBRARY SYSTEM (ADMINISTRATOR)        ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << "\n📚 BOOK MANAGEMENT\n";
    cout << "  1. Add Book\n";
    cout << "  2. Delete Book\n";
    cout << "  3. Search Book\n";
    cout << "  4. Display All Books\n";

    cout << "\n👥 MEMBER MANAGEMENT\n";
    cout << "  5. Register Member\n";
    cout << "  6. Delete Member\n";
    cout << "  7. Display All Members\n";
    cout << "  8. View Member History\n";

    cout << "\n📋 TRANSACTIONS\n";
    cout << "  9. Issue Book\n";
    cout << " 10. Return Book\n";
    cout << " 11. Display Active Loans\n";
    cout << " 12. Display Overdue Books\n";

    cout << "\n🤖 RECOMMENDATION / CLUSTERING\n";
    cout << " 13. Get Recommendations\n";
    cout << " 14. Cluster All Books\n";
    cout << " 15. Find Similar Books\n";
    cout << " 16. Recommend By Genre\n";
    cout << " 17. Browse By Category\n";

    cout << "\n📊 SYSTEM\n";
    cout << " 18. Show Dashboard\n";
    cout << " 19. Save & Update Server\n";

    cout << "\n  0. Disconnect\n";
    cout << "\nEnter choice: " << flush; 
}

void handleClient() {
    LibrarySystem system;
    system.loadBooks();
    system.loadMembers();
    system.loadLoans(); // <-- ADDED: Load history on startup
    int choice;
    bool isAdmin = false;

    cout << "\n=== WELCOME TO THE LIBRARY NETWORK ===\n";
    cout << "1. Login as Member\n";
    cout << "2. Login as Administrator\n";
    cout << "Choice: " << flush;
    
    int loginChoice;
    if (!(cin >> loginChoice)) return;
    
    if (loginChoice == 2) {
        string pass;
        cout << "Enter Admin Password: " << flush;
        cin >> pass;
        if (pass == ADMIN_PASSWORD) {
            isAdmin = true;
            cout << "\n[+] Admin Access Granted.\n" << flush;
        } else {
            cout << "\n[-] Incorrect Password. Disconnecting.\n" << flush;
            return; 
        }
    } else if (loginChoice == 1) {
        cout << "\n[+] Logged in as Member.\n" << flush;
    } else {
        cout << "\n[-] Invalid choice. Disconnecting.\n" << flush;
        return;
    }

    while (true) {
        if (isAdmin) {
            displayAdminMenu();
            if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1:  system.addBook(); system.saveBooks(); break;
                case 2:  system.deleteBook(); system.saveBooks(); break;
                case 3:  system.loadBooks(); system.searchBook(); break;
                case 4:  system.loadBooks(); system.displayAllBooks(); break;
                
                case 5:  system.addMember(); system.saveMembers(); break;
                case 6:  system.deleteMember(); system.saveMembers(); break;
                case 7:  system.loadMembers(); system.displayAllMembers(); break;
                // ADDED loadLoans() before viewing history
                case 8:  system.loadMembers(); system.loadBooks(); system.loadLoans(); system.viewMemberHistory(); break;
                
                // ADDED saveLoans() after issuing/returning
                case 9:  system.issueBook(); system.saveBooks(); system.saveMembers(); system.saveLoans(); break;
                case 10: system.returnBook(); system.saveBooks(); system.saveMembers(); system.saveLoans(); break;
                // ADDED loadLoans() before viewing active loans
                case 11: system.loadBooks(); system.loadMembers(); system.loadLoans(); system.displayActiveLoans(); break;
                case 12: system.loadBooks(); system.loadMembers(); system.displayOverdueBooks(); break;
                
                case 13: system.loadBooks(); system.getRecommendations(); break;
                case 14: system.loadBooks(); system.clusterAllBooks(); break;
                case 15: system.loadBooks(); system.findSimilarBooks(); break;
                case 16: system.loadBooks(); system.recommendByGenre(); break;
                case 17: system.loadBooks(); system.browseByCategory(); break;
                
                // ADDED loadLoans() to ensure dashboard stats are accurate
                case 18: system.loadBooks(); system.loadMembers(); system.loadLoans(); system.showDashboard(); break;
                // ADDED saveLoans() to manual save
                case 19: system.saveBooks(); system.saveMembers(); system.saveLoans(); cout << "\nData Force Saved!\n"; break;
                
                case 0:  cout << "\nDisconnecting from server...\n" << flush; system.saveBooks(); system.saveMembers(); system.saveLoans(); exit(0);
                default: cout << "\n✗ Invalid choice! Try again.\n" << flush; break;
            }
        } 
        else {
            displayMemberMenu();
            if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: system.loadBooks(); system.searchBook(); break;
                case 2: system.loadBooks(); system.displayAllBooks(); break;
                // ADDED loadLoans() before viewing history
                case 3: system.loadMembers(); system.loadBooks(); system.loadLoans(); system.viewMemberHistory(); break;
                
                // ADDED saveLoans() after issuing/returning
                case 4: system.issueBook(); system.saveBooks(); system.saveMembers(); system.saveLoans(); break;
                case 5: system.returnBook(); system.saveBooks(); system.saveMembers(); system.saveLoans(); break;
                
                case 6: system.loadBooks(); system.getRecommendations(); break;
                case 7: system.loadBooks(); system.findSimilarBooks(); break;
                case 8: system.loadBooks(); system.recommendByGenre(); break;
                case 9: system.loadBooks(); system.browseByCategory(); break;
                
                case 0: cout << "\nDisconnecting from server...\n" << flush; exit(0);
                default: cout << "\n✗ Invalid choice! Try again.\n" << flush; break;
            }
        }
        cout << flush; 
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { perror("Socket failed"); exit(EXIT_FAILURE); }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { perror("setsockopt"); exit(EXIT_FAILURE); }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { perror("Bind failed"); exit(EXIT_FAILURE); }
    if (listen(server_fd, 5) < 0) { perror("Listen failed"); exit(EXIT_FAILURE); }

    std::cout << "[+] Library Server started on port " << PORT << "\n";
    std::cout << "[+] Waiting for connections...\n";

    while (true) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) { perror("Accept failed"); continue; }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(address.sin_addr), client_ip, INET_ADDRSTRLEN);
        std::cout << "[+] New connection established from " << client_ip << "\n";

        pid_t pid = fork();
        
        if (pid == 0) { 
            close(server_fd); 
            dup2(client_fd, STDIN_FILENO);   
            dup2(client_fd, STDOUT_FILENO);  
            dup2(client_fd, STDERR_FILENO);  
            handleClient(); 
        } 
        else if (pid > 0) { close(client_fd); } 
        else { perror("Fork failed"); }
    }
    return 0;
}