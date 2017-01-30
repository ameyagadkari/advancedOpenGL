#pragma once
#include <string>

typedef unsigned int GLuint;

namespace cy
{
	class TriMesh;
}

namespace cs6610
{
	namespace Graphics
	{
		class Mesh
		{
		public:			
			void RenderMesh()const;
			explicit Mesh(std::string i_relativePath);
			~Mesh();
		private:
			uint32_t m_numberOfVertices;
			bool Initialize(const cy::TriMesh& i_meshData);
			bool CleanUp();
			GLuint m_vertexArrayId;
			GLuint m_vertexBufferId;
		};
	}
}