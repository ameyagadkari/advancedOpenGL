#include "Graphics.h"

#define GLEW_STATIC
#include "../../External/GLEW/glew.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../Asserts/Asserts.h"
#include "../../Game/MyGame/MyGame.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "Mesh.h"
#include "Effect.h"
#include "../Time/Time.h"
#include "../Camera/Camera.h"
#include "../../External/cyCodeBase/cyPoint.h"
#include "../../External/cyCodeBase/cyMatrix.h"
#include "../../External/cyCodeBase/cyGL.h"

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
	void ReShapeCallback(int width, int height);
	cs6610::Camera::Camera s_camera;
}

void cs6610::Graphics::RenderFrame(void)
{
	Time::OnNewFrame();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear color");
	const GLbitfield clearColorBuffer = GL_COLOR_BUFFER_BIT;
	glClear(clearColorBuffer);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to clear color buffer");

	size_t length = MyGame::ms_gameobjects.size();
	for (size_t i = 0; i < length; i++)
	{
		MyGame::ms_gameobjects[i]->GetEffect()->Bind();
		MyGame::ms_gameobjects[i]->GetEffect()->GetProgram()->SetUniform(0, cyMatrix4f::MatrixScale(0.05f));
		//MyGame::ms_gameobjects[i]->GetEffect()->GetProgram()->SetUniform(1, cyMatrix4f::MatrixView(s_camera.m_position, MyGame::ms_gameobjects[i]->GetPosition(), cyPoint3f(0.0f, 1.0f, 0.0f)));

		//MyGame::ms_gameobjects[i]->GetEffect()->GetProgram()->SetUniform(2, cyMatrix4f::MatrixPerspective(s_camera.m_fieldOfView, Camera::Camera::ms_aspectRatio, s_camera.m_nearPlaneDistance, s_camera.m_farPlaneDistance));
		MyGame::ms_gameobjects[i]->GetMesh()->RenderMesh();
	}
	
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
	glutReshapeFunc(ReShapeCallback);
	glutDisplayFunc(RenderFrame);
	glutIdleFunc(CallingRedisplay);
	const GLenum option = GLUT_ACTION_ON_WINDOW_CLOSE;
	const int mode = GLUT_ACTION_GLUTMAINLOOP_RETURNS;
	glutSetOption(option, mode);
	return !wereThereErrors;
}

namespace
{
	void CallingRedisplay(void)
	{
		if (cs6610::Time::GetElapsedTimeDuringPreviousFrame() > FPS)
		{
			//s_camera.UpdateCurrentCameraOrientation();
			//s_camera.UpdateCurrentCameraPosition();
			glutPostWindowRedisplay(currentWindowID);
		}
	}
	void ReShapeCallback(int width, int height)
	{
		cs6610::Camera::Camera::ms_aspectRatio = static_cast<float>(width) / height;
		glViewport(0, 0, width, height);
	}
}