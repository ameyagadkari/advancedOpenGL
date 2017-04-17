#include "MyGame.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Graphics/Scene.h"
#include "../Gameplay/Gameobject.h"

namespace cs6610
{
	namespace MyGame
	{
		Graphics::Scene* mainScene;
		Graphics::Scene* reflectionTexture;
		Graphics::Scene* refractionTexture;
		Graphics::Scene* refractionDepthTexture;
	}
}

void cs6610::MyGame::Run()
{
	glutMainLoop();
}

bool cs6610::MyGame::Initialize(int i_argumentCount, char ** i_arguments)
{
	bool wereThereErrors = false;
	Time::Initialize();
	if (!Graphics::Initialize(i_argumentCount, i_arguments))
	{
		CS6610_ASSERTF(false, "Graphics Initialization failed");
		wereThereErrors = true;
	}
	if (!UserInput::Initialize())
	{
		CS6610_ASSERTF(false, "UserInput Initialization failed");
		wereThereErrors = true;
	}

	//Init all scenes
	{
		mainScene = new Graphics::Scene(false);
		mainScene->AddGameObjectsToScene("Skybox", new Gameplay::GameObject());
		mainScene->GetGameobjectByName("Skybox")->LoadMeshAndMaterial(
			true,
			"data/meshes/skybox/skybox.obj",
			{ "data/shaders/skyboxvertex.glsl", "data/shaders/skyboxfragment.glsl" },
			{ "data/meshes/skybox/pos_x.png","data/meshes/skybox/neg_x.png" ,"data/meshes/skybox/pos_y.png" ,"data/meshes/skybox/neg_y.png" ,"data/meshes/skybox/pos_z.png" ,"data/meshes/skybox/neg_z.png" },
			"",
			"u_clippingplane");

		mainScene->AddGameObjectsToScene("CartoonLand", new Gameplay::GameObject());
		mainScene->GetGameobjectByName("CartoonLand")->LoadMeshAndMaterial(
			false,
			"data/meshes/cartoonland/cartoonland.obj",
			{ "data/shaders/cartoonlandvertex.glsl", "data/shaders/cartoonlandfragment.glsl" },
			{},
			"",
			"u_clippingplane");

		mainScene->AddGameObjectsToScene("Water", new Gameplay::GameObject(cyPoint3f(20.0f,0.75f,-10.0f), cyPoint3f(0.0f), cyPoint3f(10.0f)));
		mainScene->GetGameobjectByName("Water")->LoadMeshAndMaterial(false,
			"data/meshes/water/water.obj",
			{ "data/shaders/watervertex.glsl", "data/shaders/waterfragment.glsl" }, 
			{},
			"data/meshes/water/",
			"moveFactor");
	}

	reflectionTexture = new Graphics::Scene(true);
	refractionTexture = new Graphics::Scene(true);
	return !wereThereErrors;
}

void cs6610::MyGame::CleanUp()
{
	if (mainScene)
	{
		delete mainScene;
		mainScene = nullptr;
	}

	if (reflectionTexture)
	{
		delete reflectionTexture;
		reflectionTexture = nullptr;
	}

	if (refractionTexture)
	{
		delete refractionTexture;
		refractionTexture = nullptr;
	}

	if (refractionDepthTexture)
	{
		delete refractionDepthTexture;
		refractionDepthTexture = nullptr;
	}
}
