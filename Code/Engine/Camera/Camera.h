#pragma once

#include "../../External/cyCodeBase/cyPoint.h"
#include "../Math/Functions.h"

namespace cs6610
{
	namespace Camera
	{
		class Camera
		{
		public:
			explicit Camera(cyPoint3f i_position = cyPoint3f(10.0f, 0.0f, 0.0f), cyPoint3f i_eularAngles = cyPoint3f(0.0f, 0.0f, 0.0f), float i_fieldOfView = Math::ConvertDegreesToRadians(45.0f), float i_nearPlaneDistance = 0.1f, float i_farPlaneDistance = 100.0f);
			~Camera();

			void UpdateCurrentCameraPosition();
			void UpdateCurrentCameraOrientation(bool constrainPitch = true);

			static float ms_aspectRatio;
			//private:
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

			void UpdateLocalCameraAxes();
		};
	}
}
