#include "Mesh.cpp"
#include <assimp/DefaultLogger.hpp>
 
#include <GL/glfw.h>

void createAILogger()
{
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;
	Assimp::DefaultLogger::create("",severity, aiDefaultLogStream_STDOUT);
	Assimp::DefaultLogger::create("assimp_log.txt",severity, aiDefaultLogStream_FILE);
	Assimp::DefaultLogger::get()->info("this is my info-call");
}
 
void logInfo(std::string logString)
{
	Assimp::DefaultLogger::get()->info(logString.c_str());
}
 
void glAppShutdown(int return_code)
{
  glfwTerminate();
  exit(return_code);
}

void glAppInit(std::string winTitle = "Main window")
{
    const int window_width = 800,
          window_height = 600;

    if (glfwInit() != GL_TRUE)
        glAppShutdown(1);
    // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
    if (glfwOpenWindow(window_width, window_height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
        glAppShutdown(1);
    glfwSetWindowTitle(winTitle.c_str());

    glEnable(GL_TEXTURE_2D);
    glClearDepth(1.0f);				// Depth Buffer Setup
    glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    // Uses default lighting parameters
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
    GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightPosition[]= { 0.0f, 0.0f, 15.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);
}

int main(int argc, char** argv)
{
	glAppInit("Lets draw !");
	int running = GL_TRUE;
	double old_time = glfwGetTime();
	const float rotations_per_tick = .2;
 
	GLfloat		xrot;
    GLfloat		yrot;
    GLfloat		zrot;
 
	Mesh mesh;     
    if ( argc > 1 ) {
        if ( mesh.Init(argv[1]) != 0 ) 
            glAppShutdown(1);
    }
    else {
        if ( mesh.Init("HUMAN.blend") != 0 )
            glAppShutdown(1);
    }
 
	createAILogger();
	logInfo("Post import.");

    const aiScene* scene = mesh.getAIScene();
    if ( scene->HasAnimations() )
        cout << "Found " << scene->mNumAnimations << " animations.\n"; 
    if ( scene->HasCameras() )
        cout << "Found " << scene->mNumCameras << " cameras.\n"; 
    if ( scene->HasLights() )
        cout << "Found " << scene->mNumLights << " lights.\n"; 
    if ( scene->HasTextures() )
        cout << "Found " << scene->mNumTextures << " textures.\n"; 
    if ( scene->HasMeshes() )
        cout << "Found " << scene->mNumMeshes << " meshes.\n"; 
 
	//glOrtho( 0.0, 1.0, 0.0, 1.0, -1.0, 1.0 );
	while(running)
	{
        double current_time = glfwGetTime(),
               delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
        old_time = current_time;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.3f, 0.3f, 0.0f);	// Move 40 Units And Into The Screen
        glRotatef(yrot/8, 1.0f, 0.0f, 0.0f);
        glRotatef(yrot/8, 0.0f, 1.0f, 0.0f);
        //glRotatef(yrot, 0.0f, 0.0f, 1.0f);
        yrot+=0.1f;
        mesh.Render(1.2 /* scale */);         // This is where the mesh tries to render to OpenGL.
        glfwSwapBuffers();
        if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
            yrot += (delta_rotate);
        if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
            yrot -= (delta_rotate);
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
    glAppShutdown(0);
 
	exit(EXIT_SUCCESS);
}
