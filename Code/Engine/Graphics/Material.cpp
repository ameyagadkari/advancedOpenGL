#include "Material.h"

#define GLEW_STATIC
#include "../../External/GLEW/glew.h"
#include "../../External/cyCodeBase/cyGL.h"
#include "../Asserts/Asserts.h"
#include "Effect.h"
#include "../../External/PNGLoader/lodepng.h"

namespace
{
	size_t length = 0;
}

cs6610::Graphics::Material::Material(const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_texturePaths) :
	m_effect(new Effect(i_shaderPaths))
{
	length = i_texturePaths.size();
	m_textures = length > 0 ? new cyGLTexture2D[length] : nullptr;
	for (size_t i = 0; i < length; i++)
	{
		m_textures[i].Initialize();
		const GLenum wrapModeForTextures = GL_REPEAT;
		m_textures[i].SetWrappingMode(wrapModeForTextures, wrapModeForTextures);

		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, i_texturePaths[i].c_str());
		CS6610_ASSERTF(!error, "Decoder error %d: %s", error, lodepng_error_text(error));
		m_textures[i].SetImage(&image[0], GL_RGB, GL_RGBA, width, height);
		m_textures[i].BuildMipmaps();
	}
}

cs6610::Graphics::Material::~Material()
{
	if (m_effect)
	{
		delete m_effect;
		m_effect = nullptr;
	}
	if (m_textures)
	{
		for (size_t i = 0; i < length; i++)
		{
			m_textures[i].Delete();
		}
		delete[] m_textures;
		m_textures = nullptr;
	}
}

cs6610::Graphics::Effect * cs6610::Graphics::Material::GetEffect() const
{
	return m_effect;
}

void cs6610::Graphics::Material::Bind()const
{
	m_effect->Bind();
	for (size_t i = 0; i < length; i++)
	{
		m_textures->Bind(static_cast<int>(i));
	}
}