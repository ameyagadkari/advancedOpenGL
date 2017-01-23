#include "MyGame.h"

#include "../../Engine/Asserts/Asserts.h"

#include <crtdbg.h>
#include <iostream>

int main(int i_argumentCount, char** i_arguments)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!cs6610::MyGame::Initialize(i_argumentCount, i_arguments))
	{		
		std::cerr << "My Game initialization failed" << std::endl;
		CS6610_ASSERT(false);
	}
	else
	{
		cs6610::MyGame::Run(); 
	}

	return 0;
}