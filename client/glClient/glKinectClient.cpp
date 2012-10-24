#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

boost::mutex g_cursorsMutex;
int id;
int state;
float xAngle;
float yAngle;
float zAngle;

#define KEY_ESCAPE 27
#define CHKERR if (glGetError() != GL_NO_ERROR) printf("** I screwed up something\n");

using namespace std;

static int windowId;
int pressed;

typedef struct {
	int width;
	int height;
	string title;	
	float view_angle;
	float z_near;
	float z_far;
} glutWindow;
glutWindow win;

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
glutGlass glass;

typedef struct {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
} point3D;

#include "networkHandler.h"

// Keyboard events handler
void keyboardHandler(unsigned char key, int x, int y) {
    switch(key) {
        case KEY_ESCAPE:
            cout << "Exiting...\n";
            glutDestroyWindow(windowId);
            exit(0);
        case 'a':
            win.view_angle++; // Fovy
            break;
        case 'z':
            win.view_angle--; // Fovy
            break;
	default:
	    break;
    }
    glutPostRedisplay();
}

// Mouse events handler
void mouseHandler(int button, int state, int x, int y) {
    if ( (button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) 
        pressed = 1;
    if ( (button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) 
        pressed = 0;
}
void pMouseMotionHandler(int x, int y) {}

// Background process handler, for animation use
void idle(void) {glutPostRedisplay();}

void reshape(int x, int y) {
    if ( x < y )
        glViewport(0, (y-x)/2, x, x);
    else
        glViewport((x-y)/2, 0, y, y);
}

void display(void) {
    /* Cleaning screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    /* Managing camera */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(win.view_angle, // Fovy
                   (GLfloat) win.width/win.height, //aspect
                   win.z_near, // Near
                   win.z_far); // Far

    gluLookAt(glass.eye_x, glass.eye_y, glass.eye_z,
            glass.observed_x, glass.observed_y, glass.observed_z,
            glass.sky_x, glass.sky_y, glass.sky_z);

    /* Drawing the labo */
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    GLint i;
    glBegin(GL_LINES);
    for ( i = -10; i <= 10; i++ ) {
        glVertex3i(i, 0, -10 );
        glVertex3i(i, 0, 10);
    }
    for ( i = -10; i <= 10; i++ ) {
        glVertex3i(-10, 0, i);
        glVertex3i(10, 0, i);
    }
    glEnd();

    /* Moving objects */
    for ( int i = 0; i < 6; i++ ) {
        glPushMatrix();
        glTranslatef(-hand[i]["X"]*30, 0.0, 0.0);
        glTranslatef(0.0, -hand[i]["Y"] * 30, 0.0);
        glTranslatef(0.0, 0.0, -hand[i]["Z"] * 30);
        glutWireSphere(1 /*radius*/, 20 /*slices*/, 20 /*stacks*/);
        glPopMatrix();
    }

    /* Updating opengl server */
    //glFlush();
    glutSwapBuffers();
}

void initialize() {
    glMatrixMode(GL_PROJECTION);				
    glViewport(0, 0, win.width, win.height);			
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();								
    GLfloat aspect = (GLfloat) win.width / win.height;
    gluPerspective(win.view_angle, aspect, win.z_near, win.z_far);
    glMatrixMode(GL_MODELVIEW);		
    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );			
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char** argv, char** envp) {
    // Window init
    win.width = 640;
    win.height = 480;
    win.title = "OpenGL/GLUT Window.";
    win.view_angle = 80;
    win.z_near = 1.0f;
    win.z_far = 500.0f;

    // Look at init
    glass.eye_x = 4;
    glass.eye_y = 4;
    glass.eye_z = 7;
    glass.observed_x = 0;
    glass.observed_y = 0;
    glass.observed_z = 0;
    glass.sky_x = 0;
    glass.sky_y = 1;
    glass.sky_z = 0;

    // Glut manager init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(win.width, win.height);
    glPointSize(2.0);
    windowId = glutCreateWindow((win.title).c_str());
    glutSetWindow(windowId);
    //glutFullScreen();

    // Matrix init
    xAngle = 0;
    yAngle = 0;
    zAngle = 0;
    pressed = 0;

    // Network retriever launcher
    std::cout << "main: startup" << std::endl;
    networkHandler nh(10002);
    nh.init();
    nh.start();
    std::cout << "main: waiting for thread" << std::endl;
    nh();
    if ( nh.running() )
        std::cout << "Process running\n";

    // Callback functions
    // See timer 
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutPassiveMotionFunc(pMouseMotionHandler);
    glutReshapeFunc(reshape);

    initialize();
    glutMainLoop();

    nh.join();
    std::cout << "main: done" << std::endl;

    return 0;
}
