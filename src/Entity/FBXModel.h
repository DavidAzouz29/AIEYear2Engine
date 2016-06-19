#pragma once
#include "Entity\Entity.h"

class FBXFile;
class Camera;

class FBXModel : public Entity
{
public:
	//FBXModel() = default;
	FBXModel(const GLchar* szFileName);
	virtual ~FBXModel();

	bool Create() override;
	bool Update(GLfloat deltaTime) override;
	GLvoid Draw(const Camera& m_pCamState) override;
	//GLvoid Destroy() override;
	GLvoid RenderUI() override;

	GLvoid LoadFBXTextures(FBXFile* fbx);

	/// ----------------------------------------------------------
	GLvoid FBXSkeletonLoader();
	GLvoid FBXSkeletonRender();
	GLvoid FBXAnimationDraw(const Camera& a_pCamState);
	/// ----------------------------------------------------------

private:
	/// ----------------------------------------------------------
	/// FBXLoader
	/// ----------------------------------------------------------
	GLvoid CreateOpenGLBuffers(FBXFile* fbx);
	GLvoid CleanupOpenGLBuffers(FBXFile* fbx);
	GLfloat m_timer;

	std::shared_ptr<FBXFile> m_pFbx;
	GLuint m_program_FBXAnimation_ID; // another program ID used for animation

};

