#include "Mesh.cpp"
#include <assimp/DefaultLogger.hpp>
#include <stdlib.h>
 
void createAILogger()
{
	//Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;
	// Create a logger instance for Console Output
	Assimp::DefaultLogger::create("",severity, aiDefaultLogStream_STDOUT);
	// Create a logger instance for File Output (found in project folder or near .exe)
	Assimp::DefaultLogger::create("assimp_log.txt",severity, aiDefaultLogStream_FILE);
	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
}
 
void logInfo(std::string logString)
{
	//Will add message to File with "info" Tag
	Assimp::DefaultLogger::get()->info(logString.c_str());
}
 
Mesh mesh;
 
GLfloat		xrot;
GLfloat		yrot;
GLfloat		zrot;
 
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 15.0f, 1.0f };
 
int InitGL()					 // All Setup For OpenGL goes here
{
	mesh.Init("HUMAN.blend");
 
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation
 
 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);
 
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
 
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
 
	return true;					// Initialization Went OK
}
 
void DrawGLScene()				//Here's where we do all the drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();				// Reset MV Matrix
 
	glTranslatef(0.0f, -10.0f, -40.0f);	// Move 40 Units And Into The Screen
 
 
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);
 
	mesh.Render(0.5f);
 
	glutSwapBuffers();
	//xrot+=0.3f;
	yrot+=0.2f;
	//zrot+=0.4f;
}
 
 
void glutDisplayFunction()
{
	DrawGLScene();
}
 
 
int main(int argc, char** argv)
{
	bool done = false;	// Bool Variable To Exit Loop
 
	glutInit(&argc,argv);
 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_STENCIL | GLUT_RGBA);
	glutInitWindowSize(800,600);
 
	if(!glutCreateWindow("Testing"))
		exit(EXIT_FAILURE);
 
	glutIdleFunc(&glutDisplayFunction);
	glutDisplayFunc(&glutDisplayFunction);
 
	InitGL(); //Load mesh, call LoadGLTextures and set OpenGL values.
 
	createAILogger();
	logInfo("App fired!");
	// load scene
	logInfo("=============== Post Import ====================");
 
	glutMainLoop();
	return 0;
}
