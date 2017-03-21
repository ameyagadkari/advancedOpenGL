#include "Effect.h"

#include "../../External/cyCodeBase/cyGL.h"
#include "../Asserts/Asserts.h"

std::list<cs6610::Graphics::Effect*> cs6610::Graphics::Effect::allActiveEffects;

void cs6610::Graphics::Effect::Bind()const
{
	m_program->Bind();
}

cs6610::Graphics::Effect::Effect(const std::vector<std::string> i_shaderPaths)
	:
	m_program(new cyGLSLProgram()),
	m_shaderPaths(i_shaderPaths)
{
	Compile();
	allActiveEffects.push_back(this);
}

cs6610::Graphics::Effect::~Effect()
{
	if (m_program)
	{
		m_program->Delete();
		delete m_program;
		m_program = nullptr;
	}
	allActiveEffects.remove(this);
}

cy::GLSLProgram * cs6610::Graphics::Effect::GetProgram() const
{
	return m_program;
}
/*cs6610::Graphics::Effect::Effect(): 
m_program(nullptr)
{
}*/

void cs6610::Graphics::Effect::RegisterUniforms(const char * const i_names) const
{
	m_program->RegisterUniforms(i_names);
}

void cs6610::Graphics::Effect::Compile() const
{
	if (!m_program->BuildFiles(m_shaderPaths[0].c_str(), m_shaderPaths[1].c_str()))
	{
		CS6610_ASSERTF(false, "Failed to build shaders and create program");
	}
}

void cs6610::Graphics::Effect::ReCompile()
{
	for (auto it = allActiveEffects.begin(); it != allActiveEffects.end(); ++it)
	{
		(*it)->Compile();
	}
}
