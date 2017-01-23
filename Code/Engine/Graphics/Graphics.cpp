#include "Graphics.h"

#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../Math/Functions.h"

#include <string>
#include <cassert>
#include <iostream>
namespace
{
	const int windowPositionX = 100;
	const int windowPositionY = 100;
	const int windowWidth = 800;
	const int windowHeight = 600;
	const std::string windowTitle = "Ameya Gadkari's CS6610 Game";
	int currentWindowID = 0;
	void CalculateNewClearColor(int speed);
	struct
	{
		GLclampf r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
	}clearColor;
	const int delay = 215;
	const int colorChangeSpeed = 10;
}

void cs6610::Graphics::RenderFrame(void)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	assert(glGetError() == GL_NO_ERROR);
	const GLbitfield clearColorBuffer = GL_COLOR_BUFFER_BIT;
	glClear(clearColorBuffer);
	assert(glGetError() == GL_NO_ERROR);
	glutSwapBuffers();
}
bool cs6610::Graphics::Initialize(int i_argumentCount, char** i_arguments)
{
	bool wereThereErrors = false;
	glutInit(&i_argumentCount, i_arguments);
	const GLbitfield enableRGBAChannelsAndDoubleBuffer = GLUT_RGBA | GLUT_DOUBLE;
	glutInitDisplayMode(enableRGBAChannelsAndDoubleBuffer);
	glutInitWindowPosition(windowPositionX, windowPositionY);
	glutInitWindowSize(windowWidth, windowHeight);
	currentWindowID = glutCreateWindow(windowTitle.c_str());
	if (currentWindowID <= 0)
	{
		assert(false);
		std::cerr << "Window creation failed" << std::endl;
		wereThereErrors = true;
	}
	glutDisplayFunc(RenderFrame);
	glutTimerFunc(delay, CalculateNewClearColor, colorChangeSpeed);
	const GLenum option = GLUT_ACTION_ON_WINDOW_CLOSE;
	const int mode = GLUT_ACTION_GLUTMAINLOOP_RETURNS;
	glutSetOption(option, mode);
	return !wereThereErrors;
}

namespace
{
	void CalculateNewClearColor(int speed)
	{
		float t = cosf(static_cast<GLfloat>(glutGet(GLUT_ELAPSED_TIME))*1.0f/speed);
		clearColor.b = (t<0)?(t*-1):t;
		glutTimerFunc(delay, CalculateNewClearColor, colorChangeSpeed);
		glutPostWindowRedisplay(currentWindowID);
	}
}