#include "cinder/Text.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Font.h"
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

struct CensusEntry{
  float x;
  float y;
  int population;
};

class NearestStarbucksApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void prepareSettings(Settings *settings);
	void draw();
	std::istream& safeGetline(std::istream& is, std::string& t);
	void createHeatMap(CensusEntry* entries, int len, bool inc_blue, bool inc_green, bool inc_red);
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int r, int color_increment, bool inc_blue, bool inc_green, bool inc_red);
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int r, int red, int green, int blue); 
	void keyDown(KeyEvent event);
	void render();
  private:
	 Font master_font_;
	 gl::Texture master_texture_font_;
	  Starbucks_Sonodabe* tree;
	  gl::Texture mImage;
	  Entry* entries;
	  CensusEntry* census_2010;
	  CensusEntry* census_2000;
	  Surface* starbucksLayer_;
	  int list_length_;
	  Entry* nearest_starbucks_;
	  int filler_; 
	  bool display_starbucks_locations_;
	  bool display_help;
	  double param_;
	  int shape_changer_;
	  int total_population_2010;
	  int total_population_2000;
	  Surface* mySurface_;
	  static const int kTextureSize=1024;
	  static const int kAppWidth=800;
	  static const int kAppHeight=600;
};

void NearestStarbucksApp::prepareSettings(Settings *settings){
	settings->setWindowSize(kAppWidth, kAppHeight);
	settings->setFrameRate(60.0f);
	settings->setResizable(false);
	settings->setFullScreen(false);
}

void NearestStarbucksApp::setup()
{
	master_font_ = Font("Helvetica",32);
	display_starbucks_locations_ = false;
	display_help = true;
	param_ = 0;
	nearest_starbucks_ = NULL;
	std::string path = "Starbucks_2006.csv";
	std::string census_2000_path = "Census_2000.csv";
	std::string census_2010_path = "Census_2010.csv";
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
	ifs.close();
	std::ifstream ifs2(path.c_str());
	entries = new Entry[len+1];
	t = "Temporary text";
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
	//Credit to Brandon Sonodabe for this object.
	tree = new Starbucks_Sonodabe();
	tree->build(entries, n);
	list_length_ = n;
	std::ifstream ifs3(census_2000_path.c_str());
	census_2010 = new CensusEntry[216331];
	census_2000 = new CensusEntry[206676];
	total_population_2010 = 0;
	total_population_2000 = 0;
	n = 0;
	while(safeGetline(ifs3, t)){
		if (t.length() > 1){
			n++;
		}
		else{ 
			break;
		}
		census_2010[n].population = atoi(t.substr(0, t.find_first_of(",")).c_str());
		census_2010[n].x = atof(t.substr(t.find(",")+1, t.rfind(",") - t.find(",")-1).c_str());
		census_2010[n].y = atof(t.substr(t.find_last_of(",")+1,t.length() - t.rfind(",")).c_str());
		total_population_2010 += census_2010[n].population;
	}
	n = 0;
	std::ifstream ifs4(census_2000_path.c_str());
	while(safeGetline(ifs4, t)){
		if (t.length() > 1){
			n++;
		}
		else{ 
			break;
		}
		census_2000[n].population =  atoi(t.substr(0, t.find_first_of(",")).c_str());
		census_2000[n].x = atof(t.substr(t.find(",")+1, t.rfind(",") - t.find(",")-1).c_str());
		census_2000[n].y = atof(t.substr(t.find_last_of(",")+1,t.length() - t.rfind(",")).c_str());
		total_population_2000 += census_2000[n].population;
	}
	//Load locations to a data structure
	//Test to make sure the data structure established
	console() << tree->tree->root->data->identifier << std::endl;
	console() << tree->getNearest(.4,.4)->identifier << std::endl;
	//Test to make sure resource is in assets folder
	console() << "US Picture at " << getAssetPath("USA.jpg") << std::endl;
	mImage = gl::Texture(loadImage(loadResource(RES_USA)));
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	createHeatMap(census_2010,216331,false,true,false);
	createHeatMap(census_2000,206676,false,false,true);
	render();	
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

void NearestStarbucksApp::keyDown(KeyEvent event){
	if( event.getChar() == 'j' ){
		display_starbucks_locations_ = !(display_starbucks_locations_);
	}
	else if (event.getChar() == '?'){
		display_help = !(display_help);
	}
	else{}
}

void NearestStarbucksApp::render(){
	string txt = "Starbucks Visualizer\n Green saturation represents population in 2000. \n Red saturation represents population in 2010. \n Intermediate colors represent areas of change.\n Pale areas represent changes in regional person/Starbucks density. \n The redder the region, the more people in 2010. \n Press j to remove the pale spots. \n ? will toggle these instructions.";
	TextBox tbox = TextBox().alignment( TextBox::CENTER ).font(master_font_).size( Vec2i( 512, 511) ).text( txt );
	tbox.setColor( Color( 1.0f, 0.65f, 0.35f ) );
	tbox.setBackgroundColor( ColorA( 0.5, 0, 0, 1 ) );
	master_texture_font_ = gl::Texture( tbox.render() );
}

void NearestStarbucksApp::createHeatMap(CensusEntry* entries, int len, bool inc_blue, bool inc_green, bool inc_red){
	uint8_t* dataArray = (*mySurface_).getData();
	for(int i = 0; i < len; i++){
		drawCircle(dataArray, getWindowWidth()*entries[i].x, getWindowHeight()-getWindowHeight()*entries[i].y,3,500*(total_population_2010 - entries[i].population)/total_population_2010,inc_red,inc_green,inc_blue);
	}
}

/**
 * This method is modified from HW01. It's originally Brandon Harmon's. 
 * Code is viewable at https://github.com/ajduberstein/HomeWork01/blob/master/src/HomeWork01App.cpp
 */
void NearestStarbucksApp::drawCircle(uint8_t* pixels, int center_x, int center_y, int r, int color_increment, bool inc_blue, bool inc_green, bool inc_red)
{
	if(r <= 0) 
		return;
	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){
			if((y < 0 || x < 0 || x >= kTextureSize || y >= kTextureSize)) break;
			int dist = (int) sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			if(dist <= r)
			{
				int curBlock = 3*(x + y*kTextureSize);
				//Invert color by 255-current pixels
				if (inc_red){
					pixels[3*(x+y*kTextureSize)] += color_increment;
					pixels[3*(x+y*kTextureSize)+1] -= color_increment;
				}
				if (inc_green){
					pixels[3*(x+y*kTextureSize)+1] += color_increment;
					pixels[3*(x+y*kTextureSize)] -= color_increment;
				}
				if (inc_blue)
					pixels[3*(x+y*kTextureSize)+2] += color_increment;
			}
		}
	}
}

