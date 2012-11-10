#pragma once
#include "Starbucks.h"
/**
 * Code attributed to Brandon E. Sonoda
 * https://github.com/Sonodabe/HW04_Sonodabe as of 11-10-2012
 */
class Node{
public:
    //Date of the node
    Entry* data;
    //Previous pointer
    Node* prev;
    //next pointer
    Node* next;
};