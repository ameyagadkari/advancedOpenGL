#pragma once
#include <vector>
#include "../../External/cyCodeBase/cyTriMesh.h"

typedef unsigned int GLenum;

namespace cy
{
	template <GLenum TEXTURE_TYPE>
	class GLTexture2;
	template <GLenum TEXTURE_TYPE>
	class GLTextureCubeMap;
}

namespace cs6610
{
	namespace Graphics
	{
		class UniformBuffer;
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
			Material(const bool i_isCubeMap, const cyTriMesh& i_meshData, const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_texturePaths);
			~Material();
			Effect* GetEffect()const;
			void Bind()const;
		//private:
#define GL_TEXTURE_2D 0x0DE1
			cy::GLTexture2<GL_TEXTURE_2D>* m_textures;
#define GL_TEXTURE_CUBE_MAP 0x8513
			cy::GLTextureCubeMap<GL_TEXTURE_CUBE_MAP>* m_texturesCubeMap;
			UniformBuffer *m_materialBuffer;
			Effect* m_effect;
			size_t m_numbeOfTextures;
			bool m_isCubeMap;
		};
	}
}