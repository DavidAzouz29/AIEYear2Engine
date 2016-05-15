#pragma once

#include "Helpers.h"

#include "gl_core_4_4.h"

#include <memory>

class Mesh;
class Texture;

class Renderable
{
public:
	Renderable();
	~Renderable();

	//TODO: 
	/*ref to mesh = Mesh* - maybe? [] - hold onto different submeshes - FBX models
	ref to textures[]
	ref to shader prog (which is) m_programID*/

private:
	GLuint* m_program_ID;
	// TODO: which? const GLuint& m_program_ID;

	// TODO: replace all my draw calls with one of these?
	// Draw multiple instances of a submesh in one call
	// glDrawElementsBaseVertex();

	// Allows drawing of 'N' instances of a SUBSET of indexed vertices in one call.
	// pass in the visible instances.
	//glDrawElementsInstancedBaseVertex();

	// Buckets - draw items that are the same - sort into buckets
	//TODO: Bucket sorter - a bucket is a batch
	// a batch is whatever we can squeeze into a single draw call (a batch is a draw call)
	//GPU-based instancing
};

