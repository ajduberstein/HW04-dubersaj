#include "Starbucks.h"
#include "Resources.h"
/**
 * Code attributed to Brandon E. Sonoda
 * https://github.com/Sonodabe/HW04_Sonodabe as of 11-10-2012
 */
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
		void setParent(Quadtree* t);
		Entry* getEntry();
		Quadtree* getNorthWest();
		Quadtree* getNorthEast();
		Quadtree* getSouthWest();
		Quadtree* getSouthEast();
		Quadtree* getRoot();
		Quadtree* getParent();
	private:
		Quadtree* northWest;
		Quadtree* northEast;
		Quadtree* southWest;
		Quadtree* southEast;
		Quadtree* root;
		Quadtree* parent;
		Entry* location;
};