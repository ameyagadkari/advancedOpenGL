#include "MyGame.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../Gameplay/Gameobject.h"
#include "../../Engine/Camera/Camera.h"

std::vector<cs6610::Gameplay::GameObject*> cs6610::MyGame::ms_gameobjects;
cs6610::Camera::Camera* cs6610::MyGame::ms_camera;

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
		ms_gameobjects.push_back(new Gameplay::GameObject());
		ms_gameobjects.back()->SetMesh("data/meshes/teapot.mesh");
		ms_gameobjects.back()->SetEffect("data/shaders/stdvertex.glsl", "data/shaders/stdfragment.glsl","u_model u_view u_projection");
	}

	// Init Camera
	{
		ms_camera = new Camera::Camera();
	}
	return !wereThereErrors;
}

void cs6610::MyGame::CleanUp(void)
{
	size_t length = ms_gameobjects.size();
	for (size_t i = 0; i < length; i++)
	{
		delete ms_gameobjects[i];
		ms_gameobjects[i] = nullptr;
	}
	ms_gameobjects.clear();
	if(ms_camera)
	{
		delete ms_camera;
		ms_camera = nullptr;
	}
}
