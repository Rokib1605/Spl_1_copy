#include "Recommendation.h"
using namespace std;


int BookRecommendation::isSimilar(Book& b1, Book& b2) {
    return strcmp(b1.genre, b2.genre) == 0 ? 1 : 0;
}

// Simple Genre Clustering
void BookRecommendation::clusterByGenre(Book books[], int count) {
    cout << "\n=== Book Clustering by Genre ===\n";

    char genres[10][30];  
    int genreCount = 0;

    // Find unique genres
    for (int i = 0; i < count; i++) {
        bool found = false;
        for (int j = 0; j < genreCount; j++) {
            if (strcmp(books[i].genre, genres[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            strcpy(genres[genreCount++], books[i].genre);
        }
    }

    // Display clusters
    for (int i = 0; i < genreCount; i++) {
        cout << "\nCluster " << (i + 1) << ": " << genres[i] << endl;
        for (int j = 0; j < count; j++) {
            if (strcmp(books[j].genre, genres[i]) == 0) {
                cout << "  - " << books[j].title << endl;
            }
        }
    }
}


void BookRecommendation::recommend(Book books[], int count, int lastBookID) {
    cout << "\n=== Recommendations ===\n";

    // Find last borrowed book
    Book* target = NULL;
    for (int i = 0; i < count; i++) {
        if (books[i].bookID == lastBookID) {
            target = &books[i];
            break;
        }
    }

    if (target == NULL) return;

    cout << "Based on: " << target->title << " (" << target->genre << ")\n\n";

    // Recommend books with same genre
    int recommended = 0;
    for (int i = 0; i < count && recommended < 5; i++) {
        if (books[i].bookID != lastBookID &&
            strcmp(books[i].genre, target->genre) == 0) {
            cout << ++recommended << ". " << books[i].title << endl;
        }
    }

    if (recommended == 0) {
        cout << "No similar books found!\n";
    }
}
