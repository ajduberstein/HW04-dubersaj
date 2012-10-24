#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Resources.h"
//#include "Quadtree.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class NearestStarbucksApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void NearestStarbucksApp::setup()
{
//	Quadtree t;
	ifstream in("Starbucks_2006.csv");
	string line;
	getline(in,line);
	in.get();
	double d;
	in >> d;
	double d1;
	in >> d1;
}

void NearestStarbucksApp::mouseDown( MouseEvent event )
{
}

void NearestStarbucksApp::update()
{
}

void NearestStarbucksApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( NearestStarbucksApp, RendererGl )
