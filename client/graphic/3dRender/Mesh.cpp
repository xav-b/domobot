#include "Mesh.h"

using namespace std;

aiVector3D scene_min, scene_max, scene_center;
float angle = 0;

Mesh::Mesh() : basePath(""), modelName("") {}
 
Mesh::Mesh(std::string modelName): basePath(""), modelName("")
{
	Init(modelName);
}
 
Mesh::~Mesh(void) {
	aiReleaseImport(scene);
	aiDetachAllLogStreams();
}

void Mesh::get_bounding_box_for_node (aiNode* nd, 
	aiVector3D* min, 
	aiVector3D* max, 
	aiMatrix4x4* trafo
){
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {
			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

// ----------------------------------------------------------------------------
void Mesh::get_bounding_box (aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

void Mesh::Render(float scale = 0.f) {
    glPushMatrix();
    // scale the whole asset to fit into our view frustum 
    float tmp = scene_max.x-scene_min.x;
    tmp = aisgl_max(scene_max.y - scene_min.y,tmp);
    tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
    tmp = 1.f / tmp;
    glScalef(tmp, tmp, tmp);
    glScalef(scale, scale, scale);

    // center the model
    glTranslatef( -scene_center.x, -scene_center.y, -scene_center.z );

    // if the display list has not been made yet, create a new one and fill it with scene contents
    if(scene_list == 0) {
        scene_list = glGenLists(1);
        glNewList(scene_list, GL_COMPILE);
        // now begin at the root node of the imported data and traverse the scenegraph by multiplying subsequent local transforms
        // together on GL's matrix stack.
        this->Render(scene, scene->mRootNode, 0.5f);
        glEndList();
    }
    glCallList(scene_list);
    glPopMatrix();
}

int Mesh::Init(std::string modelFile)
{
	scene_list = 0;
    basePath = "../../models/BLEND/";   
    modelName = modelFile;
	configured = false;
 
    //Assimp::Importer importer;
    //scene = importer.ReadFile( (basePath+modelName).c_str(),
                   //aiProcess_CalcTangentSpace       | 
                   //aiProcess_Triangulate            |
                   //aiProcess_JoinIdenticalVertices  |
                   //aiProcess_SortByPType );
    scene = aiImportFile((basePath + modelName).c_str(), //aiProcessPreset_TargetRealtime_Quality); 
              aiProcess_CalcTangentSpace       | 
              aiProcess_Triangulate            |
              aiProcess_JoinIdenticalVertices  |
              aiProcess_SortByPType);
	if( !scene) {
		//std::cout << importer.GetErrorString() << std::endl;
		std::cout << aiGetErrorString() << std::endl;
		return 1;
	} else {
		get_bounding_box(&scene_min, &scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		return 0;
	}
	return 2;	
    //LoadGLTextures();
}
 
const aiScene* Mesh::getAIScene() {
    return scene;
}

void Mesh::Render(const aiScene* node, const struct aiNode* nd, float scale)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		ApplyMaterial(scene->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}
			glEnd();
		}
	}
	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) 
		Render(scene, nd->mChildren[n], 0.f);
	
	glPopMatrix();
}
 
void Mesh::ApplyMaterial(const aiMaterial* mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else 
		glEnable(GL_CULL_FACE);
}
 
int Mesh::LoadGLTextures()
{
    ILboolean success;
 
    if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
        ILint test = ilGetInteger(IL_VERSION_NUM);
        //wrong DevIL version
        std::string error = "Wrong DevIL version";
        char* cError = (char*) error.c_str();
        //glfwCloseWindow();
        return -1;
    }
 
    ilInit();
 
    if(scene->HasTextures())
    {
        //glfwCloseWindow();
    }
 
    for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        int texIndex = 0;
        aiReturn texFound = AI_SUCCESS;
        aiString path; //filename
 
        while(texFound == AI_SUCCESS)
        {
            texFound = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
            textureIdMap[path.data] = NULL;
            texIndex++;
        }
    }
 
    int numTextures = textureIdMap.size();
 
    ILuint* imageIds = NULL;
    imageIds = new ILuint[numTextures];
 
    /* generate DevIL Image IDs */
    ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */
 
    /* create and fill array with GL texture ids */
    textureIds = new GLuint[numTextures];
    glGenTextures(numTextures, textureIds); /* Texture name generation */
 
    /* define texture path */
    //std::string texturepath = "../../../test/models/Obj/";
 
    /* get iterator */
    std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
 
    for (int i=0; i<numTextures; i++)
    {
 
        //save IL image ID
        std::string filename = (*itr).first;  // get filename
        (*itr).second =  &textureIds[i];	  // save texture id for filename in map
        itr++;								  // next texture
 
 
        ilBindImage(imageIds[i]); // Binding of DevIL image name */
        std::string fileloc = basePath + filename;	// Loading of image */
        success = ilLoadImage(fileloc.c_str());
 
        if (success) /* If no error occured: */
        {
            success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); // Convert every colour component into
            //unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA 
            if (!success)
            {
                /* Error occured */
                std::cout << "Couldn't convert image." << std::endl;
                //glfwCloseWindow();
                return -1;
            }
            //glGenTextures(numTextures, &textureIds[i]); // Texture name generation 
            glBindTexture(GL_TEXTURE_2D, textureIds[i]); // Binding of texture name 
            //redefine standard texture values
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // We will use linear
            //interpolation for magnification filter 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // We will use linear
            //interpolation for minifying filter 
            glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
                ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
                ilGetData()); // Texture specification 
        }
        else
        {
            /* Error occured */
            std::cout << "Couldn't load image:" << fileloc << std::endl;
            //MessageBox(NULL, ("Couldn't load Image: " + fileloc).c_str() , "ERROR", MB_OK | MB_ICONEXCLAMATION);
        }
    }
    ilDeleteImages(numTextures, imageIds); // Because we have already copied image data into texture data
    //we can release memory used by image. 
 
    //Cleanup
    delete [] imageIds;
    imageIds = NULL;
 
    //return success;
    return true;
}
 
void Mesh::Color4f(const aiColor4D* color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void Mesh::color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void Mesh::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
