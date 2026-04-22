#ifndef BST_H
#define BST_H
#include "Book.h"

class BookNode {
public:
    Book data;
    BookNode* left;
    BookNode* right;
    
    BookNode() {
        left = NULL;
        right = NULL;
    }
};

class BookBST {
private:
    BookNode* root;
    
    BookNode* insertHelper(BookNode* node, Book book) {
        if (node == NULL) {
            BookNode* newNode = new BookNode();
            newNode->data = book;
            newNode->left = NULL;
            newNode->right = NULL;
            return newNode;
        }
        
        if (book.bookID < node->data.bookID) {
            node->left = insertHelper(node->left, book);
        } else if (book.bookID > node->data.bookID) {
            node->right = insertHelper(node->right, book);
        }
        
        return node;
    }
    
    BookNode* searchHelper(BookNode* node, int bookID) {
        if (node == NULL) {
            return NULL;
        }
        
        if (bookID == node->data.bookID) {
            return node;
        } else if (bookID < node->data.bookID) {
            return searchHelper(node->left, bookID);
        } else {
            return searchHelper(node->right, bookID);
        }
    }
    
    void displayAllHelper(BookNode* node) {
        if (node != NULL) {
            displayAllHelper(node->left);
            node->data.display();
            displayAllHelper(node->right);
        }
    }
    
    void toArrayHelper(BookNode* node, Book arr[], int& index, int maxSize) {
        if (node != NULL && index < maxSize) {
            toArrayHelper(node->left, arr, index, maxSize);
            arr[index++] = node->data;
            toArrayHelper(node->right, arr, index, maxSize);
        }
    }

    BookNode* findMin(BookNode* node) {
        while (node && node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    BookNode* removeHelper(BookNode* node, int bookID, bool& success) {
        if (node == NULL) {
            return node; 
        }

        if (bookID < node->data.bookID) {
            node->left = removeHelper(node->left, bookID, success);
        } 
        else if (bookID > node->data.bookID) {
            node->right = removeHelper(node->right, bookID, success);
        } 
        else {
            success = true;
            if (node->left == NULL) {
                BookNode* temp = node->right;
                delete node;
                return temp;
            } 
            else if (node->right == NULL) {
                BookNode* temp = node->left;
                delete node;
                return temp;
            }

            BookNode* temp = findMin(node->right);
            node->data = temp->data;
            node->right = removeHelper(node->right, temp->data.bookID, success);
        }
        return node;
    }

public:
    BookBST() {
        root = NULL;
    }
    
    void insert(Book book) {
        root = insertHelper(root, book);
    }
    
    Book* search(int bookID) {
        BookNode* result = searchHelper(root, bookID);
        if (result != NULL) {
            return &(result->data);
        }
        return NULL;
    }
    
    void displayAll() {
        if (root == NULL) {
            cout << "\nNo books in library!\n";
            return;
        }
        displayAllHelper(root);
    }
    
    int toArray(Book arr[], int maxSize) {
        int index = 0;
        toArrayHelper(root, arr, index, maxSize);
        return index;
    }

    bool remove(int bookID) {
        bool success = false;
        root = removeHelper(root, bookID, success); 
        return success;
    }
};

#endif