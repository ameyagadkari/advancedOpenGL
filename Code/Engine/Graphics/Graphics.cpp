#include "Graphics.h"

#define GLEW_STATIC
#include "../../External/GLEW/glew.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../Asserts/Asserts.h"
#include "../../External/cyCodeBase/cyTimer.h"
#include "../../Game/MyGame/MyGame.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "Mesh.h"

namespace
{
#define FPS 0.0166666666666667
	const int windowPositionX = 100;
	const int windowPositionY = 100;
	const int windowWidth = 800;
	const int windowHeight = 600;
	const std::string windowTitle = "Ameya Gadkari's CS6610 Game "
		" -- "
#ifdef _WIN64
		"x64"
#else
		"x86"
#endif
		" -- "
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
		;
	int currentWindowID = 0;
	struct
	{
		GLclampf r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
	}clearColor;
	void CallingRedisplay(void);
	cyTimer s_timer;
	double s_currentTime = 0, s_elaspsedTime = 0;
}

void cs6610::Graphics::RenderFrame(void)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear color");
	const GLbitfield clearColorBuffer = GL_COLOR_BUFFER_BIT;
	glClear(clearColorBuffer);

	size_t length = MyGame::ms_gameobjects.size();
	for (size_t i = 0; i < length; i++)
	{
		MyGame::ms_gameobjects[i]->GetMesh()->RenderMesh();
	}

	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to clear color buffer");
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
		CS6610_ASSERTF(false, "Window creation failed");
		wereThereErrors = true;
	}
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		CS6610_ASSERTF(false, "Failed to initialize GLEW");
		wereThereErrors = true;
	}
	glutDisplayFunc(RenderFrame);
	glutIdleFunc(CallingRedisplay);
	const GLenum option = GLUT_ACTION_ON_WINDOW_CLOSE;
	const int mode = GLUT_ACTION_GLUTMAINLOOP_RETURNS;
	glutSetOption(option, mode);
	s_timer.Start();
	return !wereThereErrors;
}

namespace
{
	void CallingRedisplay(void)
	{
		s_currentTime = s_timer.Stop();
		if (s_currentTime - s_elaspsedTime > FPS)
		{
			glutPostWindowRedisplay(currentWindowID);
			s_elaspsedTime = s_currentTime;
		}
	}
}