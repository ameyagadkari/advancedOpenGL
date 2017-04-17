#include "Material.h"

#include "../../External/cyCodeBase/cyGL.h"
#include "../Asserts/Asserts.h"
#include "Effect.h"
#include "../../External/PNGLoader/lodepng.h"
#include "UniformBuffer.h"
#include "UniformBufferData.h"
#include "../Math/BitManipulator.h"

cs6610::Graphics::Material::Material(const bool i_isCubeMap, const cyTriMesh& i_meshData, const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_skyBoxTextures, const std::string i_texturePathPrefix) :
	m_texturesCubeMap(nullptr),
	m_effect(new Effect(i_shaderPaths)),
	m_numbeOfMaterials(static_cast<size_t>(i_meshData.NM())),
	m_isCubeMap(i_isCubeMap)
{
	m_subMaterials = m_numbeOfMaterials > 0 ? new sMaterial[m_numbeOfMaterials] : nullptr;
	uint32_t textureUnitMask = 0;

	for (size_t i = 0; i < m_numbeOfMaterials; i++)
	{
		int ii = static_cast<int>(i);
		std::map<const int, const std::string> textureMapNames;
		if (i_meshData.M(ii).map_Kd.data)
		{
			m_subMaterials[i].m_numbeOfTextures++;
			m_subMaterials[i].m_textures.emplace(0, new cyGLTexture2D());
			textureMapNames.emplace(0, i_meshData.M(ii).map_Kd.data);
			Math::BitManipulator::SetBit(textureUnitMask, 0);
		}
		if (i_meshData.M(ii).map_Ks.data)
		{
			m_subMaterials[i].m_numbeOfTextures++;
			m_subMaterials[i].m_textures.emplace(1, new cyGLTexture2D());
			textureMapNames.emplace(1, i_meshData.M(ii).map_Ks.data);
			Math::BitManipulator::SetBit(textureUnitMask, 1);
		}
		if (i_meshData.M(ii).map_bump.data)
		{
			m_subMaterials[i].m_numbeOfTextures++;
			m_subMaterials[i].m_textures.emplace(2, new cyGLTexture2D());
			textureMapNames.emplace(2, i_meshData.M(ii).map_bump.data);
			Math::BitManipulator::SetBit(textureUnitMask, 2);
		}
		if (i_meshData.M(ii).map_Ns.data)
		{
			m_subMaterials[i].m_numbeOfTextures++;
			m_subMaterials[i].m_textures.emplace(3, new cyGLTexture2D());
			textureMapNames.emplace(3, i_meshData.M(ii).map_Ns.data);
			Math::BitManipulator::SetBit(textureUnitMask, 3);
		}
		if (i_meshData.M(ii).map_d.data)
		{
			m_subMaterials[i].m_numbeOfTextures++;
			m_subMaterials[i].m_textures.emplace(4, new cyGLTexture2D());
			textureMapNames.emplace(4, i_meshData.M(ii).map_d.data);
			Math::BitManipulator::SetBit(textureUnitMask, 4);
		}
		if (i_meshData.M(ii).map_disp.data)
		{
			m_subMaterials[i].m_numbeOfTextures++;
			m_subMaterials[i].m_textures.emplace(5, new cyGLTexture2D());
			textureMapNames.emplace(5, i_meshData.M(ii).map_disp.data);
			Math::BitManipulator::SetBit(textureUnitMask, 5);
		}
		if (!i_isCubeMap)
		{
			for (auto const& texture : m_subMaterials[i].m_textures)
			{
				cyGLTexture2D* tempTexture = const_cast<cyGLTexture2D*>(texture.second);
				tempTexture->Initialize();
				std::vector<unsigned char> image;
				unsigned width, height;
				unsigned error = lodepng::decode(image, width, height, i_texturePathPrefix + textureMapNames[texture.first]);
				CS6610_ASSERTF(!error, "Decoder error %d: %s", error, lodepng_error_text(error));
				tempTexture->SetImage(&image[0], 4, width, height);
				tempTexture->BuildMipmaps();
			}
		}
		UniformBufferData::MaterialBuffer materialBufferData(i_meshData.M(ii).Ka, i_meshData.M(ii).Kd, i_meshData.M(ii).Ks, i_meshData.M(ii).Ns, textureUnitMask);
		m_subMaterials[i].m_materialBuffer = new UniformBuffer(UniformBufferType::MATERIAL, sizeof(materialBufferData), &materialBufferData);
	}
	if (i_isCubeMap)
	{
		m_texturesCubeMap = new cyGLTextureCubeMap();
		m_texturesCubeMap->Initialize();
		for (size_t j = 0; j < 6; j++)
		{
			std::vector<unsigned char> image;
			unsigned width, height;
			unsigned error = lodepng::decode(image, width, height, i_skyBoxTextures[j].c_str()); CS6610_ASSERTF(!error, "Decoder error %d: %s", error, lodepng_error_text(error));
			m_texturesCubeMap->SetImage(static_cast<cyGLTextureCubeMap::Side>(j), &image[0], 4, width, height);
		}
		m_texturesCubeMap->SetSeamless();
	}
	/*if (m_numbeOfTextures > 0)
	{
		if (!i_isCubeMap)
		{
			m_textures = new cyGLTexture2D[m_numbeOfTextures];
			for (size_t i = 0; i < m_numbeOfTextures; i++)
			{
				m_textures[i].Initialize();
				//const GLenum wrapModeForTextures = GL_REPEAT;
				//m_textures[i].SetWrappingMode(wrapModeForTextures, wrapModeForTextures);
				std::vector<unsigned char> image;
				unsigned width, height;
				unsigned error = lodepng::decode(image, width, height, i_texturePaths[i].c_str());
				CS6610_ASSERTF(!error, "Decoder error %d: %s", error, lodepng_error_text(error));
				m_textures[i].SetImage(&image[0], 4, width, height);
				m_textures[i].BuildMipmaps();
			}
		}
		else
		{
			m_textures = nullptr;
			m_texturesCubeMap = new cyGLTextureCubeMap();
			m_texturesCubeMap->Initialize();
			for (size_t i = 0; i < m_numbeOfTextures; i++)
			{
				std::vector<unsigned char> image;
				unsigned width, height;
				unsigned error = lodepng::decode(image, width, height, i_texturePaths[i].c_str());
				CS6610_ASSERTF(!error, "Decoder error %d: %s", error, lodepng_error_text(error));
				m_texturesCubeMap->SetImage(static_cast<cyGLTextureCubeMap::Side>(i), &image[0], 4, width, height);
			}
			m_texturesCubeMap->SetSeamless();
		}
	}
	if (i_meshData.NM() == 1)
	{
		UniformBufferData::MaterialBuffer materialBufferData(i_meshData.M(0).Ka, i_meshData.M(0).Kd, i_meshData.M(0).Ks, i_meshData.M(0).Ns);
		m_materialBuffer = new UniformBuffer(UniformBufferType::MATERIAL, sizeof(materialBufferData), &materialBufferData);
	}*/
}

cs6610::Graphics::Material::~Material()
{
	if (m_effect)
	{
		delete m_effect;
		m_effect = nullptr;
	}
	if (m_subMaterials)
	{
		delete[] m_subMaterials;
		m_subMaterials = nullptr;
	}
	if (m_texturesCubeMap)
	{
		delete m_texturesCubeMap;
		m_texturesCubeMap = nullptr;
	}
}

cs6610::Graphics::Effect * cs6610::Graphics::Material::GetEffect() const
{
	return m_effect;
}

size_t cs6610::Graphics::Material::GetNumberOfMaterials() const
{
	return m_numbeOfMaterials;
}

void cs6610::Graphics::Material::Bind(size_t materialID)const
{
	if (materialID == 0)
	{
		m_effect->Bind();
	}
	if (m_subMaterials)
	{
		if (m_subMaterials[materialID].m_materialBuffer)m_subMaterials[materialID].m_materialBuffer->Bind();
	}
	if (!m_isCubeMap && m_numbeOfMaterials)
	{
		for (auto const& texture : m_subMaterials[materialID].m_textures)
		{
			texture.second->Bind(texture.first);
		}
	}
	else
	{
		if (m_texturesCubeMap)m_texturesCubeMap->Bind();
	}
}