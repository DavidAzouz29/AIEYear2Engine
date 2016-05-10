#pragma once
#include "Entity\Entity.h"

class FBXFile;
class Camera;

class FBXModel : public Entity
{
public:
	FBXModel();
	virtual ~FBXModel();

	bool Create() override;
	GLvoid Update() override;
	GLvoid Draw(const Camera& m_pCamState) override;
	GLvoid Destroy() override;
	GLvoid RenderUI() override;

	/// ----------------------------------------------------------
	GLvoid FBXLoader();
	GLvoid RenderFBX(Camera* cam);
	GLvoid FBXSkeletonLoader();
	GLvoid FBXSkeletonRender();
	//GLvoid FBXUpdate();
	GLvoid FBXAnimationDraw(const Camera& m_pCamState);
	/// ----------------------------------------------------------

private:
	/// ----------------------------------------------------------
	/// FBXLoader
	/// ----------------------------------------------------------
	GLvoid CreateOpenGLBuffers(FBXFile* fbx);
	GLvoid CleanupOpenGLBuffers(FBXFile* fbx);
	GLfloat m_timer;

	GLuint m_program_ID;

	std::shared_ptr<FBXFile> m_pFbx;
	GLuint m_program_FBXAnimation_ID;

};

