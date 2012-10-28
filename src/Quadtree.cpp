#include "Starbucks.h"
#include "Quadtree.h"

void Quadtree::build(Entry* c, int n){
	//Computes the centroid
	double x_centroid = 0;
	double y_centroid = 0;
	for (int i = 0; i < n; i++){
		x_centroid += c[i].x;
		y_centroid += c[i].y;
	}
	x_centroid /= n;
	y_centroid /= n;
	Entry centroid;
	centroid.x = x_centroid;
	centroid.y = y_centroid;
	//Using the centroid, constructs the quadtree
	Quadtree* root = new Quadtree();
	Quadtree* tmp = new Quadtree();
	root->setEntry(&centroid);
	for (int i = 0; i < n; i++){
		tmp->setEntry(&c[i]);
		subdivide(tmp,root);
	}
}

void Quadtree::subdivide(Quadtree* inserted_point, Quadtree* current_point){
	if(inserted_point->getEntry()->y > current_point->getEntry()->y)
	{
		if(inserted_point->getEntry()->x <= current_point->getEntry()->x)
			if(!(northWest->occupied)){
				current_point->northWest = inserted_point;
				northWest->occupied = true;
			}
			else
				subdivide(inserted_point, current_point->northWest);
		else
			if(!(northEast->occupied)){
				current_point->northEast = inserted_point;
				northEast->occupied = true;
			}
			else
				subdivide(inserted_point, current_point->northEast);
	}
	else if(inserted_point->getEntry()->y <= current_point->getEntry()->y)
	{
		if(inserted_point->getEntry()->x <= current_point->getEntry()->x)
			if(!(southWest->occupied)){
				current_point->southWest = inserted_point;
				southWest->occupied = true;
			}
			else
				subdivide(inserted_point, current_point->southWest);
		else
			if(!(southEast->occupied)){
				current_point->southEast = inserted_point;
				southEast->occupied = true;
			}
			else
				subdivide(inserted_point, current_point->southEast);
	}
	else{
		//Exception goes here
	}
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
