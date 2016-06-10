#include "Renderable.h"
#include "Camera.h"
//Renderable(const Program& a_program, const Mesh& a_mesh) :

Renderable::Renderable(const GLuint & a_program_ID, const Mesh & a_mesh) :
	Renderable(a_program_ID, a_mesh, std::vector<Sampler>())
{}

Renderable::Renderable(const GLuint & a_program_ID, const Mesh & a_mesh, const std::vector<Sampler>& a_samplers) :
	program_ID(a_program_ID),
	mesh(a_mesh),
	samplers(a_samplers)
{}

//TODO: HELP - is this where this should be done?
GLvoid Renderable::Draw(const Camera& a_pCamState)
{
	// use our texture program
	glUseProgram(program_ID);

	// bind the camera
	int loc = glGetUniformLocation(program_ID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &a_pCamState.getProjectionView()[0][0]);

	// unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	// unsigned int uiTime = glGetUniformLocation(m_programID, "time");
	// 
	// glUniform1f(uiHeightScale, fHeightScale);
	// glUniform1f(uiTime, fTime);

	glBindTexture(GL_TEXTURE_2D, TextureManager::GetSingleton().LoadTexture("./data/textures/crate.png")->GetId()); //TODO: clean this up?

	// tell the shader where it is
	glActiveTexture(GL_TEXTURE4);
	loc = glGetUniformLocation(program_ID, "diffuse");
	glUniform1i(loc, 4);

	// draw
	glBindVertexArray(mesh.GetVAO()); //TODO: replace m_VAO with VAO
							  //glBindVertexArray(Gem->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// TODO: find by name i.e. "soulspear_n"
std::shared_ptr<Texture> Renderable::GetTextureByPath(const GLchar* a_path)
{
	for (auto &pSamplers : samplers)
	{
		// Compares against Texture in Renderable/ that we have.
		if (pSamplers.tTexture->GetPath() == a_path)
		{
			return pSamplers.tTexture;
		}
	}
	return nullptr;
}
