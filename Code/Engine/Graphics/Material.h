#pragma once

#include "UniformBuffer.h"

#include "../../External/cyCodeBase/cyTriMesh.h"

#include "../../External/cyCodeBase/cyGL.h"

#include <vector>
#include <map>
typedef unsigned int GLenum;

/*namespace cy
{
	template <GLenum TEXTURE_TYPE>
	class GLTexture2;
	template <GLenum TEXTURE_TYPE>
	class GLTextureCubeMap;
}*/

namespace cs6610
{
	namespace Graphics
	{
		//class UniformBuffer;
		class Effect;
	}
}

namespace cs6610
{
	namespace Graphics
	{
		class Material
		{
		public:
			Material(const bool i_isCubeMap, const cyTriMesh& i_meshData, const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_skyBoxTextures, const std::string i_texturePathPrefix);
			~Material();
			Effect* GetEffect()const;
			size_t GetNumberOfMaterials()const;
			void Bind(size_t materialID = 0)const;
		private:
			struct sMaterial
			{
				std::map<const int, const cyGLTexture2D*> m_textures;
				UniformBuffer *m_materialBuffer;
				size_t m_numbeOfTextures;
				sMaterial() : m_materialBuffer(nullptr), m_numbeOfTextures(0) {}
				~sMaterial()
				{
					if (m_materialBuffer)
					{
						delete m_materialBuffer;
						m_materialBuffer = nullptr;
					}
					for (auto const& texture : m_textures)
					{
						if (texture.second)
						{
							delete texture.second;
						}
					}
					m_textures.clear();
				}
			};
			cyGLTextureCubeMap* m_texturesCubeMap;
			Effect* m_effect;
			sMaterial* m_subMaterials;
			size_t m_numbeOfMaterials;
			bool m_isCubeMap;
		};
	}
}