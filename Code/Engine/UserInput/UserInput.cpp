#include "UserInput.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Game/MyGame/MyGame.h"

namespace
{
	void keyboard(unsigned char c, int x, int y);
	void mouse(int button, int state, int x, int y);
	void close(void);
}

bool cs6610::UserInput::Initialize(void)
{
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(close);
	return true;
}

namespace
{
	void keyboard(unsigned char c, int x, int y)
	{
		switch (c)
		{
		case VK_ESCAPE:
			cs6610::MyGame::CleanUp();
			glutLeaveMainLoop();
		default:
			break;
		}
	}
	void mouse(int button, int state, int x, int y)
	{
		//Will be implemented afterwards for camera
	}
	void close(void)
	{
		cs6610::MyGame::CleanUp();
		glutLeaveMainLoop();
	}
}