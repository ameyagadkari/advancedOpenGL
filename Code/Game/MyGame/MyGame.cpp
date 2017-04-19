#include "MyGame.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Graphics/Scene.h"
#include "../Gameplay/Gameobject.h"
#include "../../Engine/Camera/Camera.h"

namespace cs6610
{
	namespace MyGame
	{
		Graphics::Scene* mainScene;
		Graphics::Scene* reflectionTexture;
		Graphics::Scene* refractionTexture;
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
		mainScene = new Graphics::Scene(new Camera::Camera(cyPoint3f(0.0f, 5.0f, 10.0f)));
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
			"u_clippingplane u_normal u_lightPosition");

		mainScene->AddGameObjectsToScene("Water", new Gameplay::GameObject(cyPoint3f(0.0f, 1.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(55.0f)));
		mainScene->GetGameobjectByName("Water")->LoadMeshAndMaterial(false,
			"data/meshes/water/water.obj",
			{ "data/shaders/watervertex.glsl", "data/shaders/waterfragment.glsl" }, 
			{},
			"data/meshes/water/",
			"u_moveFactor u_cameraPosition u_lightPosition u_nearPlane u_farPlane");

		mainScene->AddGameObjectsToScene("Light", new Gameplay::GameObject(cyPoint3f(0.0f, 20.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(0.1f)));
		mainScene->GetGameobjectByName("Light")->LoadMeshAndMaterial(
			false,
			"data/meshes/light/light.obj",
			{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
			{});

		mainScene->AddGameObjectsToScene("Boat", new Gameplay::GameObject(cyPoint3f(-40.0f, -42.5f, 0.0f), cyPoint3f(0.0f, 90.f, 0.0f), cyPoint3f(0.1f)));
		mainScene->GetGameobjectByName("Boat")->LoadMeshAndMaterial(
			false,
			"data/meshes/boat/boat.obj",
			{ "data/shaders/boatvertex.glsl", "data/shaders/boatfragment.glsl" },
			{},
			"data/meshes/boat/",
			"u_clippingplane u_normal u_lightPosition");

		mainScene->AddGameObjectsToScene("WoodenBridge", new Gameplay::GameObject(cyPoint3f(25.0f, -45.0f, 0.0f), cyPoint3f(0.0f, 90.0f,0.0f), cyPoint3f(0.1f)));
		mainScene->GetGameobjectByName("WoodenBridge")->LoadMeshAndMaterial(
			false,
			"data/meshes/woodenbridge/woodenbridge.obj",
			{ "data/shaders/woodenbridgevertex.glsl", "data/shaders/woodenbridgefragment.glsl" },
			{},
			"data/meshes/woodenbridge/",
			"u_clippingplane u_normal u_lightPosition");
	}

	reflectionTexture = new Graphics::Scene(nullptr, true, true, false, false);
	refractionTexture = new Graphics::Scene(nullptr, true, true, true, false);
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
}
