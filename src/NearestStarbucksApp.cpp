#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "Resources.h"
#include "Quadtree.h"
#include <vector>

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
	  Quadtree* tree;
	  gl::Texture mImage;
};

void NearestStarbucksApp::prepareSettings(Settings *settings){
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
	settings->setResizable(false);
	settings->setFullScreen(false);
}

void NearestStarbucksApp::setup()
{
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
	Entry* entries = new Entry[len+1];
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
	//Load locations to a data structure
	tree = new Quadtree();
	tree->build(entries, n+1);
	//Test to make sure the data structure established
	console() << tree->getRoot()->getNorthWest()->getEntry()->identifier << std::endl;
	console() << tree->getNearest(.4,.4)->identifier << std::endl;
	//Test to make sure resource is in assets folder
	console() << "US Picture at " << getAssetPath("USA.jpg") << std::endl;
//	Surface processedImage( loadImage( loadResource( RES_USA ) ) );
	mImage = gl::Texture(loadImage(loadResource(RES_USA)));
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
}

void NearestStarbucksApp::update()
{
}

void NearestStarbucksApp::draw()
{
//	gl::clear( Color( 0, 0, 0 ) ); 
	gl::draw(mImage);
}

CINDER_APP_BASIC( NearestStarbucksApp, RendererGl )
