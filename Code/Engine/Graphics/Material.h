#pragma once
#include <vector>
#include "../../External/cyCodeBase/cyTriMesh.h"

typedef unsigned int GLenum;

namespace cy
{
	template <GLenum TEXTURE_TYPE>
	class GLTexture;
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
			Material(const cyTriMesh& i_meshData, const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_texturePaths);
			~Material();
			Effect* GetEffect()const;
			void Bind()const;
		private:
#define GL_TEXTURE_2D 0x0DE1
			cy::GLTexture<GL_TEXTURE_2D>* m_textures;
			UniformBuffer *m_materialBuffer;
			Effect* m_effect;
			size_t m_numbeOfTextures;
		};
	}
}