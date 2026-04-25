# Networked Library Management System

A concurrent Client-Server Library Management System built entirely in C++ from scratch. This project demonstrates system-level programming, operating system concepts, and custom algorithmic data structures without relying on the C++ Standard Template Library (STL).

## Key Features

* **Multi-Client Architecture:** Built using TCP/IP POSIX Sockets. Utilizes the `fork()` system call to handle multiple parallel user connections simultaneously.
* **Role-Based Access Control:** Secure Administrator dashboard (password: admin123) for inventory management, and a restricted Member dashboard for borrowing.
* **Smart Discovery Engine:** Features a fuzzy string-matching algorithm to handle search typos and dynamic genre clustering for intelligent book recommendations.
* **Concurrency & Syncing:** Uses an auto-save and auto-load file I/O system with plain-text ledgers (books.txt, members.txt, loans.txt) to ensure real-time data synchronization across all connected clients.

## Data Structures & Algorithms

All core logic is implemented entirely from scratch:
* **Binary Search Tree (BST):** Achieves O(log n) efficiency for book storage and searches. Features a custom deletion algorithm using Inorder Successor logic.
* **Circular Queue:** Powers the reservation waitlist, enforcing a strict first-come-first-serve policy.
* **Standard Arrays:** Provides fast, indexed access for member profiles and the global transaction ledger.

## Tech Stack

* **Language:** C++ (No STL)
* **Networking:** POSIX Sockets
* **Environment:** Linux / Unix-based systems
* **Compiler:** GCC (g++)

## How to Run

1. **Compile the Server**
Clone the repository and compile the source code:
```bash
git clone [https://github.com/your-username/Library-Management-System.git](https://github.com/your-username/Library-Management-System.git)
cd Library-Management-System
g++ *.cpp -o server_final
