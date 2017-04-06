#include "Graphics.h"

#include "../../External/cyCodeBase/cyGL.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../Asserts/Asserts.h"
#include "../../Game/MyGame/MyGame.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "Mesh.h"
#include "Effect.h"
#include "Material.h"
#include "../Time/Time.h"
#include "../Camera/Camera.h"
#include "UniformBuffer.h"
#include "UniformBufferData.h"
#include "Scene.h"
#include "../Math/Functions.h"

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
	void CallingRedisplay();
	void ReShapeCallback(int width, int height);

	cs6610::Graphics::UniformBuffer* s_drawcallBuffer = nullptr;
}

void cs6610::Graphics::RenderFrame()
{
	Time::OnNewFrame();

	UniformBufferData::DrawcallBuffer drawcallBufferData;
	Gameplay::GameObject* skybox = MyGame::mainScene->GetGameobjectByName("Skybox");
	Gameplay::GameObject* water = MyGame::mainScene->GetGameobjectByName("Water");
	//Gameplay::GameObject* lavaland = MyGame::mainScene->GetGameobjectByName("Lavaland");
	Gameplay::GameObject* cartoonland = MyGame::mainScene->GetGameobjectByName("CartoonLand");

	// Draw Reflection Texture
	{
		MyGame::reflectionTexture->RenderScene();
		glEnable(GL_CLIP_DISTANCE0);
		//Draw Cartoon Land
		{
			Material* cartoonlandMaterial = cartoonland->GetMaterial();
			Mesh* cartoonlandMesh = cartoonland->GetMesh();
			float distanceToMoveCameraBelowWater = 2.0f*(MyGame::mainScene->GetCamera()->GetPosition().y - water->GetPosition().y);
			MyGame::mainScene->GetCamera()->SetPosition(MyGame::mainScene->GetCamera()->GetPosition() - cyPoint3f(0.0f, distanceToMoveCameraBelowWater, 0.0f));
			MyGame::mainScene->GetCamera()->InvertPitch();
			drawcallBufferData.model = cyMatrix4f::MatrixScale(1.0f);
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			//int id = glGetUniformLocation(cartoonlandMaterial->GetEffect()->GetProgram()->GetID(), "u_clippingplane");
			////cyPoint4f cp = cyPoint4f(0.0f, 1.0f, 0.0f, -water->GetPosition().y);
			//float cp[4] = { 0.0f, 1.0f, 0.0f, -water->GetPosition().y };
			//CS6610_ASSERT(glGetError() == GL_NO_ERROR);
			//glUniform4fv(id, 1, cp);
			////cartoonlandMaterial->GetEffect()->GetProgram()->SetUniform(0, cp);
			//CS6610_ASSERT(glGetError() == GL_NO_ERROR);
			for (size_t i = 0; i < cartoonlandMaterial->GetNumberOfMaterials(); i++)
			{
				cartoonlandMaterial->Bind(i);
				cartoonlandMesh->RenderMesh(i);
			}
			MyGame::mainScene->GetCamera()->SetPosition(MyGame::mainScene->GetCamera()->GetPosition() + cyPoint3f(0.0f, distanceToMoveCameraBelowWater, 0.0f));
			MyGame::mainScene->GetCamera()->InvertPitch();
		}
		MyGame::reflectionTexture->GetRenderBuffer()->Unbind();
	}

	// Draw Refraction Texture
	{
		MyGame::refractionTexture->RenderScene();
		
		//Draw Cartoon Land
		{
			Material* cartoonlandMaterial = cartoonland->GetMaterial();
			Mesh* cartoonlandMesh = cartoonland->GetMesh();
			drawcallBufferData.model = cyMatrix4f::MatrixScale(1.0f);
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			//cartoonlandMaterial->GetEffect()->GetProgram()->SetUniform(0, cyPoint4f(0.0f, -1.0f, 0.0f, water->GetPosition().y));
			for (size_t i = 0; i < cartoonlandMaterial->GetNumberOfMaterials(); i++)
			{
				cartoonlandMaterial->Bind(i);
				cartoonlandMesh->RenderMesh(i);
			}
		}
		MyGame::refractionTexture->GetRenderBuffer()->Unbind();
	}

	// Draw Main Scene
	{
		MyGame::mainScene->RenderScene();
		glDisable(GL_CLIP_DISTANCE0);
		glDepthMask(GL_FALSE);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to reset depth mask for writing to depth buffer");
		//Draw Env Cube
		{
			skybox->GetMaterial()->Bind();
			drawcallBufferData.view = cyMatrix4f(cyMatrix3f(MyGame::mainScene->GetCamera()->GetViewMatrix()));
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			skybox->GetMesh()->RenderMesh();
		}
		glDepthMask(GL_TRUE);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to reset depth mask for writing to depth buffer");

		//Draw Water
		{
			water->GetMaterial()->Bind();
			drawcallBufferData.model = cyMatrix4f::MatrixTrans(water->GetPosition());
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			water->GetMesh()->RenderMesh();
		}

		//Draw Cartoon Land
		{
			Material* cartoonlandMaterial = cartoonland->GetMaterial();
			Mesh* cartoonlandMesh = cartoonland->GetMesh();
			drawcallBufferData.model = cyMatrix4f::MatrixScale(1.0f);
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			//cartoonlandMaterial->GetEffect()->GetProgram()->SetUniform(0, cyPoint4f(0.0f, -1.0f, 0.0f, 2.0f));
			for (size_t i = 0; i < cartoonlandMaterial->GetNumberOfMaterials(); i++)
			{
				cartoonlandMaterial->Bind(i);
				cartoonlandMesh->RenderMesh(i);
			}
		}
	}

	glutSwapBuffers();
}
bool cs6610::Graphics::Initialize(int i_argumentCount, char** i_arguments)
{
	bool wereThereErrors = false;
	glutInit(&i_argumentCount, i_arguments);
	glutInitContextFlags(GLUT_DEBUG);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitContextVersion(4, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
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
	{
		glEnable(GL_DEPTH_TEST);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to enable depth buffer");
		glDepthFunc(GL_LESS);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set depth func");
		glDepthMask(GL_TRUE);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to set depth mask for writing to depth buffer");
	}
	glEnable(GL_CLIP_DISTANCE0);
	const GLenum option = GLUT_ACTION_ON_WINDOW_CLOSE;
	const int mode = GLUT_ACTION_GLUTMAINLOOP_RETURNS;
	glutSetOption(option, mode);
	s_drawcallBuffer = new UniformBuffer(UniformBufferType::DRAWCALL, sizeof(UniformBufferData::DrawcallBuffer));
	s_drawcallBuffer->Bind();
	CY_GL_REGISTER_DEBUG_CALLBACK;
	return !wereThereErrors;
}

void cs6610::Graphics::CleanUp()
{
	if (s_drawcallBuffer)
	{
		delete s_drawcallBuffer;
		s_drawcallBuffer = nullptr;
	}
}

namespace
{
	void CallingRedisplay()
	{
		if (cs6610::Time::GetElapsedTimeDuringPreviousFrame() > FPS)
		{
			cs6610::MyGame::mainScene->GetCamera()->UpdateCurrentCameraPosition();
			glutPostWindowRedisplay(currentWindowID);
		}
	}
	void ReShapeCallback(int width, int height)
	{
		cs6610::Camera::Camera::ms_aspectRatio = static_cast<float>(width) / height;
		glViewport(0, 0, width, height);
		if (!cs6610::MyGame::reflectionTexture->GetRenderBuffer()->Resize(4, width, height))
		{
			CS6610_ASSERTF(false, "RenderBuffer is not ready");
		}
		if (!cs6610::MyGame::refractionTexture->GetRenderBuffer()->Resize(4, width, height))
		{
			CS6610_ASSERTF(false, "RenderBuffer is not ready");
		}
	}
}