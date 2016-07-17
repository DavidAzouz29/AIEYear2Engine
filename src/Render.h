// TODO: clean this header up
// structs moved to "VertexData.h" and made to be compatible with Mesh.h

#pragma once
//#ifndef _RENDER_H_
//#define _RENDER_H_

#define GLM_SWIZZLE

#include "Entity\Entity.h"
//#include "Mesh.h" //TOOD: needed?
#include "Renderable.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

//using glm::vec2;
//using glm::vec3;
//using glm::vec4;
using glm::mat4;

struct Material; //m_pRenderable->mesh.Create(
struct Meshes;
struct Scene;
struct SubMesh;

Scene LoadSceneFromOBJ(char* dir, char* filename);
void FreeSceneFromOBJ(Scene* scene);

unsigned int LoadGLTextureBasic(const char * path);

Meshes *CreateMeshFromBuffers(SimpleVertex* vertex_data, unsigned int vertex_count, unsigned int *index_data, unsigned int index_count, Material material);
//NOTE(aidan): This should only be called on meshes created from the
//CreateMeshFromBuffers function. DO NOT call on the meshes in a scene
void FreeMesh(Meshes* mesh);
void RebuildVertexBuffer(Meshes* mesh);


class Renderer
{
public:
	Renderer();
	~Renderer();

	void PushMesh(Meshes* mesh, mat4 transform);
	void RenderAndClear(mat4 view_proj);

	struct RenderItem
	{
		Meshes* mesh;
		mat4 transform;
	};

	std::vector<RenderItem> render_queue;

	unsigned int main_shader;
};

class Camera;
//class Meshes;

class Render // : public Entity
{
public:
	Render() :
		fHeightScale(2.0f),
		fTime(0.0f)
	{}
	~Render() = default;

	bool Create();

	/// ----------------------------------------------------------
	/// function to create a grid
	/// ----------------------------------------------------------
	/// param:
	///			a_iRows: a set of rows
	///			a_iCols: a set of columns
	/// ----------------------------------------------------------
	//GLvoid generateGrid(const GLuint a_iRows, const GLuint a_iCols);

	GLvoid InitGeometry();
	GLvoid DrawGeometry(const Camera& a_pCamState);

	GLuint GetProgramID() const { return m_programID; }

private:
	//our vertex and index buffers
	//unsigned int m_VAO; // Vertex Array Object
	//unsigned int m_VBO;	// Vertex Buffer Object
	//unsigned int m_IBO;	// Index Buffer Object

	GLuint m_programID;
//	Vertex_PositionColor* aoVertices;
	
	/// ----------------------------------------------------------
	/// Texture
	/// ----------------------------------------------------------
	// Store the OpenGL texture.
	//////////std::map<const std::string, const GLuint> m_textures, m_normalmap;
	/// ----------------------------------------------------------
	/// RenderTarget
	/// ----------------------------------------------------------
	//GLuint m_fboTexture;

	GLfloat fHeightScale;
	GLfloat fTime;

	Mesh m_mesh; //TODO: remove

	//std::shared_ptr<Texture> m_pTexture;
};

//#endif