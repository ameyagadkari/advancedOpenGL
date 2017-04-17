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
//#include "../Math/Functions.h"

namespace
{
	const double fps = 1.0 / 60.0;
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

	cs6610::Graphics::UniformBuffer* drawcallBuffer = nullptr;

	const float waveSpeed = 0.03f;
	float moveFactor = 0.0f;
}

void cs6610::Graphics::RenderFrame()
{
	Time::OnNewFrame();

	UniformBufferData::DrawcallBuffer drawcallBufferData;

	Gameplay::GameObject const * const skybox = MyGame::mainScene->GetGameobjectByName("Skybox");
	Material const * const skyBoxMaterial = skybox->GetMaterial();
	cyGLSLProgram * const skyBoxProgram = skyBoxMaterial->GetEffect()->GetProgram();
	Mesh const * const skyboxMesh = skybox->GetMesh();

	Gameplay::GameObject const * const water = MyGame::mainScene->GetGameobjectByName("Water");
	Material const * const waterMaterial = water->GetMaterial();
	cyGLSLProgram * const waterProgram = waterMaterial->GetEffect()->GetProgram();

	Gameplay::GameObject const * const cartoonland = MyGame::mainScene->GetGameobjectByName("CartoonLand");
	Material const * const cartoonlandMaterial = cartoonland->GetMaterial();
	cyGLSLProgram * const cartoonlandProgram = cartoonlandMaterial->GetEffect()->GetProgram();
	Mesh const * const cartoonlandMesh = cartoonland->GetMesh();

	Gameplay::GameObject const * const light = MyGame::mainScene->GetGameobjectByName("Light");
	Material const * const lightMaterial = light->GetMaterial();

	Camera::Camera * const currentCamera = MyGame::mainScene->GetCamera();

	// Draw Reflection Texture
	{
		MyGame::reflectionTexture->RenderScene();

		glEnable(GL_CLIP_DISTANCE0);
		CS6610_ASSERT(glGetError() == GL_NO_ERROR);

		const cyPoint3f distanceToMoveCameraBelowWater(0.0f, 2.0f*(currentCamera->GetPosition().y - water->GetPosition().y), 0.0f);
		currentCamera->SetPosition(currentCamera->GetPosition() - distanceToMoveCameraBelowWater);
		currentCamera->InvertPitch();

		//Draw Env Cube
		{
			glDepthMask(GL_FALSE);
			CS6610_ASSERT(glGetError() == GL_NO_ERROR);

			drawcallBufferData.view = cyMatrix4f(cyMatrix3f(currentCamera->GetViewMatrix()));
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			skyBoxMaterial->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			skyBoxProgram->SetUniform(0, cyPoint4f(0.0f, 1.0f, 0.0f, -water->GetPosition().y));

			skyboxMesh->RenderMesh();

			glDepthMask(GL_TRUE);
			CS6610_ASSERT(glGetError() == GL_NO_ERROR);
		}

		//Draw Cartoon Land
		{
			drawcallBufferData.model = cyMatrix4f::MatrixScale(1.0f);
			drawcallBufferData.view = currentCamera->GetViewMatrix();
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			cartoonlandProgram->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			cartoonlandProgram->SetUniform(0, cyPoint4f(0.0f, 1.0f, 0.0f, -water->GetPosition().y));

			for (size_t i = 0; i < cartoonlandMaterial->GetNumberOfMaterials(); i++)
			{
				cartoonlandMaterial->Bind(i);
				cartoonlandMesh->RenderMesh(i);
			}
		}

		currentCamera->SetPosition(currentCamera->GetPosition() + distanceToMoveCameraBelowWater);
		currentCamera->InvertPitch();

		MyGame::reflectionTexture->GetRenderBuffer()->Unbind();
	}

	// Draw Refraction Texture
	{
		MyGame::refractionTexture->RenderScene();

		//Draw Env Cube
		{
			glDepthMask(GL_FALSE);
			CS6610_ASSERT(glGetError() == GL_NO_ERROR);

			drawcallBufferData.view = cyMatrix4f(cyMatrix3f(currentCamera->GetViewMatrix()));
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			skyBoxMaterial->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			skyBoxProgram->SetUniform(0, cyPoint4f(0.0f, -1.0f, 0.0f, water->GetPosition().y));

			skyboxMesh->RenderMesh();

			glDepthMask(GL_TRUE);
			CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to reset depth mask for writing to depth buffer");
		}

		//Draw Cartoon Land
		{
			drawcallBufferData.model = cyMatrix4f::MatrixScale(1.0f);
			drawcallBufferData.view = currentCamera->GetViewMatrix();
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			cartoonlandProgram->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			cartoonlandProgram->SetUniform(0, cyPoint4f(0.0f, -1.0f, 0.0f, water->GetPosition().y));

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
		CS6610_ASSERT(glGetError() == GL_NO_ERROR);

		//Draw Env Cube
		{
			glDepthMask(GL_FALSE);
			CS6610_ASSERT(glGetError() == GL_NO_ERROR);

			drawcallBufferData.view = cyMatrix4f(cyMatrix3f(currentCamera->GetViewMatrix()));
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			skyBoxMaterial->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

			skyboxMesh->RenderMesh();

			glDepthMask(GL_TRUE);
			CS6610_ASSERT(glGetError() == GL_NO_ERROR);
		}

		//Draw Water
		{
			drawcallBufferData.model =
				cyMatrix4f::MatrixTrans(water->GetPosition())*
				cyMatrix4f::MatrixScale(water->GetScale());
			drawcallBufferData.view = currentCamera->GetViewMatrix();
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			waterMaterial->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			MyGame::reflectionTexture->GetRenderBuffer()->BindTexture(10);
			MyGame::refractionTexture->GetRenderBuffer()->BindTexture(11);		
			moveFactor += waveSpeed * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame());
			moveFactor = fmodf(moveFactor, 1.0f);
			waterProgram->SetUniform(0, moveFactor);
			waterProgram->SetUniform(1, currentCamera->GetPosition());
			waterProgram->SetUniform(2, light->GetPosition());

			water->GetMesh()->RenderMesh();
		}

		//Draw Cartoon Land
		{
			drawcallBufferData.model = cyMatrix4f::MatrixScale(1.0f);
			drawcallBufferData.view = currentCamera->GetViewMatrix();
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			cartoonlandProgram->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

			for (size_t i = 0; i < cartoonlandMaterial->GetNumberOfMaterials(); i++)
			{
				cartoonlandMaterial->Bind(i);
				cartoonlandMesh->RenderMesh(i);
			}
		}

		//Draw Light
		{
			drawcallBufferData.model = cyMatrix4f::MatrixTrans(light->GetPosition())*
				cyMatrix4f::MatrixScale(light->GetScale());
			drawcallBufferData.view = currentCamera->GetViewMatrix();
			drawcallBufferData.projection = currentCamera->GetPerspectiveProjectionMatrix();

			lightMaterial->Bind();

			drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

			light->GetMesh()->RenderMesh();		
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
	const GLenum option = GLUT_ACTION_ON_WINDOW_CLOSE;
	const int mode = GLUT_ACTION_GLUTMAINLOOP_RETURNS;
	glutSetOption(option, mode);
	drawcallBuffer = new UniformBuffer(UniformBufferType::DRAWCALL, sizeof(UniformBufferData::DrawcallBuffer));
	drawcallBuffer->Bind();
	CY_GL_REGISTER_DEBUG_CALLBACK;
	return !wereThereErrors;
}

void cs6610::Graphics::CleanUp()
{
	if (drawcallBuffer)
	{
		delete drawcallBuffer;
		drawcallBuffer = nullptr;
	}
}

namespace
{
	void CallingRedisplay()
	{
		if (cs6610::Time::GetElapsedTimeDuringPreviousFrame() > fps)
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
			CS6610_ASSERTF(false, "Reflection RenderBuffer is not ready");
		}
		if (!cs6610::MyGame::refractionTexture->GetRenderBuffer()->Resize(4, width, height))
		{
			CS6610_ASSERTF(false, "Refraction RenderBuffer is not ready");
		}
	}
}