#pragma once

#include <vector>

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
		static std::vector<Gameplay::GameObject*> ms_gameobjects;
	};
}
