#pragma once

#include "FBXFile.h"
#include "Camera.h"
#include "VertexData.h"
#include "Mesh.h"

#include <map>
#include <memory>

class Render
{
public:
	Render();
	~Render();

	/// ----------------------------------------------------------
	/// function to create a grid
	/// ----------------------------------------------------------
	/// param:
	///			a_iRows: a set of rows
	///			a_iCols: a set of columns
	/// ----------------------------------------------------------
	void generateGrid(const unsigned int a_iRows, const unsigned int a_iCols);

	void InitGeometry();
	void DrawGeometry(Camera* cam);

	///void FBXLoader();
	//void RenderFBX(Camera* cam);

	unsigned int TextureInit(const char* name);
	void TextureLoader();
	void RenderTexture();
	void DrawTexture(Camera* cam);
	void DrawTextureP(Camera* cam);
	void AddTexture(const char* name, const unsigned int id);

	unsigned int GetTextureByName(const char* name);

	void RenderTargetLoader();

	Mesh* GetSharedPointer() const { return m_pMesh.get(); }
	GLuint GetProgramID() const { return m_programID; }

private:
	//our vertex and index buffers
	//unsigned int m_VAO; // Vertex Array Object
	//unsigned int m_VBO;	// Vertex Buffer Object
	//unsigned int m_IBO;	// Index Buffer Object

	unsigned int m_programID;
//	Vertex_PositionColor* aoVertices;
	
	/// ----------------------------------------------------------
	/// Texture
	/// ----------------------------------------------------------
	// Store the OpenGL texture.
	std::map<const std::string, const unsigned int> m_textures, m_normalmap;
	/// ----------------------------------------------------------
	/// RenderTarget
	/// ----------------------------------------------------------
	//GLuint m_fboTexture;

	float fHeightScale;
	float fTime;

	std::shared_ptr<Mesh> m_pMesh;
	//FBXFile* m_fbx;
};

