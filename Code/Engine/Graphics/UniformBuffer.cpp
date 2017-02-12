#include "UniformBuffer.h"

#define GLEW_STATIC
#include "../../External/GLEW/glew.h"
#include "../Asserts/Asserts.h"

cs6610::Graphics::UniformBuffer::UniformBuffer(const UniformBufferType i_uniformBufferType, const size_t i_uniformBufferSize, const void * const i_initialBufferData) :
	m_uniformBufferType(i_uniformBufferType),
	m_uniformBufferSize(i_uniformBufferSize)
{
	if (!Initialize(i_initialBufferData))
	{
		CS6610_ASSERTF(false, "Failed to create uniform buffer of type %d", m_uniformBufferType);
	}
}

bool cs6610::Graphics::UniformBuffer::Initialize(const void * const i_initialBufferData)
{
	bool wereThereErrors = false;

	// Create a uniform buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_uniformBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				CS6610_ASSERTF(false, "OpenGL failed to bind the new uniform buffer %u: %s", m_uniformBufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to get an unused uniform buffer ID: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Allocate space and copy the constant data into the uniform buffer
	{
		glBufferData(GL_UNIFORM_BUFFER, m_uniformBufferSize, reinterpret_cast<const GLvoid*>(i_initialBufferData), GL_DYNAMIC_DRAW);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to allocate the new uniform buffer %u: %s", m_uniformBufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

OnExit:
	if (wereThereErrors && m_uniformBufferId != 0)
	{
		CleanUp();
	}
	return !wereThereErrors;
}

cs6610::Graphics::UniformBuffer::~UniformBuffer()
{
	if (!CleanUp())
	{
		CS6610_ASSERTF(false, "Uniform buffer cleanup failed");
	}
}

void cs6610::Graphics::UniformBuffer::Bind() const
{
	const GLuint bindingPointAssignedInShader = static_cast<int>(m_uniformBufferType);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointAssignedInShader, m_uniformBufferId);
	CS6610_ASSERT(glGetError() == GL_NO_ERROR);
}

void cs6610::Graphics::UniformBuffer::Update(const void * const i_newBufferData, size_t i_newUniformBufferSize) const
{
	// Make the uniform buffer active
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferId);
		CS6610_ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Copy the updated memory to the GPU
	{
		GLintptr updateAtTheBeginning = 0;
		GLsizeiptr updateTheEntireBuffer = 0;
		if (i_newUniformBufferSize <= m_uniformBufferSize)
		{
			updateTheEntireBuffer = static_cast<GLsizeiptr>(m_uniformBufferSize);
		}
		else
		{
			CS6610_ASSERTF(false, "Size of the new buffer data is greater than initial size of the buffer data");
		}
		glBufferSubData(GL_UNIFORM_BUFFER, updateAtTheBeginning, updateTheEntireBuffer, reinterpret_cast<const GLvoid*>(i_newBufferData));
		CS6610_ASSERT(glGetError() == GL_NO_ERROR);
	}
}

bool cs6610::Graphics::UniformBuffer::CleanUp()
{
	bool wereThereErrors = false;
	if (m_uniformBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_uniformBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to delete the uniform buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_uniformBufferId = 0;
	}
	return !wereThereErrors;
}