void NearestStarbucksApp::drawCircle(uint8_t* pixels, int center_x, int center_y, int r, int red, int green, int blue)
{
	if(r <= 0) 
		return;
	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){
			if((y < 0 || x < 0 || x >= kAppWidth || y >= kAppHeight)) break;
			int dist = (int) sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			if(dist <= r)
			{
				int curBlock = 3*(x + y*kTextureSize);
				//Invert color by 255-current pixels
				if(red < 250)
					pixels[3*(x+y*kTextureSize)]  = red;
				else
					red = 250;
				if(green < 250)
					pixels[3*(x+y*kTextureSize)+1] = green;
				else
					green = 250;
				if(blue < 250)
					pixels[3*(x+y*kTextureSize)+2] = blue;
				else
					blue = 250;
			}
		}
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
	gl::clear(Color(0,0,0));
	gl::draw(*mySurface_);
	uint8_t* pixels = (*mySurface_).getData();
//	gl::draw(mImage,Rectf(0,0,getWindowWidth(),getWindowHeight()));
	//Colors each region based on population change, satisying HW04, Phase 2, Goal E
	//Colors each region based on population density, measured by people/Starbucks, satisying HW04, Phase 2, Goal F
	//Colors each region based on change in people per Starbucks, satisying HW04, Phase 2, satisying HW04, Phase 2, Goal G
	//Draws all 2006 Starbucks locations on the map, satisying HW04, Phase 2, Goal A.
	
	if(nearest_starbucks_ != NULL){
		gl::enableAlphaBlending();
		gl::color(ColorA(200,10,100,150));
	//Highlights nearest Starbucks to clicked location, satisying HW04, Phase 2, Goal B.
		gl::drawSolidEllipse(Vec2f(getWindowWidth()*nearest_starbucks_->x,getWindowHeight()-getWindowHeight()*nearest_starbucks_->y),13.00f*param_,14.00f*param_,3+param_);
		gl::disableAlphaBlending();
	}
	if(!display_starbucks_locations_){
		for(int i = 0; i < list_length_; i++){
			gl::enableAlphaBlending();
			gl::color(ColorA8u(250,250,250,30));
			gl::drawSolidCircle(Vec2f(getWindowWidth()*entries[i].x,getWindowHeight()-getWindowHeight()*entries[i].y),10,40);
			gl::color(ColorA(200,10,100,150));
			gl::disableAlphaBlending();
		}
	}
	if (master_texture_font_ && display_help){
		gl::enableAlphaBlending();
		gl::color(ColorA8u(150,150,0));
		gl::draw( master_texture_font_);
	}
}

CINDER_APP_BASIC( NearestStarbucksApp, RendererGl )
