#include "GlutManager.h"

using namespace std;

GlutManager::GlutManager() {}

void GlutManager::keyboardHandler(unsigned char key, int x, int y) {
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
    return NULL;
}

// Mouse events handler
void GlutManager::mouseHandler(int button, int state, int x, int y) {
    if ( (button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) 
        pressed = 1;
    if ( (button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) 
        pressed = 0;
    return NULL;
}

void *GlutManager::pMouseMotionHandler(int x, int y) {
    return NULL;
}

// Background process handler, for animation use
void *GlutManager::idle(void) {
    return NULL;
}

void *GlutManager::reshape(int x, int y) {
    if ( x < y )
        glViewport(0, (y-x)/2, x, x);
    else
        glViewport((x-y)/2, 0, y, y);
    return NULL;
}

void *GlutManager::display(void) {
    /* Cleaning screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(win.view_angle, // Fovy
                   (GLfloat) win.width/win.height, //aspect
                   win.z_near, // Near
                   win.z_far); // Far

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    /* Calibrating camera */
    gluLookAt(glass.eye_x, glass.eye_y, glass.eye_z,
            glass.observed_x, glass.observed_y, glass.observed_z,
            glass.sky_x, glass.sky_y, glass.sky_z);

    /* Moving objects */
    //glRotated(-xAngle/2, 1.0, 0.0, 0.0);
    //glRotated(-yAngle/2, 0.0, 1.0, 0.0);
    //glTranslatef(-xAngle, 0.0, 0.0);
    //glTranslatef(0.0, -yAngle, 0.0);
    //glTranslatef(0.0, 0.0, -zAngle);
    //std::cout << id << ": Translating cube (" << state <<  ") -> " << xAngle << " - " << yAngle << " - " << zAngle << std::endl << std::endl;
    //glutWireTorus(0.5, 2, 10, 30);
    glutWireCube(1);

    /* Updating opengl server */
    glutSwapBuffers();
    return NULL;
}

void GlutManager::initialize() {
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

void GlutManager::mainLoop(int argc, char** argv) {
// Window init
    win.width = 640;
    win.height = 480;
    win.title = "OpenGL/GLUT Window.";
    win.view_angle = 70;
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
    pressed = 0;

    // Callback functions
    // See timer 
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutPassiveMotionFunc(pMouseMotionHandler);
    glutReshapeFunc(reshape);

    //initialize();
    glutMainLoop();

    std::cout << "main: done" << std::endl;
    return;
}

