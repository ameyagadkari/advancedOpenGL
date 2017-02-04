#include "UserInput.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Game/MyGame/MyGame.h"
#include "../Graphics/Effect.h"
#include "../../External/cyCodeBase/cyPoint.h"
#include "../Camera/Camera.h"

namespace
{
	void keyPress(unsigned char c, int x, int y);
	void keyRelease(unsigned char c, int x, int y);
	void keyPressSpecial(int key, int x, int y);
	void keyReleaseSpecial(int key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void close(void);
	int xPosOnPress = 0, yPosOnPress = 0;
	float xOffset = 0, yOffset = 0;
	const float mouseSensitivity = 0.25f;
	const float mouseSensitivityCam = 0.025f;
	int yPosOnPressCam = 0;
	float yOffsetCam = 0;
	bool lmbFirstPress = true;
	bool lmbStillPressed = false;
	bool rmbFirstPress = true;
	bool rmbStillPressed = false;
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
				if (lmbFirstPress)
				{
					xPosOnPress = x;
					yPosOnPress = y;
					lmbFirstPress = false;
					lmbStillPressed = true;
				}
			}
			else if (state == GLUT_UP)
			{
				lmbFirstPress = true;
				lmbStillPressed = false;
			}
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if (lmbFirstPress)
				{
					yPosOnPressCam = y;
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
			xOffset = static_cast<float>(x - xPosOnPress);
			yOffset = static_cast<float>(y - yPosOnPress);
			xPosOnPress = x;
			yPosOnPress = y;
			cs6610::UserInput::UserInput::isCameraPerspective ? cs6610::MyGame::ms_pcamera->UpdateCurrentCameraOrientation(xOffset, yOffset) : cs6610::MyGame::ms_ocamera->UpdateCurrentCameraOrientation(xOffset, yOffset);
		}
		if (rmbStillPressed)
		{
			yOffsetCam = static_cast<float>(y - yPosOnPressCam);
			yPosOnPressCam = y;
			cs6610::UserInput::UserInput::isCameraPerspective ? cs6610::MyGame::ms_pcamera->UpdateCurrentCameraPosition(yOffsetCam) : cs6610::MyGame::ms_ocamera->UpdateCurrentCameraPosition(yOffsetCam);
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