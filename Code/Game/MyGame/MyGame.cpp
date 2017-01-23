#include "MyGame.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/UserInput/UserInput.h"

#include <cassert>
#include <iostream>

void cs6610::MyGame::Run(void)
{
	glutMainLoop();
}

bool cs6610::MyGame::Initialize(int i_argumentCount, char ** i_arguments)
{
	bool wereThereErrors = false;
	if (!cs6610::Graphics::Initialize(i_argumentCount, i_arguments))
	{
		assert(false);
		std::cerr << "Graphics Initialization failed" << std::endl;
		wereThereErrors = true;
	}
	if (!cs6610::UserInput::Initialize())
	{
		assert(false);
		std::cerr << "UserInput Initialization failed" << std::endl;
		wereThereErrors = true;
	}
	return !wereThereErrors;
}
