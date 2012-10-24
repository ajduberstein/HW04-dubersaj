#include "Starbucks.h"
class Quadtree{
	public:
		struct Point{
			double x;
			double y;
		};
		struct BoundingBox{
			Point center;
			Point halfDimension;
			BoundingBox construct(Point c, Point hd) {
				center = c;
				halfDimension = hd;
			}
			bool containsPoint(Point p) {
				
			}
		};
		void build(Entry* c, int n);
		Entry* getNearest(double x, double y);
	private:
		const int NODE_CAPACITY = 4;
		Quadtree* northWest;
		Quadtree* northEast;
		Quadtree* southWest;
		Quadtree* southEast;
		BoundingBox area;
		Point p;
};