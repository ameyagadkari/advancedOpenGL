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
	void close(void);
	int xPosOnPress = 0, yPosOnPress = 0;
	float xOffest = 0, yOffset = 0;
	const float mouseSensitivity = 0.25f;
	const float mouseSensitivityCam = 0.025f;
	int yPosOnPressCam = 0;
	float yOffsetCam = 0;
}

std::bitset<256> cs6610::UserInput::UserInput::keys;
float cs6610::UserInput::UserInput::xRot = -90.0f;
float cs6610::UserInput::UserInput::zRot = 0.0f;

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
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				xPosOnPress = x;
				yPosOnPress = y;
			}
			else if (state == GLUT_UP)
			{
				xOffest = static_cast<float>(x - xPosOnPress);
				yOffset = static_cast<float>(y - yPosOnPress);

				xOffest *= mouseSensitivity;
				yOffset *= mouseSensitivity;

				cs6610::UserInput::UserInput::xRot += xOffest;
				cs6610::UserInput::UserInput::zRot += yOffset;
			}
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				yPosOnPressCam = y;
			}
			else if (state == GLUT_UP)
			{
				yOffsetCam = static_cast<float>(y - yPosOnPressCam);

				yOffsetCam *= mouseSensitivityCam;

				cyPoint3f currentCameraPosition = cs6610::MyGame::ms_camera->GetPosition();
				currentCameraPosition.z += yOffsetCam;

				cs6610::MyGame::ms_camera->SetPosition(currentCameraPosition);
			}
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