#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "Resources.h"
#include "KDTree.h"
#include <vector>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Starbucks_Sonodabe.h"
#include "cinder/gl/Texture.h"
#include <iostream>
#include <fstream>
#include <time.h>
#define PI 3.14159265
using namespace ci;
using namespace ci::app;
using namespace std;

class NearestStarbucksApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void prepareSettings(Settings *settings);
	void draw();
	std::istream& safeGetline(std::istream& is, std::string& t);
  private:
	  Starbucks_Sonodabe* tree;
	  gl::Texture mImage;
	  Entry* entries;
	  Surface* starbucksLayer_;
	  int list_length_;
	  Entry* nearest_starbucks_;
	  int filler_; 
	  double param_;
	  int shape_changer_;
};

void NearestStarbucksApp::prepareSettings(Settings *settings){
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
	settings->setResizable(false);
	settings->setFullScreen(false);
}

void NearestStarbucksApp::setup()
{
	param_ = 0;
	nearest_starbucks_ = NULL;
	std::string path = "Starbucks_2006.csv";
	std::ifstream ifs(path.c_str());
    std::string t;
	int n = 0;
	int len = 0;
	while(safeGetline(ifs, t)){
		if (t.length() > 1){
			++len;
		}
		else{
			break;
		}
	}
	std::ifstream ifs2(path.c_str());
	entries = new Entry[len+1];
	t = "Temporary text";
	/*
	Pixels will be placed from the entries array on a surface. We might have to invert the y. 1 - decimal_values
	Check if there's a population growth area around
	If so, for every increment of population growth, add a degree of red color change

	*/
	//Import Starbucks Locations file
	while(safeGetline(ifs2, t)){
		if (t.length() > 1){
			n++;
		}
		else{ 
			break;
		}
		entries[n].identifier =  t.substr(0, t.find_first_of(","));
		entries[n].x = atof(t.substr(t.find(",")+1, t.rfind(",") - t.find(",")-1).c_str());
		entries[n].y = atof(t.substr(t.find_last_of(",")+1,t.length() - t.rfind(",")).c_str());
	}
	//Load locations to a data structure
	tree = new Starbucks_Sonodabe();
	tree->build(entries, n);
	//Test to make sure the data structure established
	console() << tree->tree->root->data->identifier << std::endl;
	console() << tree->getNearest(.4,.4)->identifier << std::endl;
	//Test to make sure resource is in assets folder
	console() << "US Picture at " << getAssetPath("USA.jpg") << std::endl;
//	Surface processedImage( loadImage( loadResource( RES_USA ) ) );
	mImage = gl::Texture(loadImage(loadResource(RES_USA)));
	list_length_ = n;
}

/**
 *Credit to http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
 *for this cross-platform solution for file-reading
 */
std::istream& NearestStarbucksApp::safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\r':
            c = sb->sgetc();
            if(c == '\n')
                sb->sbumpc();
            return is;
        case '\n':
        case EOF:
            return is;
        default:
            t += (char)c;
        }
    }
}

void NearestStarbucksApp::mouseDown( MouseEvent event )
{
	if(event.isRight()){
		console() << event.getX() << "," << event.getY() << std::endl;
	}
	if(event.isLeft()){
		nearest_starbucks_ = new Entry();
		nearest_starbucks_->x = tree->getNearest(1-(getWindowWidth()-event.getX()*1.0000f)/getWindowWidth(),(getWindowHeight()-event.getY()*1.0f)/getWindowHeight()*1.0f)->x; 
		nearest_starbucks_->y = tree->getNearest(1-(getWindowWidth()-event.getX()*1.0000f)/getWindowWidth(),(getWindowHeight()-event.getY()*1.0f)/getWindowHeight()*1.0f)->y; 
		nearest_starbucks_->identifier = tree->getNearest(1-(getWindowWidth()-event.getX()*1.0000f)/getWindowWidth(),(getWindowHeight()-event.getY()*1.0f)/getWindowHeight()*1.0f)->identifier; 
		console() << "The nearest Starbucks to the clicked point is listed below. " << std::endl; 
		console() << nearest_starbucks_->identifier << std::endl;
	}
}

void NearestStarbucksApp::update()
{
	param_++;
	shape_changer_ = sin((param_)*PI/100000);
	if (param_>3)
		param_ = 0;
}

void NearestStarbucksApp::draw()
{
	gl::draw(mImage,Rectf(0,0,getWindowWidth(),getWindowHeight()));
	gl::color(0,112,74);
	for(int i = 0; i < list_length_; i++){
		gl::drawSolidCircle(Vec2f(getWindowWidth()*entries[i].x,getWindowHeight()-getWindowHeight()*entries[i].y),2.50f,40);
	}
	if(nearest_starbucks_ != NULL){
		gl::enableAlphaBlending();
		gl::color(ColorA8u(174,0,174,150));
		gl::drawSolidEllipse(Vec2f(getWindowWidth()*nearest_starbucks_->x,getWindowHeight()-getWindowHeight()*nearest_starbucks_->y),13.00f*param_,14.00f*param_,3+param_);
	}
		gl::disableAlphaBlending();
}

CINDER_APP_BASIC( NearestStarbucksApp, RendererGl )
