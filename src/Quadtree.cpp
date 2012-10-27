#include "Starbucks.h"
#include "Quadtree.h"

void Quadtree::build(Entry* c, int n){
	//Computes the centroid
	double x_centroid = 0;
	double y_centroid = 0;
	for (int i = 0; i < n; i++){
		x_centroid += c[i]->x;
		y_centroid += c[i]->y;
	}
	x_centroid /= n;
	y_centroid /= n;
	Entry centroid;
	centroid.x = x_centroid;
	centroid.y = y_centroid;
	//Using the centroid, constructs the quadtree
	Quadtree t = new Quadtree();
	t.setEntry(centroid);
	for (int i = 0; i < n; i++){
		if(c[i].y > centroid.y){
			if(c[i].x <= centroid.x)
				if(!occupied)
					northWest->location = c[i];
				else
					northWest.subdivide();
			else
				if(!occupied)
					northEast->location = c[i];
				else
					northEast.subdivide();
		}
		else{
			if(c[i].x <= centroid.x)
				if(!occupied)
			        	southWest->location = c[i];
				else
					southWest.subdivide();
			else
				if(!occupied)
					southEast->location = c[i];
				else
					southEast.subdivide();	
		}
	}	
}

void Quadtree::subdivide(){

}

Entry* Quadtree::getNearest(double x, double y){
	return 0;
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
