#pragma once
#include "Color.h"
#include <map>
#include <string>

typedef unsigned int GLbitfield;
typedef unsigned int GLenum;

namespace cy
{
	template <GLenum TEXTURE_TYPE>
	class GLRenderTexture;
	template <GLenum TEXTURE_TYPE>
	class GLRenderDepth;
}

namespace cs6610
{
	namespace Camera
	{
		class Camera;
	}
}

namespace cs6610
{
	namespace Gameplay
	{
		class GameObject;
	}
}

namespace cs6610
{
	namespace Graphics
	{
		class Scene
		{
		public:
			//Scene(bool const i_useRenderBuffer = false, Color const i_clearColor = { 0.0f,0.0f,0.0f,0.0f }, float const i_clearDepth = 1.0f, uint32_t const i_clearControlBits = 0xc0000000, bool const i_useDepthBuffer = true);
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
			explicit Scene(
				Camera::Camera* i_perspectiveCamera = nullptr,
				const bool i_hasColorTexture = false, 
				const bool i_useDepthBufferOfColorTexture = false, 
				const bool i_hasDepthTexture = false, 
				const bool i_useHardwareDepthComparisonInDepthTexture = false, 			
				const Color i_clearColor = { 0.0f,0.0f,0.0f,1.0f },
				const float i_clearDepth = 1.0f, 
				const GLbitfield i_clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			~Scene();
			void RenderSceneUsingColorTexture()const;
			void RenderSceneUsingDepthTexture()const;
			void AddGameObjectsToScene(std::string const i_name, Gameplay::GameObject* i_gameOject);
			void ClearScreen()const;

#pragma region Gets
			Gameplay::GameObject * GetGameobjectByName(std::string const i_name)const;
			Camera::Camera* GetCamera()const;
#define GL_TEXTURE_2D 0x0DE1
			cy::GLRenderTexture<GL_TEXTURE_2D>* GetColorBuffer()const;
			cy::GLRenderDepth<GL_TEXTURE_2D>* GetDepthBuffer()const;
#pragma endregion 
		private:					
			std::map<const std::string, Gameplay::GameObject*> m_gameobjects;			
			Color m_clearColor;
			Camera::Camera* m_perspectiveCamera;
			cy::GLRenderTexture<GL_TEXTURE_2D>* m_colorBuffer;
			cy::GLRenderDepth<GL_TEXTURE_2D>* m_depthBuffer;
			float m_clearDepth;
			GLbitfield m_clearMask;
			bool m_hasColorTexture;
			bool m_useDepthBufferOfColorTexture;
			bool m_hasDepthTexture;
			bool m_useHardwareDepthComparisonInDepthTexture;
		};
	}
}
