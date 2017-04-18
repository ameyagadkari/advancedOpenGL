#include "Scene.h"
#include "../../External/cyCodeBase/cyGL.h"
#include "../Camera/Camera.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "../Asserts/Asserts.h"

/*cs6610::Graphics::Scene::Scene(bool const i_useRenderBuffer, Color const i_clearColor, float const i_clearDepth, uint32_t const i_clearControlBits, bool const i_useDepthBuffer)
	:
	m_clearColor(i_clearColor),
	m_pcamera(new Camera::Camera(cyPoint3f(0.0f,5.0f,10.0f))),
	m_renderBuffer(i_useRenderBuffer ? new cyGLRenderTexture2D() : nullptr),
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
		else
		{
			m_renderBuffer->SetTextureWrappingMode(GL_REPEAT,GL_REPEAT);
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture Wrapping Mode");
			//m_renderBuffer->SetTextureFilteringMode(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
			//CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture Filtering Mode");
			m_renderBuffer->SetTextureMaxAnisotropy();
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture MaxAnisotropy");
		}
	}
}*/

cs6610::Graphics::Scene::Scene(
	Camera::Camera* i_perspectiveCamera,
	const bool i_hasColorTexture,
	const bool i_useDepthBufferOfColorTexture,
	const bool i_hasDepthTexture,
	const bool i_useHardwareDepthComparisonInDepthTexture,	
	const Color i_clearColor,
	const float i_clearDepth,
	const GLbitfield i_clearMask)
	:
	m_clearColor(i_clearColor),
	m_perspectiveCamera(i_perspectiveCamera),
	m_colorBuffer(i_hasColorTexture ? new cyGLRenderTexture2D() : nullptr),
	m_depthBuffer(i_hasDepthTexture ? new cyGLRenderDepth2D() : nullptr),
	m_clearDepth(i_clearDepth),
	m_clearMask(i_clearMask),
	m_hasColorTexture(i_hasColorTexture),
	m_useDepthBufferOfColorTexture(i_useDepthBufferOfColorTexture),
	m_hasDepthTexture(i_hasDepthTexture),
	m_useHardwareDepthComparisonInDepthTexture(i_useHardwareDepthComparisonInDepthTexture)
{

	if (m_hasColorTexture && m_colorBuffer)
	{
		if (!m_colorBuffer->Initialize(m_useDepthBufferOfColorTexture))
		{
			CS6610_ASSERTF(false, "Color Buffer is not ready");
		}
		else
		{
			m_colorBuffer->SetTextureWrappingMode(GL_REPEAT, GL_REPEAT);
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture Wrapping Mode");
			/*m_colorBuffer->BuildTextureMipmaps();
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Build MipMaps");
			m_colorBuffer->SetTextureFilteringMode(GL_LINEAR, GL_LINEAR);
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture Filtering Mode");*/
			m_colorBuffer->SetTextureMaxAnisotropy();
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture MaxAnisotropy");
		}
	}

	if (m_hasDepthTexture && m_depthBuffer)
	{
		if (!m_depthBuffer->Initialize(m_useHardwareDepthComparisonInDepthTexture))
		{
			CS6610_ASSERTF(false, "Depth Buffer is not ready");
		}
		else
		{
			m_colorBuffer->SetTextureWrappingMode(GL_REPEAT, GL_REPEAT);
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture Wrapping Mode");
			/*m_colorBuffer->BuildTextureMipmaps();
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Build MipMaps");
			m_colorBuffer->SetTextureFilteringMode(GL_LINEAR, GL_LINEAR);
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture Filtering Mode");*/
			m_colorBuffer->SetTextureMaxAnisotropy();
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to Set Texture MaxAnisotropy");
		}
	}
}

#pragma region Gets
cs6610::Gameplay::GameObject* cs6610::Graphics::Scene::GetGameobjectByName(std::string const i_name)const
{
	return m_gameobjects.at(i_name);
}
cs6610::Camera::Camera* cs6610::Graphics::Scene::GetCamera()const
{
	return m_perspectiveCamera;
}
cyGLRenderTexture2D* cs6610::Graphics::Scene::GetColorBuffer()const
{
	return m_colorBuffer;
}
cyGLRenderDepth2D* cs6610::Graphics::Scene::GetDepthBuffer()const
{
	return m_depthBuffer;
}
#pragma endregion 

void cs6610::Graphics::Scene::AddGameObjectsToScene(std::string const i_name, Gameplay::GameObject* i_gameOject)
{
	if (i_gameOject)m_gameobjects[i_name] = i_gameOject;
}

void cs6610::Graphics::Scene::RenderSceneUsingDepthTexture() const
{
	if (m_depthBuffer)m_depthBuffer->Bind();
	ClearScreen();
}

void cs6610::Graphics::Scene::RenderSceneUsingColorTexture()const
{
	if (m_colorBuffer)m_colorBuffer->Bind();
	ClearScreen();
}

cs6610::Graphics::Scene::~Scene()
{
	if (m_perspectiveCamera)
	{
		delete m_perspectiveCamera;
		m_perspectiveCamera = nullptr;
	}
	if (m_colorBuffer)
	{
		delete m_colorBuffer;
		m_colorBuffer = nullptr;
	}
	if (m_depthBuffer)
	{
		delete m_depthBuffer;
		m_depthBuffer = nullptr;
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

void cs6610::Graphics::Scene::ClearScreen()const
{
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear color");
	glClearDepth(m_clearDepth);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear depth");
	glClear(m_clearMask);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to clear color buffer and depth buffer");
}
