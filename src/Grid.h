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
/// TODO: Clean up
/// Camera zoom with scroll
/// search for TODO:
/// DrawGeom(m_projectionViewMatrix or in this case m4ProjectionTrans
/// FBXTexture
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
		iGrid(64),
		m_perlinTextureID(),
		uiOctaves(6),
		fAmplitude(5.8f),
		fPersistence(0.3f)
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
	GLint iGrid;
	GLuint m_perlinTextureID; //m_programID
	Texture m_perlinTexture; //-1;

	GLint uiOctaves;
	GLfloat fAmplitude;
	GLfloat fPersistence;
};

