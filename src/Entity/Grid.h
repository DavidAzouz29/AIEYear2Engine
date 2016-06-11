/// <summary>
/// File Name:		Grid.h
/// Author: 		David Azouz
/// Date Created: 	10/06/16
/// Date Modified: 	10/06/16
///----------------------------------------------------------
/// Brief: A Grid Class that generates Perlin data.
/// viewed: 
///
/// ***EDIT***
/// - Perlin land generation added	 	- David Azouz 10/06/16
/// 
/// TODO: fix m_perlinTextureID in .cpp
/// 
/// </summary>
/// ----------------------------------------------------------

#pragma once

#include "Entity\Entity.h"
#include "Texture.h"
#include "gl_core_4_4.h"

class Grid : public Entity
{
public:
	Grid() :
		fHeightScale(2),
		fTime(0.0f),
		m_perlinTextureID(),
		m_iGrid(64),
		m_fScale((1.0f / m_iGrid) * 3),
		m_uiOctaves(6),
		m_fAmplitude(5.8f),
		m_fPersistence(0.3f)
	{}
	~Grid();

	// ------------------------------------
	// Entity specific
	// ------------------------------------
	bool Create() override;
	GLvoid Update() override;
	GLvoid Draw(const Camera& m_pCamState) override;
	GLvoid Destroy() override {}
	GLvoid RenderUI() override;
	// ----------------------------------------------------------
	// function to create a grid
	// ----------------------------------------------------------
	// param:
	//			a_iRows: a set of rows
	//			a_iCols: a set of columns
	// ----------------------------------------------------------
	GLvoid GenerateGrid(const GLuint a_iRows, const GLuint a_iCols);
	
	//Shader code (red)
	GLvoid CreateDrawShader();
	GLvoid DrawGeometry(const glm::mat4& a_projectionView);

	GLuint GetProgramID() const { return m_perlinTextureID; }
	
private:
	GLfloat fHeightScale;
	GLfloat fTime;
	GLuint m_perlinTextureID; //m_programID
	Texture m_perlinTexture; //-1;

	GLint m_iGrid;
	GLfloat m_fScale;
	GLint m_uiOctaves;
	GLfloat m_fAmplitude;
	GLfloat m_fPersistence;
};

