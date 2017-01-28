#include "MyGame.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Asserts/Asserts.h"

void cs6610::MyGame::Run(void)
{
	glutMainLoop();
}

bool cs6610::MyGame::Initialize(int i_argumentCount, char ** i_arguments)
{
	bool wereThereErrors = false;
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
	return !wereThereErrors;
}
