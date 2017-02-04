#pragma once

#include<cstdint>
#include<cstdlib>

namespace cs6610
{
	namespace Graphics
	{
		struct MeshData
		{
			struct Vertex
			{
				float x, y, z;
				void AddVertexData(const float i_x, const float i_y, const float i_z)
				{
					x = i_x;
					y = i_y;
					z = i_z;
				}
			};
			Vertex *vertexData;
			uint32_t numberOfVertices;
			MeshData() :
				vertexData(nullptr),
				numberOfVertices(0)
			{}
			MeshData(const uint32_t i_numberOfVertices) :
				vertexData(reinterpret_cast<Vertex*>(malloc(i_numberOfVertices * sizeof(Vertex)))),
				numberOfVertices(i_numberOfVertices)
			{}
			~MeshData()
			{
				if (vertexData)
				{
					free(vertexData);
					vertexData = nullptr;
				}
			}
		};
	}
}
