#include "Gameobject.h"

#include "../../Engine/Graphics/Mesh.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Graphics/Material.h"
#include "../../Engine/Graphics/Effect.h"
#include "../../Engine/Time/Time.h"
#include "../../External/cyCodeBase/cyTriMesh.h"

namespace
{
	cyTriMesh* s_meshData = nullptr;
}

cs6610::Gameplay::GameObject::GameObject(const cyPoint3f i_position, const cyPoint3f i_eularAngles, const cyPoint3f i_scale) :
	m_mesh(nullptr),
	m_material(nullptr),
	m_position(i_position),
	m_eularAngles(i_eularAngles),
	m_scale(i_scale)
{}
cs6610::Gameplay::GameObject::~GameObject()
{
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
	if (m_material)
	{
		delete m_material;
		m_material = nullptr;
	}
}

void cs6610::Gameplay::GameObject::LoadMeshAndMaterial(const bool i_isCubeMap, const std::string i_meshRelativePath, const std::vector<std::string> i_shaderPaths, const std::vector<std::string> i_texturePaths, const char * const i_names)
{
	s_meshData = new cyTriMesh();
	s_meshData->LoadFromFileObj(i_meshRelativePath.c_str());
	s_meshData->ComputeNormals();
	s_meshData->ComputeBoundingBox();

	// Loading the mesh
	if (!m_mesh)
	{
		cyPoint3f minBounds, maxBounds;
		m_mesh = new Graphics::Mesh(*s_meshData, minBounds, maxBounds);
		m_position += (maxBounds + minBounds) *0.5f;
	}
	else
	{
		CS6610_ASSERTF(false, "Use the other overload to set mesh");
	}

	// Loading the material
	if (!m_material)
	{
		m_material = new Graphics::Material(i_isCubeMap, *s_meshData, i_shaderPaths, i_texturePaths);
		if (i_names)
		{
			m_material->GetEffect()->RegisterUniforms(i_names);
		}
	}
	else
	{
		CS6610_ASSERTF(false, "Use the other overload to set effect");
	}

	if (s_meshData)
	{
		delete s_meshData;
		s_meshData = nullptr;
	}
}

void cs6610::Gameplay::GameObject::UpdatePosition(float yOffset)
{
	cyPoint3f localOffset = cyPoint3f(0.0f);

	localOffset.y += yOffset;

	const float speed_unitsPerSecond = 10.0f;
	const float offsetModifier = speed_unitsPerSecond * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame());
	localOffset *= offsetModifier;
	m_position += localOffset;
}

void cs6610::Gameplay::GameObject::UpdateOrientation(float xOffset, float yOffset)
{
	cyPoint3f localOffset = cyPoint3f(0.0f);

	localOffset.z += xOffset;
	localOffset.x += yOffset;

	const float speed_unitsPerSecond = 10.0f;
	const float offsetModifier = speed_unitsPerSecond * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame());
	localOffset *= offsetModifier;
	m_eularAngles += localOffset;
}

#pragma region Gets
cs6610::Graphics::Material* cs6610::Gameplay::GameObject::GetMaterial() const
{
	return m_material;
}
cs6610::Graphics::Mesh* cs6610::Gameplay::GameObject::GetMesh()const
{
	return m_mesh;
}
cyPoint3f cs6610::Gameplay::GameObject::GetPosition()const
{
	return m_position;
}
cyPoint3f cs6610::Gameplay::GameObject::GetOrientationEular()const
{
	return m_eularAngles;
}
cyPoint3f cs6610::Gameplay::GameObject::GetScale() const
{
	return m_scale;
}
#pragma endregion

#pragma region Sets
void cs6610::Gameplay::GameObject::SetMaterial(Graphics::Material* const i_material, const char* const i_names)
{
	if (m_material)
	{
		delete m_material;
		m_material = nullptr;
		m_material = i_material;
		if (i_names)
		{
			m_material->GetEffect()->RegisterUniforms(i_names);
		}
	}
	else
	{
		CS6610_ASSERTF(false, "Use the other overload to set effect");
	}
}
void cs6610::Gameplay::GameObject::SetMesh(Graphics::Mesh* const i_mesh)
{
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = nullptr;
		m_mesh = i_mesh;
	}
	else
	{
		CS6610_ASSERTF(false, "Use the other overload to set mesh");
	}
}
void cs6610::Gameplay::GameObject::SetPosition(const cyPoint3f i_position)
{
	m_position = i_position;
}
void cs6610::Gameplay::GameObject::SetOrientationEular(const cyPoint3f i_eularAngles)
{
	m_eularAngles = i_eularAngles;
}
#pragma endregion 