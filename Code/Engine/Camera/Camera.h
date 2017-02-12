#pragma once

#include "../../External/cyCodeBase/cyPoint.h"
#include "../Math/Functions.h"
#include "../../External/cyCodeBase/cyMatrix.h"

namespace cs6610
{
	namespace Camera
	{
		class Camera
		{
		public:
			explicit Camera(cyPoint3f i_position = cyPoint3f(0.0f, -5.0f, 0.0f), cyPoint3f i_eularAngles = cyPoint3f(-90.0f, 0.0f, 0.0f), float i_fieldOfView = Math::ConvertDegreesToRadians(45.0f), float i_nearPlaneDistance = 0.1f, float i_farPlaneDistance = 100.0f, float i_topPlaneDistance = 20.0f, float i_bottomPlaneDistance = -20.0f, float i_leftPlaneDistance = -40.0f, float i_rightPlaneDistance = 40.0f);
			~Camera();

			void UpdateCurrentCameraPosition();
			void UpdateCurrentCameraOrientation(bool constrainPitch = true);



			static float ms_aspectRatio;

#pragma region Sets
			void SetPosition(cyPoint3f i_position);
#pragma endregion

#pragma region Gets
			cyMatrix4f GetOrthographicProjectionMatrix()const;
			cyMatrix4f GetPerspectiveProjectionMatrix()const;
			cyPoint3f GetPosition()const;
			cyPoint3f GetEularAngles()const;
			float GetFieldOfView()const;
			float GetNearPlaneDistance()const;
			float GetFarPlaneDistance()const;
			cyMatrix4f GetViewMatrix()const;
#pragma endregion

		private:
			cyPoint3f m_position;
			cyPoint3f m_eularAngles;

			struct
			{
				cyPoint3f m_right;
				cyPoint3f m_up;
				cyPoint3f m_front;
			}m_localCameraAxis;

			float m_fieldOfView;
			float m_nearPlaneDistance;
			float m_farPlaneDistance;

			float m_topPlaneDistance;
			float m_bottomPlaneDistance;
			float m_leftPlaneDistance;
			float m_rightPlaneDistance;

			cyMatrix4f m_orthographicProjection;

			void UpdateLocalCameraAxes();
			void CalculateOrthographicProjectionMatrix();
		};
	}
}
