#include "MyGame.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Graphics/Scene.h"
#include "../Gameplay/Gameobject.h"
//#include "../../Engine/Camera/Camera.h"



namespace cs6610
{
	namespace MyGame
	{
		Graphics::Scene* mainScene;
		Graphics::Scene* secondaryScene;
		//std::map<std::string, Gameplay::GameObject*> ms_gameobjects;
		//Camera::Camera* ms_pcamera;
		//Camera::Camera* ms_ocamera;
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
	secondaryScene = new Graphics::Scene();
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
	/*secondaryScene->AddGameObjectsToScene("Plane", new Gameplay::GameObject(cyPoint3f(0.0f, 10.0f, 0.0f), cyPoint3f(45.0f, 0.0f, 45.0f), cyPoint3f(1.0f)));
	secondaryScene->GetGameobjectByName("Plane")->LoadMeshAndMaterial(
		i_arguments[3],
		{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
		{});*/

	// Init all gameobjects
	/*{
		ms_gameobjects["Teapot"] = new Gameplay::GameObject(cyPoint3f(0.0f, 0.0f, 0.0f), cyPoint3f(180.0f, 0.0f, 180.0f));
		ms_gameobjects.at("Teapot")->LoadMeshAndMaterial(
			"data/meshes/teapot/teapot.obj",
			{ "data/shaders/stdvertex.glsl", "data/shaders/stdfragment.glsl" },
			{ "data/meshes/teapot/teapot_diffuse.png", "data/meshes/teapot/teapot_specular.png" },
			"u_normal u_lightPosition");

		ms_gameobjects["Light"] = new Gameplay::GameObject(cyPoint3f(0.0f, -10.0f, 0.0f));
		ms_gameobjects.at("Light")->LoadMeshAndMaterial(
			"data/meshes/cube/cube.obj",
			{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" }, 
			{});
	}*/

	// Init Camera
	/*{
		ms_pcamera = new Camera::Camera();
		//ms_ocamera = new Camera::Camera(cyPoint3f(0.0f, 0.0f, 0.0f), cyPoint3f(0.0f, 0.0f, 0.0f), Math::ConvertDegreesToRadians(45.0f), -20.0f, 20.0f);
	}*/
	return !wereThereErrors;
}

void cs6610::MyGame::CleanUp()
{
	/*auto begin = ms_gameobjects.begin();
	auto end = ms_gameobjects.end();
	for (auto it = begin; it != end; ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
	ms_gameobjects.clear();
	if (ms_pcamera)
	{
		delete ms_pcamera;
		ms_pcamera = nullptr;
	}*/
	//if (ms_ocamera)
	//{
	//	delete ms_ocamera;
	//	ms_ocamera = nullptr;
	//}

	if (secondaryScene)
	{
		delete secondaryScene;
		secondaryScene = nullptr;
	}
}
