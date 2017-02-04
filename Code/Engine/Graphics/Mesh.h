#pragma once
#include <string>
#include "../../External/cyCodeBase/cyPoint.h"

typedef unsigned int GLuint;

namespace cs6610
{
	namespace Graphics
	{
		class Mesh
		{
		public:
			void RenderMesh()const;
			explicit Mesh(const std::string i_relativePath, cy::Point3f &o_minBounds, cy::Point3f &o_maxBounds);
			~Mesh();
		private:
			uint32_t m_numberOfVertices;
			bool Initialize();
			bool CleanUp();
			GLuint m_vertexArrayId;
			GLuint m_vertexBufferId;
		};
	}
}