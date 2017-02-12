#pragma once
#include "../../External/cyCodeBase/cyPoint.h"
#include "../../External/cyCodeBase/cyTriMesh.h"

typedef unsigned int GLuint;

namespace cs6610
{
	namespace Graphics
	{
		class Mesh
		{
		public:
			void RenderMesh()const;
			explicit Mesh(const cyTriMesh& i_meshData, cy::Point3f &o_minBounds, cy::Point3f &o_maxBounds);
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