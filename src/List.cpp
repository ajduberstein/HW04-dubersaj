/**
 * Code attributed to Brandon E. Sonoda
 */
#include "List.h"

List::List(Entry* entries, int len){
    sentinel = new Node;
    isX = true;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    length = 0;
    
    insertAndCheckAll(entries, len);
}

List::List(Entry* entries, int len, bool x){
    sentinel = new Node;
    isX = x;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    length = 0;
    
    insertAll(entries, len);
}

List::~List(){
    delete sentinel;
    sentinel = NULL;
}

void List::insertAll(Entry* entries, int len){
    for(int i = 0; i<len; i++)
        insert(entries+i);
}

void List::insertAndCheckAll(Entry* entries, int len){
    for(int i = 0; i<len; i++)
        insertAndCheck(entries+i);
}

List* List::split(bool first){
    List* newList;
    int len = 0;
    Entry* newEntries;
    if(first){
        len = length/2;
        newEntries = new Entry[len];
        int i = 0;
        Node* cur = sentinel->next;
        
        while(cur!= getMedian()){
            newEntries[i].identifier = cur->data->identifier;
            newEntries[i].x = cur->data->x;
            newEntries[i].y = cur->data->y;
            cur = cur->next;
            i++;
        }
    }else{
        if(len%2 == 0)
            len = length-length/2-1;
        else
            len = length-length/2;
        newEntries = new Entry[len];
        
        Node* cur = getMedian()->next;
        int i = 0;
        while(cur!= sentinel){
            newEntries[i].identifier = cur->data->identifier;
            newEntries[i].x = cur->data->x;
            newEntries[i].y = cur->data->y;
            cur = cur->next;
            i++;
        }
        
    }
    
    newList = new List(newEntries, len, !isX);
    return newList;
}



Node* List::getMedian(){
    int medianIndex = length/2+1;
    Node* cur;
    cur = sentinel;
    for(int i = 0; i<medianIndex; i++)
        cur = cur->next;
    
    
    
    return cur;
}

Entry* List::remove(Node* toBeRemoved){
    if(toBeRemoved == NULL || toBeRemoved == sentinel)
        return NULL;
    
    toBeRemoved->prev->next = toBeRemoved->next;
    toBeRemoved->next->prev = toBeRemoved->prev;
    
    Entry* data = toBeRemoved->data;
    delete toBeRemoved;
    length--;
    return data;
}


void List::insertAndCheck(Entry* toInsert){
    Node* insertee = new Node;
    insertee->data = new Entry;
    insertee->data = toInsert;
    
    double posX = insertee->data->x;
    double posY = insertee->data->y;
    Node* cur = sentinel->next;
    
    double distX, distY;
    
    if(isX){
        while(cur != sentinel && cur->data->x < posX){
            distX = distance(cur->data->x, posX);
            distY = distance(cur->data->y, posY);
            if(distX <= 0.00001 && distY <= 0.00001)
                return;
            cur = cur->next;
        }
    }else{
        while(cur != sentinel && cur->data->y < posY){
            distX = distance(cur->data->x, posX);
            distY = distance(cur->data->y, posY);
            if(distX <= 0.00001 && distY <= 0.00001)
                return;
            cur = cur->next;
        }
        
    }
    
    if(cur == sentinel){
        sentinel->prev->next = insertee;
        insertee->prev = sentinel->prev;
        insertee->next = sentinel;
        sentinel->prev = insertee;
        length++;
        return;
    }
    
    distX = distance(cur->data->x, posX);
    distY = distance(cur->data->y, posY);
    if(distX <= 0.00001 && distY <= 0.00001)
        return;
    
    cur->prev->next = insertee;
    insertee->prev = cur->prev;
    insertee->next = cur;
    cur->prev = insertee;
    length++;
}

void List::insert(Entry* toInsert){
    Node* insertee = new Node;
    insertee->data = new Entry;
    insertee->data = toInsert;

    double posX = toInsert->x;
    double posY = toInsert->y;
    Node* cur = sentinel->next;
    
    if(isX){
        while(cur != sentinel && cur->data->x < posX)
            cur = cur->next;
    }else{
        while(cur != sentinel && cur->data->y < posY)
            cur = cur->next;  
    }
    
    
    cur->prev->next = insertee;
    insertee->prev = cur->prev;
    insertee->next = cur;
    cur->prev = insertee; 
    length++;
}

double List::distance(double a, double b){
    double dist = a-b;
    if(dist>=0)
        return dist;
    return -dist;
}
