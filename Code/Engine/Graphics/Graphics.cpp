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
	cyGLRenderDepth2D* secondarySceneRenderBuffer = nullptr;

}

void cs6610::Graphics::RenderFrame()
{
	Time::OnNewFrame();
	UniformBufferData::DrawcallBuffer drawcallBufferData;
	cyMatrix3f normal;
	Gameplay::GameObject* teapot = MyGame::mainScene->GetGameobjectByName("Teapot");
	Gameplay::GameObject* light = MyGame::mainScene->GetGameobjectByName("Light");
	//Gameplay::GameObject* plane = MyGame::mainScene->GetGameobjectByName("Plane");

	// Draw Main Scene
	{
		MyGame::mainScene->RenderScene();

		// Draw the light
		{
			Material* lightMaterial = light->GetMaterial();			

			drawcallBufferData.model =
				cyMatrix4f::MatrixScale(light->GetScale())*
				cyMatrix4f::MatrixTrans(light->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(light->GetOrientationEular().y))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(light->GetOrientationEular().x));
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();

			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			for (size_t i = 0; i < lightMaterial->m_numbeOfMaterials; i++)
			{
				lightMaterial->Bind(i);
				light->GetMesh()->RenderMesh(i);
			}
		}

		// Draw Teapot
		{
			Material* teapotMaterial = teapot->GetMaterial();
			teapotMaterial->Bind();

			drawcallBufferData.model =
				cyMatrix4f::MatrixScale(teapot->GetScale())*
				cyMatrix4f::MatrixTrans(teapot->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().y))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().x));
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();

			normal = cyMatrix3f(((drawcallBufferData.view*drawcallBufferData.model).GetInverse()).GetTranspose());
			cyGLSLProgram* program = teapotMaterial->GetEffect()->GetProgram();
			program->SetUniform(0, normal);
			program->SetUniform(1, light->GetPosition());

			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			teapot->GetMesh()->RenderMesh();
		}
	}

	/*// Draw Secondary Scene
	{
		MyGame::secondaryScene->RenderScene();

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

			cyMatrix4f modelLight =
				cyMatrix4f::MatrixScale(light->GetScale())*
				cyMatrix4f::MatrixTrans(teapot->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(light->GetOrientationEular().z))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(light->GetOrientationEular().x))*
				cyMatrix4f::MatrixTrans(light->GetPosition() - teapot->GetPosition());

			lightPositionWorld = cyPoint3f(modelLight*cyPoint4f(light->GetPosition(), 1.0f));

			drawcallBufferData.model =
				cyMatrix4f::MatrixScale(teapot->GetScale())*
				cyMatrix4f::MatrixTrans(teapot->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().z))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().x));

				drawcallBufferData.view = cyMatrix4f::MatrixView(lightPositionWorld, teapot->GetPosition(), cyPoint3f(0.0f, 1.0f, 0.0f));
			drawcallBufferData.projection = cyMatrix4f::MatrixPerspective(Math::ConvertDegreesToRadians(45.0f), Camera::Camera::ms_aspectRatio, 0.01f, 500.0f);
			//normal = cyMatrix3f(((drawcallBufferData.view*drawcallBufferData.model).GetInverse()).GetTranspose());
			cyGLSLProgram* program = teapotMaterial->GetEffect()->GetProgram();	
			//program->SetUniform(0, normal);
			//program->SetUniform(1, lightPositionWorld);
			program->SetUniform(2, true);
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			teapot->GetMesh()->RenderMesh();
		}
		secondarySceneRenderBuffer->Unbind();
	}
	
	// Draw Main Scene
	{
		secondarySceneRenderBuffer->BindTexture();
		MyGame::mainScene->RenderScene();
		
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
			//drawcallBufferData.viewInv = MyGame::mainScene->GetCamera()->GetViewMatrix().GetInverse();

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
			drawcallBufferData.viewInv = cyMatrix4f::MatrixTrans(cyPoint3f(0.5f, 0.5f, 0.495f)) * cyMatrix4f::MatrixScale(0.5f, 0.5f, 0.5f) *cyMatrix4f::MatrixPerspective(Math::ConvertDegreesToRadians(45.0f), Camera::Camera::ms_aspectRatio, 0.01f, 500.0f)*cyMatrix4f::MatrixView(lightPositionWorld, teapot->GetPosition(), cyPoint3f(0.0f, 1.0f, 0.0f));
			cyGLSLProgram* program = teapotMaterial->GetEffect()->GetProgram();
			program->SetUniform(0, normal);
			program->SetUniform(1, lightPositionWorld);
			program->SetUniform(2, false);
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			teapot->GetMesh()->RenderMesh();
		}
		//Draw Plane
		{
			plane->GetMaterial()->Bind();
			
			drawcallBufferData.model =
				cyMatrix4f::MatrixScale(plane->GetScale())*
				cyMatrix4f::MatrixTrans(plane->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(plane->GetOrientationEular().z))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(plane->GetOrientationEular().x));
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			drawcallBufferData.viewInv = cyMatrix4f::MatrixTrans(cyPoint3f(0.5f, 0.5f, 0.495f)) * cyMatrix4f::MatrixScale(0.5f, 0.5f, 0.5f) *cyMatrix4f::MatrixPerspective(Math::ConvertDegreesToRadians(45.0f), Camera::Camera::ms_aspectRatio, 0.01f, 500.0f)*cyMatrix4f::MatrixView(lightPositionWorld, teapot->GetPosition(), cyPoint3f(0.0f, 1.0f, 0.0f));
			normal = cyMatrix3f(((drawcallBufferData.view*drawcallBufferData.model).GetInverse()).GetTranspose());
			cyGLSLProgram* program = plane->GetMaterial()->GetEffect()->GetProgram();
			//program->SetUniform(0, 0.01f);
			//program->SetUniform(1, 500.0f);
			program->SetUniform(0, normal);
			program->SetUniform(1, light->GetPosition());
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

			plane->GetMesh()->RenderMesh();
		}
	}

	// Draw Secondary Scene
	{
		MyGame::secondaryScene->RenderScene();
		/*glDepthMask(GL_FALSE);
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
				cyMatrix4f::MatrixScale(1.0f, -1.0f, 1.0f)*
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
				cyMatrix4f::MatrixScale(1.0f, -1.0f, 1.0f)*
				cyMatrix4f::MatrixScale(teapot->GetScale())*
				cyMatrix4f::MatrixTrans(teapot->GetPosition())*
				cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().z))*
				cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(teapot->GetOrientationEular().x));
			drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
			drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
			normal = cyMatrix3f(((drawcallBufferData.view*drawcallBufferData.model).GetInverse()).GetTranspose());
			cyGLSLProgram* program = teapotMaterial->GetEffect()->GetProgram();
			program->SetUniform(0, normal);
			program->SetUniform(1, lightPositionWorld);
			s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));
			teapot->GetMesh()->RenderMesh();
		}
		secondarySceneRenderBuffer->Unbind();
	}
	//Draw Plane
	{
		plane->GetMaterial()->Bind();
		secondarySceneRenderBuffer->BindTexture(1);
		envCube->GetMaterial()->m_texturesCubeMap->Bind(0);
		drawcallBufferData.model =
			cyMatrix4f::MatrixScale(plane->GetScale())*
			cyMatrix4f::MatrixTrans(plane->GetPosition())*
			cyMatrix4f::MatrixRotationY(Math::ConvertDegreesToRadians(plane->GetOrientationEular().z))*
			cyMatrix4f::MatrixRotationX(Math::ConvertDegreesToRadians(plane->GetOrientationEular().x));
		drawcallBufferData.view = MyGame::mainScene->GetCamera()->GetViewMatrix();
		drawcallBufferData.projection = MyGame::mainScene->GetCamera()->GetPerspectiveProjectionMatrix();
		drawcallBufferData.viewInv = MyGame::mainScene->GetCamera()->GetViewMatrix().GetInverse();
		normal = cyMatrix3f(((drawcallBufferData.view*drawcallBufferData.model).GetInverse()).GetTranspose());
		cyGLSLProgram* program = plane->GetMaterial()->GetEffect()->GetProgram();
		program->SetUniform(2, normal);
		program->SetUniform(0, 0);
		program->SetUniform(1, 1);
		s_drawcallBuffer->Update(&drawcallBufferData, sizeof(drawcallBufferData));

		plane->GetMesh()->RenderMesh();
	}*/
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
		/*if (!secondarySceneRenderBuffer)secondarySceneRenderBuffer = cs6610::MyGame::secondaryScene->GetRenderBuffer();
		if (!secondarySceneRenderBuffer->Resize(width, height))
		{
			CS6610_ASSERTF(false, "RenderBuffer is not ready");
		}*/
	}
}