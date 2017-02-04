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
#include "../UserInput/UserInput.h"

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
	void CallingRedisplay(void);
	void ReShapeCallback(int width, int height);
}

void cs6610::Graphics::RenderFrame(void)
{
	Time::OnNewFrame();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear color");
	glDepthMask(GL_TRUE);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set depth mask");
	glClearDepth(1.0f);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set clear depth");
	const GLbitfield clearColorAndDepth = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	glClear(clearColorAndDepth);
	CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to clear color buffer and depth buffer");

	size_t length = MyGame::ms_gameobjects.size();
	for (size_t i = 0; i < length; i++)
	{
		MyGame::ms_gameobjects[i]->GetEffect()->Bind();
		cyMatrix4f model;
		cyMatrix4f view;
		cyMatrix4f projection;
		cyMatrix3f normal;
		if (UserInput::UserInput::isCameraPerspective)
		{
			model = cyMatrix4f::MatrixScale(0.05f)*cyMatrix4f::MatrixTrans(MyGame::ms_gameobjects[i]->GetPosition());

			cyMatrix4f trans = cyMatrix4f::MatrixTrans(MyGame::ms_pcamera->GetPosition());
			cyMatrix4f xrot = cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(MyGame::ms_pcamera->GetEularAngles().x));
			cyMatrix4f yrot = cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(MyGame::ms_pcamera->GetEularAngles().y));
			view = trans*yrot*xrot;

			normal = cyMatrix3f(((view*model).GetInverse()).GetTranspose());

			projection = MyGame::ms_pcamera->GetPerspectiveProjectionMatrix();
		}
		else
		{
			const float scalingFactor = 1.0f / (MyGame::ms_gameobjects[i]->GetPosition() - MyGame::ms_ocamera->GetPosition()).Length();
			model = cyMatrix4f::MatrixScale(scalingFactor)*cyMatrix4f::MatrixTrans(MyGame::ms_gameobjects[i]->GetPosition());

			cyMatrix4f trans = cyMatrix4f::MatrixTrans(MyGame::ms_ocamera->GetPosition());
			cyMatrix4f xrot = cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(MyGame::ms_ocamera->GetEularAngles().x));
			cyMatrix4f yrot = cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(MyGame::ms_ocamera->GetEularAngles().y));
			view = trans*yrot*xrot;

			normal = cyMatrix3f(((view*model).GetInverse()).GetTranspose());

			projection = MyGame::ms_ocamera->GetOrthographicProjectionMatrix();
		}
		cyGLSLProgram* program = MyGame::ms_gameobjects[i]->GetEffect()->GetProgram();
		program->SetUniform(0, model);
		program->SetUniform(1, view);
		program->SetUniform(2, projection);
		program->SetUniform(3, normal);
		program->SetUniform(4, cyPoint3f(0.0f,-10.0f,0.0f));
		MyGame::ms_gameobjects[i]->GetMesh()->RenderMesh();
	}

	glutSwapBuffers();
}
bool cs6610::Graphics::Initialize(int i_argumentCount, char** i_arguments)
{
	bool wereThereErrors = false;
	glutInit(&i_argumentCount, i_arguments);
	const GLbitfield enableRGBAChannelsAndDepthAndDoubleBuffer = GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE;
	glutInitDisplayMode(enableRGBAChannelsAndDepthAndDoubleBuffer);
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
	{
		glEnable(GL_DEPTH_TEST);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to enable depth buffer");
		glDepthFunc(GL_LESS);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set depth func");
		glDepthMask(GL_TRUE);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set depth mask");
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
			//cs6610::MyGame::ms_pcamera->UpdateCurrentCameraPosition();
			glutPostWindowRedisplay(currentWindowID);
		}
	}
	void ReShapeCallback(int width, int height)
	{
		cs6610::Camera::Camera::ms_aspectRatio = static_cast<float>(width) / height;
		glViewport(0, 0, width, height);
	}
}