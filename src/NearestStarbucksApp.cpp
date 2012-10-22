#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

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
