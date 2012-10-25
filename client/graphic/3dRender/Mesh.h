#ifndef MESH_H
#define MESH_H
 
#include <GL/gl.h>
 
#include <assimp/cimport.h>
//#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>
#include <iostream>
#include <string>
#include <map>
 
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

class Mesh
{
	public:
		Mesh();
		Mesh(std::string modelName);
		~Mesh(void);
		int Init(std::string modelName);
		void Render(float scale);
		void ApplyMaterial(const aiMaterial* mtl);
        int LoadGLTextures();
        const aiScene* getAIScene();

	private:
		void get_bounding_box_for_node (aiNode* nd, 
                                        aiVector3D* min, 
                                        aiVector3D* max, 
                                        aiMatrix4x4* trafo);
		void get_bounding_box (aiVector3D* min, aiVector3D* max);
		void Render(const aiScene *sc, const aiNode* nd, float scale);
		void Color4f(const aiColor4D* color);
        void color4_to_float4(const aiColor4D *c, float f[4]);
        void set_float4(float f[4], float a, float b, float c, float d);
		bool configured;
		const aiScene *scene;
		std::map<std::string, GLuint*> textureIdMap;
		GLuint* textureIds;
        std::string basePath;
        std::string modelName;
		GLuint scene_list;
};
 
#endif
