#include "MyGame.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../Gameplay/Gameobject.h"
#include "../../Engine/Camera/Camera.h"

std::map<std::string, cs6610::Gameplay::GameObject*> cs6610::MyGame::ms_gameobjects;
cs6610::Camera::Camera* cs6610::MyGame::ms_pcamera;
cs6610::Camera::Camera* cs6610::MyGame::ms_ocamera;

void cs6610::MyGame::Run(void)
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
	if (!UserInput::UserInput::Initialize())
	{
		CS6610_ASSERTF(false, "UserInput Initialization failed");
		wereThereErrors = true;
	}

	// Init all gameobjects
	{
		ms_gameobjects["Teapot"] = new Gameplay::GameObject(cyPoint3f(0.0f, 0.0f, 0.0f), cyPoint3f(180.0f, 0.0f, 180.0f));
		ms_gameobjects.at("Teapot")->SetMesh("data/meshes/teapot/teapot.obj");
		ms_gameobjects.at("Teapot")->SetMaterial(
		{ "data/shaders/stdvertex.glsl", "data/shaders/stdfragment.glsl" },
		{ "data/meshes/teapot/teapot_diffuse.png", "data/meshes/teapot/teapot_specular.png" },
			"u_model u_view u_projection u_normal u_lightPosition");
		ms_gameobjects["Light"] = new Gameplay::GameObject(cyPoint3f(0.0f, -10.0f, 0.0f));
		ms_gameobjects.at("Light")->SetMesh("data/meshes/light.obj");
		ms_gameobjects.at("Light")->SetMaterial(
		{ "data/shaders/lightvertex.glsl", "data/shaders/lightfragment.glsl" },
		{},
			"u_model u_view u_projection");
	}

	// Init Camera
	{
		ms_pcamera = new Camera::Camera();
		ms_ocamera = new Camera::Camera(cyPoint3f(0.0f, 0.0f, 0.0f), cyPoint3f(0.0f, 0.0f, 0.0f), Math::ConvertDegreesToRadians(45.0f), -20.0f, 20.0f);
	}
	return !wereThereErrors;
}

void cs6610::MyGame::CleanUp(void)
{
	auto begin = ms_gameobjects.begin();
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
	}
	if (ms_ocamera)
	{
		delete ms_ocamera;
		ms_ocamera = nullptr;
	}
}
