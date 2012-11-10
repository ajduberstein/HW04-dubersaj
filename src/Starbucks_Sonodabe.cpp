/**
 * Code attributed to Brandon E. Sonoda
 */
#include "Starbucks_Sonodabe.h"


void Starbucks_Sonodabe::build(Entry* c, int n){
    Entry* entries = new Entry[n];
    for(int i = 0; i<n; i++){
        (entries+i)->identifier = (c+i)->identifier;
        (entries+i)->x = (c+i)->x;
        (entries+i)->y = (c+i)->y;
    }
    
    list = new List(entries, n);
    tree = new KDTree;
    tree->root = buildTree(list, true);
}

TreeNode* Starbucks_Sonodabe::buildTree(List* list, bool onX){
    if(list == NULL || list->length == 0)
        return NULL;
    
    TreeNode* root = new TreeNode;
    List* left = list->split(true);
    List* right = list->split(false);
    root->data = list->getMedian()->data;
    if(left->length != 0)
        root->left = buildTree(left, !onX);
    else
        root->left = NULL;
    if(right->length != 0)
        root->right = buildTree(right, !onX);
    else
        root->right = NULL;
    root->isX = onX;
    
    delete  left;
    delete  right;
    
    
    return root;
}

Entry* Starbucks_Sonodabe::getNearest(double x, double y){
    return getNearest(x, y, tree->root)->data;    
}

TreeNode* Starbucks_Sonodabe::getNearest(double x, double y, TreeNode* root){
    if(root == NULL)
        return NULL;
    double distSquared, distFromBorder;
    //Candidate is the nearest of the side that the point is on
    
    //If the distance between the border and the point is closer than the point and the candidate search the other side
    
    TreeNode* candidate;
    bool left;
    if(root->isX){
        if(x <= root->data->x){
            candidate = (root->left != NULL && root->left->data->identifier != "")? getNearest(x, y, root->left) : root;
            left = true;   
        }
        else{
            candidate = (root->right != NULL && root->right->data->identifier != "")? getNearest(x, y, root->right) : root;
            left = false;
        }
        
        distSquared = (candidate->data->x-x)*(candidate->data->x-x)+(candidate->data->y-y)*(candidate->data->y-y);
        distFromBorder = root->data->x - x;

        if(distFromBorder*distFromBorder <= distSquared*1.15){
            TreeNode* candidate2;
            
            if(left)
                candidate2 = (root->right != NULL && root->right->data->identifier != "")? getNearest(x, y, root->right) : root;
            else
                candidate2 = (root->left != NULL && root->left->data->identifier != "")? getNearest(x, y, root->left) : root;
            
            double distSquared2 = (candidate2->data->x-x)*(candidate2->data->x-x)+(candidate2->data->y-y)*(candidate2->data->y-y);
            if(distSquared2 < distSquared){
                candidate = candidate2;
                distSquared = distSquared2;
            }
        }
        
        
        //Check to see if the root is closer
        double distFromRoot = (root->data->x-x)*(root->data->x-x)+(root->data->y-y)*(root->data->y-y);
        
        if(distFromRoot < distSquared)
            return root;
        return candidate;
    }else{
        if(y <= root->data->y){
            candidate = (root->left != NULL && root->left->data->identifier != "")? getNearest(x, y, root->left) : root;
            left = true;   
        }
        else{
            candidate = (root->right != NULL && root->right->data->identifier != "")? getNearest(x, y, root->right) : root;
            left = false;
        }
        
        distSquared = (candidate->data->x-x)*(candidate->data->x-x)+(candidate->data->y-y)*(candidate->data->y-y);
        distFromBorder = root->data->y - y;
        
        if(distFromBorder*distFromBorder <= distSquared*1.15){
            TreeNode* candidate2;
            
            if(left)
                candidate2 = (root->right != NULL && root->right->data->identifier != "")? getNearest(x, y, root->right) : root;
            else
                candidate2 = (root->left != NULL && root->left->data->identifier != "")? getNearest(x, y, root->left) : root;
            
            double distSquared2 = (candidate2->data->x-x)*(candidate2->data->x-x)+(candidate2->data->y-y)*(candidate2->data->y-y);
            if(distSquared2 < distSquared){
                candidate = candidate2;
                distSquared = distSquared2;
            }
        }
        
        
        //Check to see if the root is closer
        double distFromRoot = (root->data->x-x)*(root->data->x-x)+(root->data->y-y)*(root->data->y-y);
        
        if(distFromRoot < distSquared)
            return root;
        return candidate;
    }
}


Entry* Starbucks_Sonodabe::getManNearest(double x, double y, List* list){
    Node* cur = list->sentinel->next;
    double distanceSq = (cur->data->x-x)*(cur->data->x-x)+(cur->data->y-y)*(cur->data->y-y);
    Node* bestSoFar = cur;
    double distBest = (cur->data->x-x)*(cur->data->x-x)+(cur->data->y-y)*(cur->data->y-y);
    
    while(cur != list->sentinel){
        distanceSq = (cur->data->x-x)*(cur->data->x-x)+(cur->data->y-y)*(cur->data->y-y);
        if(distanceSq < distBest){
            distBest = distanceSq;
            bestSoFar = cur;
        }
        cur = cur->next;
    }
    
    return bestSoFar->data;
}