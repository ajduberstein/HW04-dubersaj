#include "Starbucks.h"
#include "Quadtree.h"
#include <algorithm>

void Quadtree::build(Entry* c, int n){
	//Computes the centroid
	Entry* entries = new Entry[n];
	std::copy(c,c+n,entries);
	double x_centroid = 0;
	double y_centroid = 0;
	for (int i = 1; i <= n; i++){
		x_centroid += entries[i].x;
		y_centroid += entries[i].y;
	}
	x_centroid /= n;
	y_centroid /= n;
	Entry centroid;
	centroid.identifier = "CENTROID";
	centroid.x = x_centroid;
	centroid.y = y_centroid;
	//Using the centroid, constructs the quadtree
	root = new Quadtree();
	Quadtree* tmp = new Quadtree();
	root->setEntry(&centroid);
	for (int i = 1; i <= n; i++){
		tmp->setEntry(&entries[i]);
		subdivide(tmp,root);
	}
	//delete[] entries;
}

void Quadtree::subdivide(Quadtree* inserted_point, Quadtree* current_point)
{
	//Substract x and y's, compare with .000001, return without doing anything
	if (abs(inserted_point->getEntry()->x - current_point->getEntry()->x) > .00001 &&
		abs(inserted_point->getEntry()->y - current_point->getEntry()->y) > .00001)
	{
		//North points	
		if(inserted_point->getEntry()->y > current_point->getEntry()->y)
		{
			if(inserted_point->getEntry()->x <= current_point->getEntry()->x)
			{
				if(current_point->northWest == NULL)
				{
					current_point->northWest = new Quadtree();
					memcpy(&current_point->northWest->location,&inserted_point->location,sizeof(Entry*));
				}
				else
				{
					subdivide(inserted_point, current_point->northWest);
				}
			}
			else
			{
				if(current_point->northEast == NULL)
				{
					current_point->northEast = new Quadtree();
					memcpy(&(current_point->northEast->location),&inserted_point->location,sizeof(Entry*));
				}
				else
				{
					subdivide(inserted_point, current_point->northEast);
				}
			}
		}
		//South points
		else if(inserted_point->getEntry()->y <= current_point->getEntry()->y)
		{
			if(inserted_point->getEntry()->x <= current_point->getEntry()->x)
			{
				if(current_point->southWest == NULL)
				{
					current_point->southWest = new Quadtree();
					memcpy(&(current_point->southWest->location),&inserted_point->location,sizeof(Entry*));
				}
				else
				{
					subdivide(inserted_point, current_point->southWest);
				}
			}
			else
			{
				if(current_point->southEast == NULL)
				{
					current_point->southEast = new Quadtree();
					memcpy(&(current_point->southEast->location),&inserted_point->location,sizeof(Entry*));
				}
				else
				{
					subdivide(inserted_point, current_point->southEast);
				}
			}
		}
		else{
			string s = "If I show up in the debugger, there's an error.";
		}
	}
	else{
		//Do nothing
	}
}

//Query 
Entry* Quadtree::getNearest(double x, double y)
{
	//Code from www.stackoverflow.com/%2Fquestions%2F10745733%2Fnext-iteration-in-z-order-curve&ei=HyOPULr4K4uq0AHIjIG4Cw&usg=AFQjCNEVWeNgOmHP935rJke1Q8qDKT-Fqw
	int carry = 1;
	do
	{
		int newcarry = x & carry;
		x ^= carry;
		carry = newcarry;
		newcarry = (y & carry) << 1;
		y ^= carry;
		carry = newcarry;
	} while (carry != 0);
	//If we have a branch, see if one of the branches has a location close to the given point
	if(getNorthWest() != NULL || getNorthEast() != NULL || getSouthWest() != NULL || getSouthEast() != NULL)
	{
		//North points
		if(getEntry()->y > y)
		{
			//Northwest points
			if(getEntry()->x <= x)
			{
				if (getNorthWest() != NULL)
				{
					getNorthWest()->getNearest(x,y);
				}
				else
				{
					return getEntry();
				}
			}
			//Otherwise, Northeast points
			else
			{
				if (getNorthEast() != NULL)
				{
					getNorthEast()->getNearest(x,y);
				}
				else
				{
					return getEntry();
				}
			}
		}
		//South points
		else if(getEntry()->y <= getEntry()->y)
		{
			if(getEntry()->x <= x)
			{
				if (getSouthWest() != NULL)
				{
					getSouthWest()->getNearest(x,y);
				}
				else
				{
					return getEntry();
				}
			}
			else
			{
				if (getSouthEast() != NULL)
				{
					getSouthEast()->getNearest(x,y);
				}
				else
				{
					return getEntry();
				}
			}
		}
		else
		{
			return 0;	
		}
	}
	//Otherwise, if we're at the centroid root, move up to a node with concrete branches
	else if (getRoot() != NULL){
		return getRoot()->getNearest(x,y);
	}
	//In this case, we're not at the root and we have no further branches, so this must be a leaf.
	else{
		return getEntry();
	}
}

//Getters
Quadtree* Quadtree::getNorthWest(){
	return northWest;
}
Quadtree* Quadtree::getNorthEast(){
	return northEast;
}
Quadtree* Quadtree::getSouthWest(){
	return southWest;
}
Quadtree* Quadtree::getSouthEast(){
	return southEast;
}
Entry* Quadtree::getEntry(){
	return location;
}
Quadtree* Quadtree::getRoot(){
	return root;
}

//Setters for directional nodes
void Quadtree::setNorthWest(Quadtree* t){
	northWest = t;
}
void Quadtree::setNorthEast(Quadtree* t){
	northEast = t;
}
void Quadtree::setSouthWest(Quadtree* t){
	southWest = t;
}
void Quadtree::setSouthEast(Quadtree* t){
	southEast = t;
}
void Quadtree::setEntry(Entry* e){
	location = e;
}
