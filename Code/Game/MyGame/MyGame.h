#pragma once

namespace cs6610
{
	namespace Graphics
	{
		class Scene;
	}
}

namespace cs6610
{
	namespace MyGame
	{
		void Run();
		bool Initialize(int i_argumentCount, char** i_arguments);
		void CleanUp();
		extern Graphics::Scene* mainScene;
		extern Graphics::Scene* reflectionTexture;
		extern Graphics::Scene* refractionTexture;
	}
}
