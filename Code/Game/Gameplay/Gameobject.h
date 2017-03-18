#pragma once

#include "../../External/cyCodeBase/cyPoint.h"

#include <vector>

namespace cs6610
{
	namespace Graphics
	{
		class Mesh;
		class Material;
	}
}

namespace cs6610
{
	namespace Gameplay
	{
		class GameObject
		{
		public:
			explicit GameObject(const cyPoint3f i_position = cyPoint3f(0.0f, 0.0f, 0.0f), const cyPoint3f i_eularAngles = cyPoint3f(0.0f, 0.0f, 0.0f), const cyPoint3f i_scale = cyPoint3f(1.0f, 1.0f, 1.0f));
			~GameObject();
			void UpdatePosition(float xOffset = 0.0f, float yOffset = 0.0f);
			void UpdateOrientation(float xOffset = 0.0f, float yOffset = 0.0f);
			void LoadMeshAndMaterial(const bool i_isCubeMap, const std::string i_meshRelativePath, const std::vector<std::string> i_shaderPaths, const std::string i_texturePathPrefix, const char * const i_names = nullptr);
#pragma region Gets
			Graphics::Material* GetMaterial()const;
			Graphics::Mesh* GetMesh()const;
			cyPoint3f GetPosition()const;
			cyPoint3f GetOrientationEular()const;
			cyPoint3f GetScale()const;
#pragma endregion
#pragma region Sets
			void SetMaterial(Graphics::Material* const i_material, const char * const i_names = nullptr);
			void SetMesh(Graphics::Mesh* const i_mesh);
			void SetPosition(const cyPoint3f i_position);
			void SetOrientationEular(const cyPoint3f i_eularAngles);
#pragma endregion 
		private:
			Graphics::Mesh* m_mesh;
			Graphics::Material* m_material;
			cyPoint3f m_position;
			cyPoint3f m_eularAngles;
			cyPoint3f m_scale;
		};
	}
}