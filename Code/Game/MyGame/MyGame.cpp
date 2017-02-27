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
		mainScene = new Graphics::Scene();

		/*
		mainScene->AddGameObjectsToScene("Plane", new Gameplay::GameObject(cyPoint3f(0.0f, 10.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(1.0f)));
		mainScene->GetGameobjectByName("Plane")->LoadMeshAndMaterial(
			false,
			i_arguments[3],
			{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
		{});*/

		mainScene->AddGameObjectsToScene("Light", new Gameplay::GameObject(cyPoint3f(0.0f, 10.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(0.1f)));
		mainScene->GetGameobjectByName("Light")->LoadMeshAndMaterial(
			false,
			i_arguments[2],
			{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
			{});

		mainScene->AddGameObjectsToScene("Teapot", new Gameplay::GameObject(cyPoint3f(0.0f), cyPoint3f(0.0f), cyPoint3f(0.05f)));
		mainScene->GetGameobjectByName("Teapot")->LoadMeshAndMaterial(
			false,
			i_arguments[1],
			{ "data/shaders/teapotvertex.glsl", "data/shaders/teapotfragment.glsl" },
			{},
			"u_normal u_lightPosition");
		/*mainScene->AddGameObjectsToScene("Sphere", new Gameplay::GameObject(cyPoint3f(0.0f), cyPoint3f(0.0f), cyPoint3f(0.05f)));
		mainScene->GetGameobjectByName("Sphere")->LoadMeshAndMaterial(
			true,
			"data/meshes/sphere/sphere.obj",
			{ "data/shaders/spherevertex.glsl", "data/shaders/spherefragment.glsl" },
			{},
			"u_normal u_lightPosition");*/

		mainScene->AddGameObjectsToScene("EnvCube", new Gameplay::GameObject());
		mainScene->GetGameobjectByName("EnvCube")->LoadMeshAndMaterial(
			true,
			"data/meshes/environment/cube.obj",
			{ "data/shaders/envvertex.glsl", "data/shaders/envfragment.glsl" },
			{ "data/meshes/environment/cubemap_posx.png","data/meshes/environment/cubemap_negx.png" ,"data/meshes/environment/cubemap_posy.png" ,"data/meshes/environment/cubemap_negy.png" ,"data/meshes/environment/cubemap_posz.png" ,"data/meshes/environment/cubemap_negz.png" });
	}

	/*{
		const bool useRenderBuffer = true;
		secondaryScene = new Graphics::Scene(useRenderBuffer, { 0.25f,0.25f,0.25f });
		secondaryScene->AddGameObjectsToScene("Teapot", new Gameplay::GameObject(cyPoint3f(0.0f), cyPoint3f(180.0f, 0.0f, 180.0f), cyPoint3f(0.05f)));
		secondaryScene->GetGameobjectByName("Teapot")->LoadMeshAndMaterial(
			i_arguments[1],
			{ "data/shaders/stdvertex.glsl", "data/shaders/stdfragment.glsl" },
			{ "data/meshes/teapot/teapot_diffuse.png", "data/meshes/teapot/teapot_specular.png" },
			"u_normal u_lightPosition");
		secondaryScene->AddGameObjectsToScene("Light", new Gameplay::GameObject(cyPoint3f(0.0f, -10.0f, 0.0f), cyPoint3f(0.0f), cyPoint3f(0.1f)));
		secondaryScene->GetGameobjectByName("Light")->LoadMeshAndMaterial(
			i_arguments[2],
			{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
			{});
	}*/
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
