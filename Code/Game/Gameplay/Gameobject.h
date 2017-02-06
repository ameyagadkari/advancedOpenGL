#pragma once

#include "../../External/cyCodeBase/cyPoint.h"

#include <string>

namespace cs6610
{
	namespace Graphics
	{
		class Mesh;
		class Effect;
	}
}

namespace cs6610
{
	namespace Gameplay
	{
		class GameObject
		{
		public:
			explicit GameObject(const cyPoint3f i_position = cyPoint3f(0.0f, 0.0f, 0.0f), const cyPoint3f i_eularAngles = cyPoint3f(0.0f, 0.0f, 0.0f));
			~GameObject();
			void UpdatePosition(float zOffset);
			void UpdateOrientation(float xOffset, float yOffset);
#pragma region Gets
			Graphics::Effect* GetEffect()const;
			Graphics::Mesh* GetMesh()const;
			cyPoint3f GetPosition()const;
			cyPoint3f GetOrientationEular()const;
#pragma endregion
#pragma region Sets
			void SetEffect(const std::string i_relativePathVertex, const std::string i_relativePathFragment, const char * const i_names = nullptr);
			void SetEffect(Graphics::Effect* const i_effect, const char * const i_names = nullptr);
			void SetMesh(const std::string i_meshRelativePath);
			void SetMesh(Graphics::Mesh* const i_mesh);
			void SetPosition(const cyPoint3f i_position);
			void SetOrientationEular(const cyPoint3f i_eularAngles);
#pragma endregion 
		private:
			Graphics::Mesh* m_mesh;
			Graphics::Effect* m_effect;
			cyPoint3f m_position;
			cyPoint3f m_eularAngles;
		};
	}
}