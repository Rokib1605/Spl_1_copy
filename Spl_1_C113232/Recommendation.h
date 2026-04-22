#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include "Book.h"
#include <cstring>
#include <iostream>

class BookRecommendation {
private:
    // Simple similarity: same genre = 1, different = 0
    int isSimilar(Book& b1, Book& b2);

public:
   
    void clusterByGenre(Book books[], int count);

    
    void recommend(Book books[], int count, int lastBookID);
};

#endif
