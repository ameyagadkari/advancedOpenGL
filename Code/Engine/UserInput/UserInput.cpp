#include "UserInput.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Game/MyGame/MyGame.h"
#include "../Graphics/Effect.h"

namespace
{
	void keyPress(unsigned char c, int x, int y);
	void keyRelease(unsigned char c, int x, int y);
	void keyPressSpecial(int key, int x, int y);
	void keyReleaseSpecial(int key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void close(void);
}

std::bitset<256> cs6610::UserInput::UserInput::keys;

bool cs6610::UserInput::UserInput::Initialize(void)
{
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutSpecialFunc(keyPressSpecial);
	glutSpecialUpFunc(keyReleaseSpecial);
	glutMouseFunc(mouse);
	glutCloseFunc(close);
	return true;
}

namespace
{
	void keyPress(unsigned char c, int x, int y)
	{
		if (c == VK_ESCAPE)
		{
			cs6610::MyGame::CleanUp();
			glutLeaveMainLoop();
		}
		else
		{
			cs6610::UserInput::UserInput::keys.set(c);
		}
	}
	void keyRelease(unsigned char c, int x, int y)
	{
		cs6610::UserInput::UserInput::keys.reset(c);
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
	void keyPressSpecial(int key, int x, int y)
	{
		if (key == GLUT_KEY_F6 && !cs6610::UserInput::UserInput::keys[key])
		{
			cs6610::UserInput::UserInput::keys.set(key);
			cs6610::Graphics::Effect::ReCompile();
		}
	}
	void keyReleaseSpecial(int key, int x, int y)
	{
		cs6610::UserInput::UserInput::keys.reset(key);
	}
}