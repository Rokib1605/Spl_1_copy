#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

class ReservationQueue {
private:
    int queue[50];  
    int front;
    int rear;
    int count;
    int maxSize;
    
public:
    ReservationQueue() {
        front = 0;
        rear = -1;
        count = 0;
        maxSize = 50;
    }
    
    
    bool isFull() {
        return count == maxSize;
    }
    
    
    bool isEmpty() {
        return count == 0;
    }
    
   
    bool enqueue(int memberID) {
        if (isFull()) {
            cout << "\nReservation queue is full!\n";
            return false;
        }
        
        rear = (rear + 1) % maxSize;
        queue[rear] = memberID;
        count++;
        cout << "\nMember " << memberID << " added to reservation queue.\n";
        return true;
    }
    
    
    int dequeue() {
        if (isEmpty()) {
            cout << "\nReservation queue is empty!\n";
            return -1;
        }
        
        int memberID = queue[front];
        front = (front + 1) % maxSize;
        count--;
        return memberID;
    }
    
    
    int peek() {
        if (isEmpty()) {
            return -1;
        }
        return queue[front];
    }
    
    // Display all reservations
    void display() {
        if (isEmpty()) {
            cout << "\nNo reservations.\n";
            return;
        }
        
        cout << "\n--- Reservation Queue ---\n";
        int i = front;
        for (int j = 0; j < count; j++) {
            cout << (j + 1) << ". Member ID: " << queue[i] << endl;
            i = (i + 1) % maxSize;
        }
    }
    
    int getCount() {
        return count;
    }
};

#endif