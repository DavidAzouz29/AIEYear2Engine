#pragma once
#include "Entity\Entity.h"

class FBXFile;
class Camera;

class FBXModel : public Entity
{
public:
	FBXModel();
	virtual ~FBXModel();

	bool Create();
	void Update();
	void Draw();
	void Destroy();
	void RenderUI();

	/// ----------------------------------------------------------
	void FBXLoader();
	void RenderFBX(Camera* cam);
	void FBXSkeletonLoader();
	void FBXSkeletonRender();
	void FBXUpdate();
	void FBXDraw();
	/// ----------------------------------------------------------

private:
	/// ----------------------------------------------------------
	/// FBXLoader
	/// ----------------------------------------------------------
	void CreateOpenGLBuffers(FBXFile* fbx);
	void CleanupOpenGLBuffers(FBXFile* fbx);
	GLfloat m_timer;

	GLuint m_program_ID;

	std::shared_ptr<FBXFile> m_pFbx;
	GLuint m_FBX_program_ID;

};

