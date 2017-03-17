#pragma once
#include "Color.h"
#include <cstdint>
#include <map>

typedef unsigned int GLbitfield;
typedef unsigned int GLenum;

namespace cy
{
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
			Scene(bool const i_useRenderBuffer = false, Color const i_clearColor = { 0.0f,0.0f,0.0f,1.0f }, float const i_clearDepth = 1.0f, uint8_t const i_clearControlBits = 0xc0, bool const i_useDepthBuffer = true);
			~Scene();
			void RenderScene()const;
			void AddGameObjectsToScene(std::string const i_name, Gameplay::GameObject* i_gameOject);

#pragma region Gets
			Gameplay::GameObject * GetGameobjectByName(std::string const i_name)const;
			Camera::Camera* GetCamera()const;
#define GL_TEXTURE_2D 0x0DE1
			cy::GLRenderDepth<GL_TEXTURE_2D>* GetRenderBuffer()const;
#pragma endregion 
		private:
			Color m_clearColor;
			std::map<const std::string, Gameplay::GameObject*> m_gameobjects;
			Camera::Camera* m_pcamera;
			cy::GLRenderDepth<GL_TEXTURE_2D>* m_renderBuffer;
			float m_clearDepth;
			GLbitfield m_clearMask;
			bool m_useDepthBuffer;
		};
	}
}
