#include "Effect.h"

#include "../../External/cyCodeBase/cyGL.h"
#include "../Asserts/Asserts.h"

void cs6610::Graphics::Effect::Bind()const
{
	m_program->Bind();
}

cs6610::Graphics::Effect::Effect(const std::string i_relativePathVertex, const std::string i_relativePathFragment)
	:
m_program(new cyGLSLProgram())
{
	CS6610_ASSERTF(m_program->BuildFiles(i_relativePathVertex.c_str(), i_relativePathFragment.c_str()), "Failed to build shaders and create program");
}

cs6610::Graphics::Effect::~Effect()
{
	if (m_program)
	{
		delete m_program;
		m_program = nullptr;
	}
}

cy::GLSLProgram * cs6610::Graphics::Effect::GetProgram() const
{
	return m_program;
}

void cs6610::Graphics::Effect::RegisterUniforms(const char * const i_names) const
{
	m_program->RegisterUniforms(i_names);
}
