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
	Quadtree* root = new Quadtree();
	Quadtree* tmp = new Quadtree();
	root->setEntry(&centroid);
	for (int i = 1; i <= n; i++){
		tmp->setEntry(&entries[i]);
		subdivide(tmp,root);
	}
	memcpy(this,root,sizeof(Quadtree));
	delete[] entries;
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

Entry* Quadtree::getNearest(double x, double y)
{
	//North points
	if(getEntry()->y > y)
	{
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
