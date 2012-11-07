#include "Starbucks.h"
#include "Resources.h"

class Quadtree : public Starbucks{
	public:
		Quadtree();
		void build(Entry* c, int n);
		Entry* getNearest(double x, double y);
		void subdivide(Quadtree* inserted_point, Quadtree* current_point);
		//Getters and setters
		void setEntry(Entry* e);
		void setNorthWest(Quadtree* t);
		void setNorthEast(Quadtree* t);
		void setSouthWest(Quadtree* t);
		void setSouthEast(Quadtree* t);
		Entry* getEntry();
		Quadtree* getNorthWest();
		Quadtree* getNorthEast();
		Quadtree* getSouthWest();
		Quadtree* getSouthEast();
		Quadtree* getRoot();
	private:
		Quadtree* northWest;
		Quadtree* northEast;
		Quadtree* southWest;
		Quadtree* southEast;
		Quadtree* root;
		Entry* location;
};