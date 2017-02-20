#include "Scene.h"
#include "../../External/cyCodeBase/cyGL.h"
#include "../Math/BitManipulator.h"
#include "../Camera/Camera.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "../Asserts/Asserts.h"

cs6610::Graphics::Scene::Scene(Color const i_clearColor, float const i_clearDepth, uint8_t const i_clearControlBits, bool const i_useDepthBuffer)
	:
	m_clearColor(i_clearColor),
	m_pcamera(new Camera::Camera()),
	m_renderBuffer(nullptr),
	m_clearDepth(i_clearDepth),
	m_useDepthBuffer(i_useDepthBuffer)
{
	m_clearMask = 0;
	m_clearMask |= Math::BitManipulator::IsBitSet(i_clearControlBits, 0) ? GL_COLOR_BUFFER_BIT : 0;
	m_clearMask |= Math::BitManipulator::IsBitSet(i_clearControlBits, 1) ? GL_DEPTH_BUFFER_BIT : 0;
	;
}

#pragma region Gets
cs6610::Gameplay::GameObject* cs6610::Graphics::Scene::GetGameobjectByName(std::string const i_name)const
{
	return m_gameobjects.at(i_name);
}

cs6610::Camera::Camera* cs6610::Graphics::Scene::GetCamera()const
{
	return m_pcamera;
}
cy::GLRenderBuffer* cs6610::Graphics::Scene::GetRenderBuffer()const
{
	return m_renderBuffer;
}
#pragma endregion 

void cs6610::Graphics::Scene::AddGameObjectsToScene(std::string const i_name, Gameplay::GameObject* i_gameOject)
{
	if (i_gameOject)
	{
		m_gameobjects[i_name] = i_gameOject;
	}
}

void cs6610::Graphics::Scene::RenderScene() const
{
	if (m_useDepthBuffer)
	{
		glEnable(GL_DEPTH_TEST);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to enable depth buffer");
		glDepthFunc(GL_LESS);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set depth func");
		glDepthMask(GL_TRUE);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set depth mask for writing to depth buffer");
	}
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear color");
	glClearDepth(m_clearDepth);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear depth");
	glClear(m_clearMask);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to clear color buffer and depth buffer");
}

cs6610::Graphics::Scene::~Scene()
{
	if (m_pcamera)
	{
		delete m_pcamera;
		m_pcamera = nullptr;
	}
	if (m_renderBuffer)
	{
		delete m_renderBuffer;
		m_renderBuffer = nullptr;
	}
	auto begin = m_gameobjects.begin();
	auto end = m_gameobjects.end();
	for (auto it = begin; it != end; ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_gameobjects.clear();
}