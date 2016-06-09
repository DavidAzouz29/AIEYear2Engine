#include "Renderable.h"
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
