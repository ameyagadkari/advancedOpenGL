#include "UserInput.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Game/MyGame/MyGame.h"
#include "../Graphics/Effect.h"
#include "../../External/cyCodeBase/cyPoint.h"
#include "../Camera/Camera.h"
#include "../../Game/Gameplay/Gameobject.h"

namespace
{
	void keyPress(unsigned char c, int x, int y);
	void keyRelease(unsigned char c, int x, int y);
	void keyPressSpecial(int key, int x, int y);
	void keyReleaseSpecial(int key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void close(void);

	int xPosOnPressLMB = 0, yPosOnPressLMB = 0;
	bool lmbFirstPress = true;
	bool lmbStillPressed = false;

	int yPosOnPressRMB = 0;
	bool rmbFirstPress = true;
	bool rmbStillPressed = false;

	int xPosOnPressCtrlLMB = 0, yPosOnPressCtrlLMB = 0;
	bool ctrlLMBFirstPress = true;
	bool ctrlLMBStillPressed = false;
}

std::bitset<256> cs6610::UserInput::UserInput::keys;
bool cs6610::UserInput::UserInput::isCameraPerspective = true;

bool cs6610::UserInput::UserInput::Initialize(void)
{
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutSpecialFunc(keyPressSpecial);
	glutSpecialUpFunc(keyReleaseSpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutCloseFunc(close);
	return true;
}

namespace
{
	void keyPress(unsigned char c, int x, int y)
	{
		if (c == 'p' && !cs6610::UserInput::UserInput::keys['p'])
		{
			cs6610::UserInput::UserInput::isCameraPerspective = !cs6610::UserInput::UserInput::isCameraPerspective;
		}
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
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
				{
					if (ctrlLMBFirstPress)
					{
						xPosOnPressCtrlLMB = x;
						yPosOnPressCtrlLMB = y;
						ctrlLMBFirstPress = false;
						ctrlLMBStillPressed = true;
					}
				}
				else
				{
					if (lmbFirstPress)
					{
						xPosOnPressLMB = x;
						yPosOnPressLMB = y;
						lmbFirstPress = false;
						lmbStillPressed = true;
					}
				}
			}
			else if (state == GLUT_UP)
			{
				lmbFirstPress = true;
				lmbStillPressed = false;

				ctrlLMBFirstPress = true;
				ctrlLMBStillPressed = false;
			}
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if (lmbFirstPress)
				{
					yPosOnPressRMB = y;
					rmbFirstPress = false;
					rmbStillPressed = true;
				}
			}
			else if (state == GLUT_UP)
			{
				rmbFirstPress = true;
				rmbStillPressed = false;
			}
		}
	}
	void mouseMotion(int x, int y)
	{
		if (lmbStillPressed)
		{
			float xOffsetLMB = static_cast<float>(x - xPosOnPressLMB);
			float yOffsetLMB = static_cast<float>(yPosOnPressLMB - y);
			xPosOnPressLMB = x;
			yPosOnPressLMB = y;
			cs6610::MyGame::ms_gameobjects.at("Teapot")->UpdateOrientation(xOffsetLMB, yOffsetLMB);
		}
		if (rmbStillPressed)
		{
			float yOffsetRMB = static_cast<float>(yPosOnPressRMB - y);
			yPosOnPressRMB = y;
			cs6610::MyGame::ms_gameobjects.at("Teapot")->UpdatePosition(yOffsetRMB);
		}
		if (ctrlLMBStillPressed)
		{
			float xOffsetCtrlLMB = static_cast<float>(x - xPosOnPressCtrlLMB);
			float yOffsetCtrlLMB = static_cast<float>(yPosOnPressCtrlLMB - y);
			xPosOnPressCtrlLMB = x;
			yPosOnPressCtrlLMB = y;
			cs6610::MyGame::ms_gameobjects.at("Light")->UpdateOrientation(xOffsetCtrlLMB, yOffsetCtrlLMB);
		}
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