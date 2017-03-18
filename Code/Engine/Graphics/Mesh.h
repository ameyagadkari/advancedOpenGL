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
			void RenderMesh(size_t meshID = 0)const;
			explicit Mesh(const cyTriMesh& i_meshData, cy::Point3f &o_minBounds, cy::Point3f &o_maxBounds);
			~Mesh();
		private:
			struct sMesh
			{
				int startIndex;
				int m_numberOfVertices;
			};
			size_t m_numberOfMeshes;
			int m_totalNumberOfVertices;
			sMesh* m_subMeshes;
			bool Initialize();
			bool CleanUp();
			GLuint m_vertexArrayId;
			GLuint m_vertexBufferId;
		};
	}
}