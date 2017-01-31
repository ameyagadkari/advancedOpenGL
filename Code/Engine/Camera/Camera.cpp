#include "Camera.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Time/Time.h"

float cs6610::Camera::Camera::ms_aspectRatio = 0.0f;

cs6610::Camera::Camera::Camera(
	cyPoint3f i_position, 
	cyPoint3f i_eularAngles,
	float i_fieldOfView,
	float i_nearPlaneDistance,
	float i_farPlaneDistance)
	:
	m_position(i_position),
	m_eularAngles(i_eularAngles),
	m_fieldOfView(i_fieldOfView),
	m_nearPlaneDistance(i_nearPlaneDistance),
	m_farPlaneDistance(i_farPlaneDistance)
{
	UpdateLocalCameraAxes();
}

cs6610::Camera::Camera::~Camera() {}

void cs6610::Camera::Camera::UpdateCurrentCameraPosition()
{
	cyPoint3f localOffset = cyPoint3f(0.0f);

	if (UserInput::UserInput::keys['w'])
		localOffset.x += 1.0f;
	if (UserInput::UserInput::keys['s'])
		localOffset.x -= 1.0f;
	if (UserInput::UserInput::keys['d'])
		localOffset.z += 1.0f;
	if (UserInput::UserInput::keys['a'])
		localOffset.z -= 1.0f;

	const float speed_unitsPerSecond = 1.0f;
	const float offsetModifier = speed_unitsPerSecond * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame());
	localOffset *= offsetModifier;

	m_position += localOffset;
}
void cs6610::Camera::Camera::UpdateCurrentCameraOrientation(bool constrainPitch)
{
	cyPoint3f localOffset = cyPoint3f(0.0f);

	if (UserInput::UserInput::keys['H'])
		localOffset.y += 1.0f;
	if (UserInput::UserInput::keys['F'])
		localOffset.y -= 1.0f;
	if (UserInput::UserInput::keys['G'])
		localOffset.x += 1.0f;
	if (UserInput::UserInput::keys['T'])
		localOffset.x -= 1.0f;

	const float speed_unitsPerSecond = 10.0f;
	const float offsetModifier = speed_unitsPerSecond * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame());
	localOffset *= offsetModifier;
	m_eularAngles += localOffset;

	if (constrainPitch)
	{
		if (m_eularAngles.x > 89.0f)
		{
			m_eularAngles.x = 89.0f;
		}
		if (m_eularAngles.x < -89.0f)
		{
			m_eularAngles.x = -89.0f;
		}
	}
	UpdateLocalCameraAxes();
}

void cs6610::Camera::Camera::UpdateLocalCameraAxes()
{
	m_localCameraAxis.m_front.z = -(cosf(Math::ConvertDegreesToRadians(m_eularAngles.y)) * cosf(Math::ConvertDegreesToRadians(m_eularAngles.x)));
	m_localCameraAxis.m_front.y = -sinf(Math::ConvertDegreesToRadians(m_eularAngles.x));
	m_localCameraAxis.m_front.x = sinf(Math::ConvertDegreesToRadians(m_eularAngles.y)) * cosf(Math::ConvertDegreesToRadians(m_eularAngles.x));

	m_localCameraAxis.m_front.Normalize();
	m_localCameraAxis.m_right = (m_localCameraAxis.m_front.Cross(cyPoint3f(0.0f, 1.0f, 0.0f))).GetNormalized();
	m_localCameraAxis.m_up = (m_localCameraAxis.m_right.Cross(m_localCameraAxis.m_front)).GetNormalized();
}