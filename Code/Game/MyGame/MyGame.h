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
	namespace MyGame
	{
		void Run();
		bool Initialize(int i_argumentCount, char** i_arguments);
		void CleanUp();
		extern std::map<std::string, Gameplay::GameObject*> ms_gameobjects;
		extern Camera::Camera *ms_pcamera;
		extern Camera::Camera *ms_ocamera;
	};
}
