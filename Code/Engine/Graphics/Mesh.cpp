#include "Mesh.h"

#define GLEW_STATIC
#include "../../External/GLEW/glew.h"
#include "../Asserts/Asserts.h"
#include "MeshData.h"

#include "../../External/cyCodeBase/cyTriMesh.h"

namespace
{
	cyTriMesh* s_meshData = nullptr;
	cs6610::Graphics::MeshData* s_meshData_inner = nullptr;
}
cs6610::Graphics::Mesh::Mesh(const std::string i_relativePath, cy::Point3f &o_minBounds, cy::Point3f &o_maxBounds) :
	m_vertexArrayId(0),
	m_vertexBufferId(0)
{
	s_meshData = new cyTriMesh();
	s_meshData->LoadFromFileObj(i_relativePath.c_str());
	m_numberOfVertices = s_meshData->NF() * 3;
	s_meshData_inner = new MeshData(m_numberOfVertices);
	size_t length = s_meshData->NF();
	size_t index = 0;
	for (size_t i = 0; i < length; i++)
	{
		int ii = static_cast<int>(i);
		s_meshData_inner->vertexData[index].AddVertexData(s_meshData->V(s_meshData->F(ii).v[0]).x, s_meshData->V(s_meshData->F(ii).v[0]).y, s_meshData->V(s_meshData->F(ii).v[0]).z);
		++index;
		s_meshData_inner->vertexData[index].AddVertexData(s_meshData->V(s_meshData->F(ii).v[1]).x, s_meshData->V(s_meshData->F(ii).v[1]).y, s_meshData->V(s_meshData->F(ii).v[1]).z);
		++index;
		s_meshData_inner->vertexData[index].AddVertexData(s_meshData->V(s_meshData->F(ii).v[2]).x, s_meshData->V(s_meshData->F(ii).v[2]).y, s_meshData->V(s_meshData->F(ii).v[2]).z);
		++index;
	}
	Initialize();
	s_meshData->ComputeBoundingBox();
	o_minBounds = s_meshData->GetBoundMin();
	o_maxBounds = s_meshData->GetBoundMax();
	if (s_meshData)
	{
		delete s_meshData;
		s_meshData = nullptr;
	}
	if (s_meshData_inner)
	{
		delete s_meshData_inner;
		s_meshData_inner = nullptr;
	}
}

cs6610::Graphics::Mesh::~Mesh()
{
	if (!CleanUp())
	{
		CS6610_ASSERTF(false, "Mesh cleanup failed");
	}
}

bool cs6610::Graphics::Mesh::Initialize()
{
	bool wereThereErrors = false;
	// Create a vertex array object and make it active
	{
		const GLsizei arrayCount = 1;
		glGenVertexArrays(arrayCount, &m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindVertexArray(m_vertexArrayId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				CS6610_ASSERTF(false, "OpenGL failed to bind the vertex array: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to get an unused vertex array ID: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Create a vertex buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				CS6610_ASSERTF(false, "OpenGL failed to bind the vertex buffer: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to get an unused vertex buffer ID: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Assign the data to the buffer
	{
		//Vextex Buffer init
		const unsigned int vertexBufferSize = m_numberOfVertices * sizeof(MeshData::Vertex);
		if (m_numberOfVertices > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, reinterpret_cast<GLvoid*>(s_meshData_inner->vertexData), GL_STATIC_DRAW);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				CS6610_ASSERTF(false, "OpenGL failed to allocate the vertex buffer with data: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to allocate the vertex buffer because there is no Vertex Data");
			goto OnExit;
		}
	}

	// Initialize Vertex Array Object
	{
		const GLsizei stride = sizeof(MeshData::Vertex);
		// Position at 0
		{
			const GLuint vertexElementLocation = 0;
			const GLint elementCount = 3;
			const GLboolean isNormalized = GL_FALSE;

			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, isNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(MeshData::Vertex, x)));

			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					CS6610_ASSERTF(false, "OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				CS6610_ASSERTF(false, "OpenGL failed to set the POSITION vertex attribute at location %u: %s", vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
	}

OnExit:

	if (wereThereErrors && m_vertexArrayId != 0)
	{
		glBindVertexArray(0);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			if (wereThereErrors)
			{
				CleanUp();
			}
		}
		else
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to unbind the vertex array: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
			m_vertexArrayId = 0;
			goto OnExit;
		}
	}

	return !wereThereErrors;
}

bool cs6610::Graphics::Mesh::CleanUp()
{
	bool wereThereErrors = false;
	if (m_vertexBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to delete the vertex buffer: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexBufferId = 0;
	}

	if (m_vertexArrayId != 0)
	{
		const GLsizei arrayCount = 1;
		glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			CS6610_ASSERTF(false, "OpenGL failed to delete the vertex array: %s", reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexArrayId = 0;
	}
	return !wereThereErrors;
}

void cs6610::Graphics::Mesh::RenderMesh()const
{
	// Binding the VAO
	{
		glBindVertexArray(m_vertexArrayId);
		CS6610_ASSERT(glGetError() == GL_NO_ERROR);
	}

	//Drawing data as points
	{
		const GLenum mode = GL_TRIANGLES;
		const GLint first = 0;
		glDrawArrays(mode, first, m_numberOfVertices);
		CS6610_ASSERT(glGetError() == GL_NO_ERROR);
	}

	// Unbinding the VAO
	{
		glBindVertexArray(0);
		CS6610_ASSERT(glGetError() == GL_NO_ERROR);
	}
}