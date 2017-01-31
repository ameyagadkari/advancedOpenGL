#include "Effect.h"

#include "../../External/cyCodeBase/cyGL.h"
#include "../Asserts/Asserts.h"

std::list<cs6610::Graphics::Effect*> cs6610::Graphics::Effect::allActiveEffects;

void cs6610::Graphics::Effect::Bind()const
{
	m_program->Bind();
}

cs6610::Graphics::Effect::Effect(const std::string i_relativePathVertex, const std::string i_relativePathFragment)
	:
m_program(new cyGLSLProgram()),
m_relativePathVertex(i_relativePathVertex),
m_relativePathFragment(i_relativePathFragment)
{
	Compile();
	allActiveEffects.push_back(this);
}

cs6610::Graphics::Effect::~Effect()
{
	if (m_program)
	{
		delete m_program;
		m_program = nullptr;
	}
	allActiveEffects.remove(this);
}

cy::GLSLProgram * cs6610::Graphics::Effect::GetProgram() const
{
	return m_program;
}

void cs6610::Graphics::Effect::RegisterUniforms(const char * const i_names) const
{
	m_program->RegisterUniforms(i_names);
}

void cs6610::Graphics::Effect::Compile(void) const
{
	if (!m_program->BuildFiles(m_relativePathVertex.c_str(), m_relativePathFragment.c_str()))
	{
		CS6610_ASSERTF(false, "Failed to build shaders and create program");
	}
}

void cs6610::Graphics::Effect::ReCompile(void)
{
	for (auto it = allActiveEffects.begin(); it != allActiveEffects.end(); ++it)
	{
		(*it)->Compile();
	}
}
