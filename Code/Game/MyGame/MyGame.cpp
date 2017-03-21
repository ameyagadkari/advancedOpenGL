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
		Graphics::Scene* secondaryScene;
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
		mainScene = new Graphics::Scene(false, { 0.0f,0.0f,0.25f,0.0f });

		mainScene->AddGameObjectsToScene("Light", new Gameplay::GameObject(cyPoint3f(0.0f, 20.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(0.1f)));
		mainScene->GetGameobjectByName("Light")->LoadMeshAndMaterial(
			false,
			"data/meshes/light/light.obj",
			{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
			"");

		mainScene->AddGameObjectsToScene("Teapot", new Gameplay::GameObject(cyPoint3f(0.0f), cyPoint3f(0.0f), cyPoint3f(0.05f)));
		mainScene->GetGameobjectByName("Teapot")->LoadMeshAndMaterial(
			false,
			i_arguments[1],
			{ "data/shaders/teapotvertex.glsl", "data/shaders/teapotfragment.glsl" },
			"",
			"u_normal u_lightPosition u_depthShader");

		mainScene->AddGameObjectsToScene("Plane", new Gameplay::GameObject(cyPoint3f(0.0f, 0.0f, 0.0f)));
		mainScene->GetGameobjectByName("Plane")->LoadMeshAndMaterial(
			false,
			"data/meshes/plane/plane.obj",
			{ "data/shaders/planevertex.glsl", "data/shaders/planefragment.glsl" },
			"data/meshes/plane/",
			"u_normal u_lightPosition u_near_plane u_far_plane");/*u_normal u_lightPosition u_viewPosition u_near_plane u_far_plane*/

		secondaryScene = new Graphics::Scene(true, { 0.0f,0.0f,0.0f,0.0f }, 1.0f, 0x40000000, false);
		/*mainScene->AddGameObjectsToScene("Teapot", new Gameplay::GameObject(cyPoint3f(0.0f, 0.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(0.05f)));
		mainScene->GetGameobjectByName("Teapot")->LoadMeshAndMaterial(
			false,
			i_arguments[1],
			{ "data/shaders/teapotvertex.glsl", "data/shaders/teapotfragment.glsl" },
			{},
			"u_normal u_lightPosition u_calcDepth");


		mainScene->AddGameObjectsToScene("Plane", new Gameplay::GameObject(cyPoint3f(0.0f, -20.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(1.0f)));
		mainScene->GetGameobjectByName("Plane")->LoadMeshAndMaterial(
			false,
			"data/meshes/plane/plane.obj",
			{ "data/shaders/planevertex.glsl", "data/shaders/planefragment.glsl" },
			{},
			"u_normal u_lightPosition");/*u_near_plane u_far_plane

		mainScene->AddGameObjectsToScene("Light", new Gameplay::GameObject(cyPoint3f(0.0f, 20.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(0.1f)));
		mainScene->GetGameobjectByName("Light")->LoadMeshAndMaterial(
			false,
			"data/meshes/light/light.obj",
			{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
			{});

		secondaryScene = new Graphics::Scene(true, { 0.0f,0.0f,0.0f,0.0f }, 1.0f, 0x40000000, true);*/


	}
	return !wereThereErrors;
}

void cs6610::MyGame::CleanUp()
{
	if (mainScene)
	{
		delete mainScene;
		mainScene = nullptr;
	}

	if (secondaryScene)
	{
		delete secondaryScene;
		secondaryScene = nullptr;
	}
}
