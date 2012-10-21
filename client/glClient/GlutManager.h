#ifndef DEF_GLUT_MGT
#define DEF_GLUT_MGT
 
#include <iostream>

#include <GL/gl.h>
#include <GL/glut.h>

#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


#define KEY_ESCAPE 27
#define CHKERR if (glGetError() != GL_NO_ERROR) printf("** I screwed up something\n");


typedef struct {
	int width;
	int height;
    std::string title;	
	float view_angle;
	float z_near;
	float z_far;
} glutWindow;

typedef struct {
    float eye_x;
    float eye_y;
    float eye_z;
    float observed_x;
    float observed_y;
    float observed_z;
    float sky_x;
    float sky_y;
    float sky_z;
} glutGlass;

typedef struct {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
} point3D;

/**
 * 
 */
class GlutManager
{
    public:
        GlutManager();
        ~GlutManager();
		void (*keyboardHandler)(unsigned char key, int x, int y);
		void (*mouseHandler)(int button, int state, int x, int y);
		void *pMouseMotionHandler(int x, int y);
		void *idle(void);
		void *reshape(int x, int y);
		void *display(void);
		void initialize();
		void mainLoop(int argc, char** argv);

    protected:
		static int windowId;
		int pressed;
		glutWindow win;
		glutGlass glass;
};

#endif
