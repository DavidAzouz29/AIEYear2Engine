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
/// - Undo/ Reset button added		 	- David Azouz 12/06/16
/// - Terrain Texture correct again	 	- David Azouz 12/06/16
/// - fixed crash upon regen + made Perlin 
/// adjustable 'live' during runtime.	- David Azouz 14/06/16
/// TODO: fix m_perlinTextureID in .cpp
/// 
/// </summary>
/// ----------------------------------------------------------

#pragma once

#include "Entity\Entity.h"
//#include "Texture.h"
#include "gl_core_4_4.h"

class Grid : public Entity
{
public:
	Grid() :
		m_fHeightScale(5),
		m_fTime(0.0f),
		m_perlinProgramID(),
		m_iGrid(64),
		m_fScale((1.0f / m_iGrid) * 3),
		m_uiOctaves(6),
		m_fAmplitude(5.8f),
		m_fPersistence(0.3f),
		m_pfHeightScale(m_fHeightScale),
		m_piGrid(m_iGrid),
		m_pfScale(m_fScale),
		m_puiOctaves(m_uiOctaves),
		m_pfAmplitude(m_fAmplitude),
		m_pfPersistence(m_fPersistence),
		v3LightDir(1,1,1.5f)
	{}
	~Grid() = default;

	// ------------------------------------
	// Entity specific
	// ------------------------------------
	bool Create() override;
	//bool Update(GLfloat deltaTime) override;
	GLvoid Draw(const Camera& a_camState) override;
	GLvoid Destroy() override {}
	GLvoid RenderUI() override;
	// ----------------------------------------------------------
	// function to create a grid
	// ----------------------------------------------------------
	// param:
	//			a_iRows: a set of rows
	//			a_iCols: a set of columns
	// ----------------------------------------------------------
	bool GenerateGrid(const GLuint a_iRows, const GLuint a_iCols);
	
	//Shader code (red)
	GLvoid CreateDrawShader();
	GLvoid DrawGeometry(const glm::mat4& a_projectionView);

	GLuint GetProgramID() const { return m_perlinProgramID; }
	
private:
	GLfloat m_fHeightScale;
	GLfloat m_fTime;
	GLuint m_perlinProgramID; //m_programID
	//Texture m_perlinTexture; //-1;

	GLint m_iGrid;
	GLfloat m_fScale;
	GLint m_uiOctaves;
	GLfloat m_fAmplitude;
	GLfloat m_fPersistence;

	//Previous values for undo
	GLfloat m_pfHeightScale;
	GLint m_piGrid;
	GLfloat m_pfScale;
	GLint m_puiOctaves;
	GLfloat m_pfAmplitude;
	GLfloat m_pfPersistence;

	glm::vec3 v3LightDir;
};

