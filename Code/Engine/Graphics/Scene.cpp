#include "Scene.h"
#include "../../External/cyCodeBase/cyGL.h"
#include "../Math/BitManipulator.h"
#include "../Camera/Camera.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "../Asserts/Asserts.h"

cs6610::Graphics::Scene::Scene(bool const i_useRenderBuffer, Color const i_clearColor, float const i_clearDepth, uint8_t const i_clearControlBits, bool const i_useDepthBuffer)
	:
	m_clearColor(i_clearColor),
	m_pcamera(new Camera::Camera(cyPoint3f(0.0f,1.0f,10.0f))),
	m_renderBuffer(i_useRenderBuffer ? new cyGLRenderDepth2D() : nullptr),
	m_clearDepth(i_clearDepth),
	m_clearMask(0),
	m_useDepthBuffer(i_useDepthBuffer)
{
	m_clearMask |= Math::BitManipulator::IsBitSet(i_clearControlBits, 0) ? GL_COLOR_BUFFER_BIT : 0;
	m_clearMask |= Math::BitManipulator::IsBitSet(i_clearControlBits, 1) ? GL_DEPTH_BUFFER_BIT : 0;
	if (m_renderBuffer)
	{
		if (!m_renderBuffer->Initialize(m_useDepthBuffer))
		{
			CS6610_ASSERTF(false, "RenderBuffer is not ready");
		}
		//else
		//{
		//	//m_renderBuffer->SetTextureWrappingMode(GL_REPEAT,GL_REPEAT);
		//	//m_renderBuffer->SetTextureFilteringMode(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		//	//m_renderBuffer->SetTextureMaxAnisotropy();
		//}
	}
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
cyGLRenderDepth2D* cs6610::Graphics::Scene::GetRenderBuffer()const
{
	return m_renderBuffer;
}
#pragma endregion 

void cs6610::Graphics::Scene::AddGameObjectsToScene(std::string const i_name, Gameplay::GameObject* i_gameOject)
{
	if (i_gameOject)m_gameobjects[i_name] = i_gameOject;
}

void cs6610::Graphics::Scene::RenderScene() const
{
	if (m_renderBuffer)m_renderBuffer->Bind();
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