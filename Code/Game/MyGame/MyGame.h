#pragma once

#include <map>

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
	class MyGame
	{
	public:
		static void Run(void);
		static bool Initialize(int i_argumentCount, char** i_arguments);
		static void CleanUp(void);
		static std::map<std::string, Gameplay::GameObject*> ms_gameobjects;
		static Camera::Camera *ms_pcamera;
		static Camera::Camera *ms_ocamera;
	};
}
