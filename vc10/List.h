#pragma once
#include "Node.h"

class List {
public:
    ~List(); //Deconstructor
    
    /**
     *  Creates a list with the elements of an array of entries, and a specified length
     *  This constructor checks for duplicates, and sorts on the x dimension
     **/
    List(Entry* entries, int len);
    
    /**
     * Creates a list with the elements of an array of entries, a specified length,
     * and sorted on a specified dimension
     **/
    List(Entry* entries, int len, bool x);
    
    //Sentinel node for the circular never empty list
    Node* sentinel;
    //Returns the median of the list, if the list is even in length, it returns the node closest to the front
    Node* getMedian();
    //Inserts the whole array of entries to an empty list
    void insertAll(Entry* entries, int len);
    //Inserts and checks a whole arrau of entries to a list
    void insertAndCheckAll(Entry* entries, int len);
    //Inserts one node into the correct part of a list
    void insert(Entry* toInsert);
    //Inserts and check originality of an entry to the correct part of the list
    void insertAndCheck(Entry* toInsert);
    //Number of entries
    int length;
    //True if the current list is sorted on the x dimension
    bool isX;
    //Removes a node from the list, and does the rewiring, so there arent any holes or dangling refernces
    Entry* remove(Node* toBeRemoved);
    //Splits the array into half around the median, true if you want the first half, false otherwise
    List* split(bool first);
private:
    //Computes the distance between two points
    double distance(double a, double b);
};