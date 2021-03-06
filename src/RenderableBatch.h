/// <summary>
/// File Name:		RenderableBatch.h
/// Author: 		David Azouz
/// Date Created: 	10/05/16
/// Date Modified: 	24/05/16
///----------------------------------------------------------
/// Brief: An Abstract Renderable Class defining the interface all renderable objects must implement.
/// Enables more than one object to share the same mesh or texture etc.
/// 
/// viewed: http://www.dofactory.com/net/factory-method-design-pattern
/// http://www.ogre3d.org/docs/api/1.9/class_ogre_1_1_renderable.html#details
/// https://github.com/johnsietsma/RefEngine/blob/f46a001e7194b57ac7777af8b56a4809366025b4/Engine/src/graphics/Renderable.h
/// Batching https://www.opengl.org/sdk/docs/man/html/glDrawElementsBaseVertex.xhtml
/// Batching https://www.opengl.org/sdk/docs/man/html/glDrawElementsInstancedBaseVertex.xhtml
/// https://www.khronos.org/opengles/sdk/docs/man3/html/glDrawElementsInstanced.xhtml
/// https://www.khronos.org/opengles/sdk/docs/man3/html/glVertexAttribDivisor.xhtml
/// https://www.khronos.org/opengles/sdk/docs/man3/html/glEnableVertexAttribArray.xhtml
/// http://learnopengl.com/#!Advanced-OpenGL/Instancing
/// ***EDIT***
/// - Renderable created	 	- David Azouz 10/05/16
/// - .cpp removed, ref to Sampler added  	- David Azouz 24/05/16
/// -  	- David Azouz 6/05/16
/// 
/// TODO: 
/// Reff to Mesh
/// Ref to Textures
/// ref to mesh = Mesh* - maybe? [] - hold onto different submeshes - FBX models
///	ref to textures[]
///	ref to shader prog(which is) m_programID
/// </summary>
/// ----------------------------------------------------------
#pragma once

//#include "Helpers.h"

#include "Mesh.h"
#include "Sampler.h"

#include "gl_core_4_4.h"

#include <bitset>
//#include <memory>
#include <vector>

//class Mesh;

struct RenderableBatch
{
	RenderableBatch() = default;
	RenderableBatch(const GLuint& a_program_ID, const Mesh& a_mesh, const GLint a_instances) :
		RenderableBatch(a_program_ID, a_mesh, std::vector<Sampler>(), a_instances)
	{}

	RenderableBatch(const GLuint& a_program_ID, const Mesh& a_mesh, const std::vector<Sampler>& a_samplers, const GLint a_instances) :
		program_ID(a_program_ID),
		mesh(a_mesh),
		samplers(a_samplers),
		instances(a_instances)
	{}

	GLuint program_ID; //Program program;
	Mesh mesh;
	std::vector<Sampler> samplers;
	GLenum renderMode = GL_FILL;
	GLint instances; // how many instances we are drawing
	std::vector<GLuint> instanceBuffers; // Plug in buffers

	/* 
	Need a way to define properties such as: 
	func
	glEnableVertexArrtibArray()
	attribute location to bind to, 
	vertex array (VAO), 

	//pointer to bind it to, 
	*/

	//TODO: 
	/*ref to mesh = Mesh* - maybe? [] - hold onto different submeshes - FBX models
	ref to textures[]
	ref to shader prog (which is) m_programID*/

	// TODO: which? const GLuint& m_program_ID;

	// TODO: replace all my draw calls with one of these?
	// Draw multiple instances of a submesh in one call
	// - 
	// glDrawElementsBaseVertex();

	// Allows drawing of 'N' instances of a SUBSET of indexed vertices in one call.
	// pass in the visible instances.
	// - 
	//glDrawElementsInstancedBaseVertex();

	// Buckets - draw items that are the same - sort into buckets
	//TODO: Bucket sorter - a bucket is a batch
	// a batch is whatever we can squeeze into a single draw call (a batch is a draw call)
	//GPU-based instancing
};

