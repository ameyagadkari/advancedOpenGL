#pragma once
#include <cstdint>
#include <vector>

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
			Material(const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_texturePaths);
			~Material();
			Effect* GetEffect()const;
			void Bind()const;
		private:	
#define GL_TEXTURE_2D 0x0DE1
			cy::GLTexture<GL_TEXTURE_2D>* m_textures;
			Effect* m_effect;
		};
	}
}