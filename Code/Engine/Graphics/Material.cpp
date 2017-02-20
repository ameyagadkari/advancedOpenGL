#include "Material.h"

#include "../../External/cyCodeBase/cyGL.h"
#include "../Asserts/Asserts.h"
#include "Effect.h"
#include "../../External/PNGLoader/lodepng.h"
#include "UniformBuffer.h"
#include "UniformBufferData.h"

cs6610::Graphics::Material::Material(const cyTriMesh& i_meshData, const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_texturePaths) :
	m_materialBuffer(nullptr),
	m_effect(new Effect(i_shaderPaths)),
	m_numbeOfTextures(i_texturePaths.size())
{
	m_textures = m_numbeOfTextures > 0 ? new cyGLTexture2D[m_numbeOfTextures] : nullptr;
	for (size_t i = 0; i < m_numbeOfTextures; i++)
	{
		m_textures[i].Initialize();
		const GLenum wrapModeForTextures = GL_REPEAT;
		m_textures[i].SetWrappingMode(wrapModeForTextures, wrapModeForTextures);

		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, i_texturePaths[i].c_str());
		CS6610_ASSERTF(!error, "Decoder error %d: %s", error, lodepng_error_text(error));
		m_textures[i].SetImage(&image[0], GL_RGBA, GL_RGBA, width, height);
		m_textures[i].BuildMipmaps();
	}
	UniformBufferData::MaterialBuffer materialBufferData(i_meshData.M(0).Ka, i_meshData.M(0).Kd, i_meshData.M(0).Ks, i_meshData.M(0).Ns);
	m_materialBuffer = new UniformBuffer(UniformBufferType::MATERIAL, sizeof(materialBufferData), &materialBufferData);
}

cs6610::Graphics::Material::~Material()
{
	if (m_effect)
	{
		delete m_effect;
		m_effect = nullptr;
	}
	if (m_materialBuffer)
	{
		delete m_materialBuffer;
		m_materialBuffer = nullptr;
	}
	if (m_textures)
	{
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
	m_materialBuffer->Bind();
	m_effect->Bind();
	for (size_t i = 0; i < m_numbeOfTextures; i++)
	{
		m_textures->Bind(static_cast<int>(i));
	}
}