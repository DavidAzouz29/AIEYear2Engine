#pragma once

//#include "FBXFile.h"
#include "Entity\Entity.h"
#include "Mesh.h" //TOOD: needed?
//#include "VertexData.h"

//#include <map> //TODO: remove once Texture class is working
#include <memory>

class Camera;
//class Mesh;

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

	///void FBXLoader();
	//void RenderFBX(Camera* cam);

	/// ----------------------------------------------------------
	/// Texture related
	/// ----------------------------------------------------------
	/*GLuint TextureInit(const GLchar* name);
	GLvoid TextureLoader();
	GLvoid RenderTexture();
	GLvoid DrawTexture(Camera* cam);
	GLvoid DrawTextureP(Camera* cam);
	GLvoid AddTexture(const GLchar* name, const GLuint id);

	GLuint GetTextureByName(const GLchar* name); */
	/// ----------------------------------------------------------

	//void RenderTargetLoader();

//	Mesh* GetSharedPointer() const { return m_pMesh.get(); }
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

