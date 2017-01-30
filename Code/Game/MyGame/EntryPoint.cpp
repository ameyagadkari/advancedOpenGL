#include "MyGame.h"

#include "../../Engine/Asserts/Asserts.h"

#include <crtdbg.h>

int main(int i_argumentCount, char** i_arguments)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!cs6610::MyGame::Initialize(i_argumentCount, i_arguments))
	{		
		CS6610_ASSERTF(false, "My Game initialization failed");
	}
	else
	{
		cs6610::MyGame::Run();
	}
	return 0;
}