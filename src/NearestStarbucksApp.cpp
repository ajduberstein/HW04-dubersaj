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
	std::istream& safeGetline(std::istream& is, std::string& t);
};

void NearestStarbucksApp::setup()
{
//	Quadtree t;
	std::string path = "Starbucks_2006.csv";
	std::ifstream ifs(path.c_str());
    std::string t;
	int n = 0;
	while(safeGetline(ifs, t)){
		console() << t << std::endl;
		if (t.length() > 1){
			++n;
		}
		else{
			break;
		}
	}
	console() << n << std::endl;
}

/**
 *Credit to http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
 *for this cross-platform solution for reading characters
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
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( NearestStarbucksApp, RendererGl )
