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
	cyGLRenderBuffer2D* secondarySceneRenderBuffer = nullptr;

}

void cs6610::Graphics::RenderFrame(void)
{
	Time::OnNewFrame();
	// Draw Main Scene
	{
		MyGame::mainScene->RenderScene();
		UniformBufferData::DrawcallBuffer drawcallBufferData;
		cyMatrix3f normal;
		cyPoint3f lightPositionWorld;
		Gameplay::GameObject* teapot = MyGame::mainScene->GetGameobjectByName("Teapot");
		Gameplay::GameObject* light = MyGame::mainScene->GetGameobjectByName("Light");
		Gameplay::GameObject* plane = MyGame::mainScene->GetGameobjectByName("Plane");
		Gameplay::GameObject* envCube = MyGame::mainScene->GetGameobjectByName("EnvCube");

		glDepthMask(GL_FALSE);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to reset depth mask for writing to depth buffer");

		//Draw Env Cube
		{
			envCube->GetMaterial()->Bind();
			drawcallBufferData.view = cyMatrix4f(cyMatrix3f(MyGame::mainScene->GetCamera()->GetViewMatrix()));
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			envCube->GetMesh()->RenderMesh();
		}

		glDepthMask(GL_TRUE);
		CS6610_ASSERTF(glGetError() == GL_NO_ERROR, "OpenGL failed to reset depth mask for writing to depth buffer");

		// Draw the light
		{
			
			Material* lightMaterial = light->GetMaterial();
			lightMaterial->Bind();
			drawcallBufferData.model =
				cyMatrix4f::MatrixScale(light->GetScale())*
				cyMatrix4f::MatrixTrans(teapot->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(light->GetOrientationEular().z))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(light->GetOrientationEular().x))*
				cyMatrix4f::MatrixTrans(light->GetPosition() - teapot->GetPosition());

			lightPositionWorld = cyPoint3f(drawcallBufferData.model*cyPoint4f(light->GetPosition(), 1.0f));

			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			drawcallBufferData.viewInv = MyGame::mainScene->GetCamera()->GetViewMatrix().GetInverse();

			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

			light->GetMesh()->RenderMesh();
		}
		// Draw Teapot
		{
			Material* teapotMaterial = teapot->GetMaterial();
			teapotMaterial->Bind();

			drawcallBufferData.model =
				cyMatrix4f::MatrixScale(teapot->GetScale())*
				cyMatrix4f::MatrixTrans(teapot->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().z))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().x));
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			normal = cyMatrix3f(((drawcallBufferData.view*drawcallBufferData.model).GetInverse()).GetTranspose());
			cyGLSLProgram* program = teapotMaterial->GetEffect()->GetProgram();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			program->SetUniform(0, normal);
			program->SetUniform(1, lightPositionWorld);

			teapot->GetMesh()->RenderMesh();
		}		
		//Draw Plane
		{
			plane->GetMaterial()->Bind();
			//secondarySceneRenderBuffer->BindTexture(0);
			drawcallBufferData.model =
				cyMatrix4f::MatrixScale(plane->GetScale())*
				cyMatrix4f::MatrixTrans(plane->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(plane->GetOrientationEular().z))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(plane->GetOrientationEular().x));
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

			plane->GetMesh()->RenderMesh();
		}

		

		//secondarySceneRenderBuffer->Unbind();
		//secondarySceneRenderBuffer->BuildTextureMipmaps();
	}

	//// Draw Main Scene
	//{
	//	MyGame::mainScene->RenderScene();
	//	UniformBufferData::DrawcallBuffer drawcallBufferData;
	//	Gameplay::GameObject* plane = MyGame::mainScene->GetGameobjectByName("Plane");
	//	
	//	//Draw Plane
	//	{
	//		plane->GetMaterial()->Bind();
	//		secondarySceneRenderBuffer->BindTexture(0);
	//		drawcallBufferData.model =
	//			cyMatrix4f::MatrixScale(plane->GetScale())*
	//			cyMatrix4f::MatrixTrans(plane->GetPosition())*
	//			cyMatrix4f::MatrixRotationZ(Math::ConvertDegreesToRadians(plane->GetOrientationEular().z))*
	//			cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(plane->GetOrientationEular().x));
	//		drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
	//		drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
	//		s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

	//		plane->GetMesh()->RenderMesh();
	//	}
	//}
	glutSwapBuffers();
}
bool cs6610::Graphics::Initialize(int i_argumentCount, char** i_arguments)
{
	bool wereThereErrors = false;
	glutInit(&i_argumentCount, i_arguments);
	glutInitContextFlags(GLUT_DEBUG);
	const GLbitfield enableRGBAChannelsAndDepthAndDoubleBuffer = GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE;
	glutInitDisplayMode(enableRGBAChannelsAndDepthAndDoubleBuffer);
	glutInitContextVersion(3, 3);
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
			cs6610::MyGame::mainScene->GetCamera()->UpdateCurrentCameraOrientation();
			cs6610::MyGame::mainScene->GetCamera()->UpdateCurrentCameraPosition();
			glutPostWindowRedisplay(currentWindowID);
		}
	}
	void ReShapeCallback(int width, int height)
	{
		cs6610::Camera::Camera::ms_aspectRatio = static_cast<float>(width) / height;
		glViewport(0, 0, width, height);
		/*if (!secondarySceneRenderBuffer)secondarySceneRenderBuffer = cs6610::MyGame::secondaryScene->GetRenderBuffer();
		if(!secondarySceneRenderBuffer->Resize(4, width, height))
		{
			CS6610_ASSERTF(false, "RenderBuffer is not ready");
		}*/
	}
}