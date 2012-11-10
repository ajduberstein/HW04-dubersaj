#include "Starbucks.h"
#include "List.h"
#include "KDTree.h"
/**
 * Code attributed to Brandon E. Sonoda
 * https://github.com/Sonodabe/HW04_Sonodabe as of 11-10-2012
 */

class Starbucks_Sonodabe : public Starbucks {
public:
    //Destructor
    ~Starbucks_Sonodabe(){
        delete list;
        delete tree;
        
        list = NULL;
        tree = NULL;
    }
    //Binary search tree where every other level is sorted on x and y
    KDTree* tree;
    //The list to handle the brute force searches (Some of the binary WAS unsuccessful)
    List* list;
    //Build the tree with a specified list
    TreeNode* buildTree(List* list, bool onX);
    //Builds a KD tree and a linked list when given an array of entries
	virtual void build(Entry* c, int n);
    //Finds the nearest location when given an x and y coordinate
    virtual Entry* getNearest(double x, double y);    
    //Finds the nearest tree node when given an x and y coordinate and a subtree to search
    TreeNode* getNearest(double x, double y, TreeNode* root);
    //Finds the nearest starbucks using the brute force method... had to include this because sometimes the tree didnt working
    Entry* getManNearest(double x, double y, List* list);    
};