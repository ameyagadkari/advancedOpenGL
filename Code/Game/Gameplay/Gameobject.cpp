#include "Gameobject.h"

#include "../../Engine/Graphics/Mesh.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Graphics/Effect.h"

cs6610::Gameplay::GameObject::GameObject(const cyPoint3f i_position, const cyPoint3f i_eularAngles) :
	m_mesh(nullptr),
	m_effect(nullptr),
	m_position(i_position),
	m_eularAngles(i_eularAngles)
{}
cs6610::Gameplay::GameObject::~GameObject()
{
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
	if (m_effect)
	{
		delete m_effect;
		m_effect = nullptr;
	}
}

#pragma region Gets
cs6610::Graphics::Effect* cs6610::Gameplay::GameObject::GetEffect() const
{
	return m_effect;
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
#pragma endregion

#pragma region Sets
void cs6610::Gameplay::GameObject::SetEffect(const std::string i_relativePathVertex, const std::string i_relativePathFragment, const char * const i_names)
{
	if (!m_effect)
	{
		m_effect = new Graphics::Effect(i_relativePathVertex, i_relativePathFragment);
		if (i_names)
		{
			m_effect->RegisterUniforms(i_names);
		}
	}
	else
	{
		CS6610_ASSERTF(false, "Use the other overload to set effect");
	}
}
void cs6610::Gameplay::GameObject::SetEffect(Graphics::Effect* const i_effect, const char * const i_names)
{
	if (m_effect)
	{
		delete m_effect;
		m_effect = nullptr;
		m_effect = i_effect;
		if (i_names)
		{
			m_effect->RegisterUniforms(i_names);
		}
	}
	else
	{
		CS6610_ASSERTF(false, "Use the other overload to set effect");
	}
}
void cs6610::Gameplay::GameObject::SetMesh(const std::string i_meshRelativePath)
{
	if (!m_mesh)
	{
		cyPoint3f minBounds, maxBounds;
		m_mesh = new Graphics::Mesh(i_meshRelativePath, minBounds, maxBounds);
		m_position += (maxBounds + minBounds) / -2.0f;
	}
	else
	{
		CS6610_ASSERTF(false, "Use the other overload to set mesh");
	}
}
void cs6610::Gameplay::GameObject::SetMesh(cs6610::Graphics::Mesh* const i_mesh)
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